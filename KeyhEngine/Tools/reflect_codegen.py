#!/usr/bin/env python3
"""
KeyhEngine Reflect Code Generator
===================================
Scans project header files for REFLECTIVE classes annotated with
KEYH_REFLECT_PROPERTY / KEYH_REFLECT_PROPERTY_GROUP macros and KEYH_REFLECT_ENUM
annotated enums, then generates one .inl file *per header* (named
<HeaderBaseName>.reflect_generated.inl) containing:
  - keyh::ReflectObject<T>::initializeMetaObject() template specialisations
  - ReflectEnumTraits registrations for marked enums

Usage
-----
    python reflect_codegen.py --project-dir <dir> [--output-dir <dir>]
                                       [--verbose] [--no-patch-headers]

Pre-build event example (Visual Studio .vcxproj)
-------------------------------------------------
    <PreBuildEvent>
      <Command>call "$(SolutionDir)..\\Tools\\run_reflect_codegen.bat"
               "$(ProjectDir)."</Command>
    </PreBuildEvent>

Auto-patching source headers
-----------------------------
By default the generator automatically appends
    #include "<HeaderBaseName>.reflect_generated.inl"
to every header that contains a REFLECTIVE class or KEYH_REFLECT_ENUM
(if not already present).
Pass --no-patch-headers to disable this behaviour and manage the include
manually.
"""

import re
import os
import sys
import argparse
from datetime import datetime


# ---------------------------------------------------------------------------
# Macro argument parsing
# ---------------------------------------------------------------------------

def _parse_property_args(args_str):
    """
    Parse the argument string of KEYH_REFLECT_PROPERTY(...).

    Recognised key=value pairs
    --------------------------
    PropertyName = "SomeName"    – override the property name (strip quotes)
    Default      = <expr>        – default-value expression (copied verbatim)

    Returns (property_name_override, default_value_str).
    Both values are None when the corresponding key is absent.
    """
    property_name = None
    default_value = None

    if not args_str or not args_str.strip():
        return property_name, default_value

    # PropertyName = "..."  (use word boundary to avoid matching substrings)
    pn_match = re.search(r'\bPropertyName\s*=\s*"([^"]*)"', args_str)
    if pn_match:
        property_name = pn_match.group(1)

    # Default = <expr>   – consume until the next top-level comma or end
    d_match_start = re.search(r'\bDefault\s*=\s*', args_str)
    if d_match_start:
        rest = args_str[d_match_start.end():]
        depth = 0
        end = len(rest)
        for i, ch in enumerate(rest):
            if ch in '<({':
                depth += 1
            elif ch in '>)}':
                depth -= 1
            elif ch == ',' and depth == 0:
                end = i
                break
        default_value = rest[:end].strip()

    return property_name, default_value


# ---------------------------------------------------------------------------
# Variable declaration parsing
# ---------------------------------------------------------------------------

def _parse_variable_declaration(line):
    """
    Extract (type_str, var_name) from a C++ variable declaration line such as:
        int                      _intValue = 0;
        float                    _floatValue = 0.0f;
        StaticStringA            _stringValue;
        keyh::Vector<int>        _vec;
        const keyh::HashMap<K,V> _map;

    Returns (None, None) when the line cannot be parsed.
    """
    line = line.strip()
    if not line:
        return None, None

    # Remove trailing semicolon
    if line.endswith(';'):
        line = line[:-1].strip()

    # Remove initialiser (everything from the first top-level '=')
    depth = 0
    eq_idx = -1
    for i, ch in enumerate(line):
        if ch in '<({':
            depth += 1
        elif ch in '>)}':
            depth -= 1
        elif ch == '=' and depth == 0:
            eq_idx = i
            break
    if eq_idx != -1:
        line = line[:eq_idx].strip()

    if not line:
        return None, None

    # The variable name is the last C++ identifier in the remaining string.
    i = len(line) - 1
    while i >= 0 and line[i].isspace():
        i -= 1

    end = i + 1
    while i >= 0 and (line[i].isalnum() or line[i] == '_'):
        i -= 1
    start = i + 1

    if start >= end:
        return None, None

    var_name = line[start:end]
    if not re.match(r'^[A-Za-z_][A-Za-z0-9_]*$', var_name):
        return None, None

    type_name = line[:start].strip()
    if not type_name:
        return None, None

    return type_name, var_name


# ---------------------------------------------------------------------------
# Comment stripping (line comments only)
# ---------------------------------------------------------------------------

def _strip_line_comment(line):
    """Remove a C++ '//' line comment, respecting double-quoted strings.

    Escape sequences inside strings are handled by advancing two characters
    whenever a backslash is encountered, which correctly covers both '\\' and
    '\"' without look-behind ambiguity.
    """
    in_string = False
    i = 0
    while i < len(line):
        ch = line[i]
        if ch == '\\' and in_string:
            # Skip the next character regardless of what it is; this handles
            # '\\' (escaped backslash) and '\"' (escaped quote) uniformly.
            i += 2
            continue
        if ch == '"':
            in_string = not in_string
        elif not in_string and ch == '/' and i + 1 < len(line) and line[i + 1] == '/':
            return line[:i]
        i += 1
    return line


# ---------------------------------------------------------------------------
# Enum parsing helpers
# ---------------------------------------------------------------------------

def _strip_block_comments(text):
    """Remove C-style block comments from a string."""
    return re.sub(r'/\*.*?\*/', '', text, flags=re.DOTALL)


def _split_top_level_commas(text):
    """Split a string by top-level commas (ignoring nested (), {}, [], <>)."""
    parts = []
    current = []
    depth = 0

    for ch in text:
        if ch in '([{<':
            depth += 1
        elif ch in ')]}>':
            depth = max(0, depth - 1)
        elif ch == ',' and depth == 0:
            part = ''.join(current).strip()
            if part:
                parts.append(part)
            current = []
            continue

        current.append(ch)

    tail = ''.join(current).strip()
    if tail:
        parts.append(tail)

    return parts


def _parse_enum_entries(enum_body):
    """Extract enum entry identifiers from an enum body."""
    cleaned = _strip_block_comments(enum_body)
    cleaned_lines = [_strip_line_comment(line) for line in cleaned.split('\n')]
    cleaned = '\n'.join(cleaned_lines)

    entries = []
    for token in _split_top_level_commas(cleaned):
        # Remove trailing attributes or initializers from each entry token
        token = token.strip()
        if not token:
            continue

        m = re.match(r'^([A-Za-z_][A-Za-z0-9_]*)', token)
        if not m:
            continue

        entries.append(m.group(1))

    return entries


def find_reflect_enums(filepath):
    """
    Scan one header file and return a list of (EnumName, [entry_names]) tuples
    for every enum annotated with KEYH_REFLECT_ENUM.
    """
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()

    enums = []
    pattern = re.compile(
        r'KEYH_REFLECT_ENUM\b[\s\r\n]*enum(?:\s+class)?\s+([A-Za-z_][A-Za-z0-9_]*)\s*(?:\:\s*[^{};\n]+)?\s*\{',
        re.MULTILINE
    )

    for match in pattern.finditer(content):
        enum_name = match.group(1)
        body_start = match.end()

        depth = 1
        i = body_start
        while i < len(content) and depth > 0:
            ch = content[i]
            if ch == '{':
                depth += 1
            elif ch == '}':
                depth -= 1
            i += 1

        if depth != 0:
            continue

        enum_body = content[body_start:i - 1]
        entries = _parse_enum_entries(enum_body)
        if entries:
            enums.append((enum_name, entries))

    return enums


# ---------------------------------------------------------------------------
# Class body scanner
# ---------------------------------------------------------------------------

def _parse_class_body(lines, start_line):
    """
    Scan a class body that begins at *start_line* (the line **after** the
    opening '{') and collect reflected property metadata.

    Returns a list of property dicts with keys:
        type         – C++ type string
        var_name     – member variable name (e.g. '_intValue')
        prop_name    – property name (override or var_name)
        default_value – default-value expression string, or None
        group        – group name string, or None (no group)
    """
    properties = []
    current_group = None  # None → no group (FlyweightStringA::Empty)
    depth = 1             # We start one level inside the class brace
    i = start_line

    while i < len(lines) and depth > 0:
        raw = lines[i]
        stripped = _strip_line_comment(raw).strip()

        if depth == 1 and stripped:
            # ----------------------------------------------------------------
            # KEYH_REFLECT_PROPERTY_GROUP("GroupName")
            # ----------------------------------------------------------------
            m = re.match(r'KEYH_REFLECT_PROPERTY_GROUP\s*\(\s*"([^"]*)"\s*\)', stripped)
            if m:
                current_group = m.group(1)
                for ch in stripped:
                    if ch == '{':
                        depth += 1
                    elif ch == '}':
                        depth -= 1
                i += 1
                continue

            # ----------------------------------------------------------------
            # KEYH_REFLECT_PROPERTY_GROUP_END
            # ----------------------------------------------------------------
            if re.match(r'KEYH_REFLECT_PROPERTY_GROUP_END\b', stripped):
                current_group = None
                for ch in stripped:
                    if ch == '{':
                        depth += 1
                    elif ch == '}':
                        depth -= 1
                i += 1
                continue

            # ----------------------------------------------------------------
            # KEYH_REFLECT_PROPERTY(...)
            # Use a depth-counting parser so that arguments containing nested
            # parentheses (e.g. Default = SomeType(1, 2)) are matched correctly.
            # ----------------------------------------------------------------
            if re.match(r'KEYH_REFLECT_PROPERTY\s*\(', stripped):
                # Extract argument string between the outer parentheses
                paren_start = stripped.index('(')
                args_str = ''
                depth_p = 0
                for ci, ch in enumerate(stripped[paren_start:]):
                    if ch == '(':
                        depth_p += 1
                    elif ch == ')':
                        depth_p -= 1
                        if depth_p == 0:
                            args_str = stripped[paren_start + 1:paren_start + ci].strip()
                            break

                prop_name_override, default_val = _parse_property_args(args_str)

                # The variable declaration is on the next non-blank line
                j = i + 1
                while j < len(lines) and not _strip_line_comment(lines[j]).strip():
                    j += 1

                if j < len(lines):
                    decl_line = _strip_line_comment(lines[j]).strip()
                    type_name, var_name = _parse_variable_declaration(decl_line)
                    if type_name and var_name:
                        properties.append({
                            'type':          type_name,
                            'var_name':      var_name,
                            'prop_name':     prop_name_override or var_name,
                            'default_value': default_val,
                            'group':         current_group,
                        })

                for ch in stripped:
                    if ch == '{':
                        depth += 1
                    elif ch == '}':
                        depth -= 1
                i += 1
                continue

        # Count braces for all other lines to track nesting depth
        for ch in stripped:
            if ch == '{':
                depth += 1
            elif ch == '}':
                depth -= 1

        i += 1

    return properties


# ---------------------------------------------------------------------------
# Header file scanner
# ---------------------------------------------------------------------------

def find_reflective_classes(filepath):
    """
    Scan one header file and return a list of (ClassName, [properties]) tuples
    for every class/struct decorated with REFLECTIVE(ClassName).
    """
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()

    lines = content.split('\n')
    classes = []
    i = 0

    while i < len(lines):
        stripped = _strip_line_comment(lines[i]).strip()

        # Match:  class REFLECTIVE(ClassName)  or  struct REFLECTIVE(ClassName)
        m = re.match(r'(?:class|struct)\s+REFLECTIVE\s*\(\s*(\w+)\s*\)', stripped)
        if m:
            class_name = m.group(1)

            # Locate the opening brace (may be further down)
            j = i
            while j < len(lines):
                if '{' in _strip_line_comment(lines[j]):
                    break
                j += 1

            if j < len(lines):
                props = _parse_class_body(lines, j + 1)
                classes.append((class_name, props))

        i += 1

    return classes


# ---------------------------------------------------------------------------
# Code generation
# ---------------------------------------------------------------------------

def _safe_group_var(group_name):
    """Convert an arbitrary group name string into a safe C++ identifier."""
    return 'sGroupName_' + re.sub(r'[^A-Za-z0-9_]', '_', group_name)


def generate_inl_content(all_classes, all_enums, source_filename, output_filename):
    """
    Produce the full text of the generated .inl file for a single header.

    Parameters
    ----------
    all_classes   : list of (class_name, properties, source_filepath)
                    All entries must belong to one header.
    all_enums     : list of (enum_name, entries, source_filepath)
                    All entries must belong to one header.
    source_filename : base name of the originating header
    output_filename : the .inl filename (used in the usage comment)
    """
    timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

    lines = []
    lines.append('// AUTO-GENERATED FILE. DO NOT EDIT MANUALLY.')
    lines.append('// Generated by KeyhEngine Reflect Code Generator')
    lines.append(f'// Generated at: {timestamp}')
    lines.append('//')
    lines.append('// This file is automatically #include-d at the end of its REFLECTIVE/KEYH_REFLECT_ENUM header')
    lines.append('// by the code generator (reflect_codegen.py).  You do not need to include it')
    lines.append('// manually; re-run the generator (or let the pre-build event do it) and it')
    lines.append('// will be appended to the following header if not already present:')
    lines.append(f'//   {source_filename}')
    lines.append('//')
    lines.append('// Pre-build event snippet for .vcxproj:')
    lines.append('//   <PreBuildEvent>')
    lines.append('//     <Command>call "$(SolutionDir)Tools\\run_reflect_codegen.bat"')
    lines.append('//              "$(ProjectDir)."</Command>')
    lines.append('//   </PreBuildEvent>')
    lines.append('')

    for enum_name, entries, _ in all_enums:
        if not entries:
            continue

        lines.append('namespace keyh')
        lines.append('{')
        lines.append(f'\tKEYH_REFLECT_ENUM_BEGIN({enum_name})')
        for entry in entries:
            lines.append(f'\t\tKEYH_REFLECT_ENUM_VALUE({enum_name}, {entry})')
        lines.append(f'\tKEYH_REFLECT_ENUM_END({enum_name})')
        lines.append('}')
        lines.append('')

    for class_name, properties, _ in all_classes:
        if not properties:
            continue

        lines.append('namespace keyh')
        lines.append('{')
        lines.append('\ttemplate<>')
        lines.append(f'\tinline ReflectMetaObject ReflectObject<{class_name}>::initializeMetaObject()')
        lines.append('\t{')
        lines.append('\t\tReflectMetaObject metaObject;')
        lines.append('')

        # Declare a static FlyweightStringA for each unique group used by this class
        used_groups = {}
        for prop in properties:
            g = prop['group']
            if g and g not in used_groups:
                used_groups[g] = _safe_group_var(g)

        if used_groups:
            for group_name, var_name in used_groups.items():
                lines.append(f'\t\tstatic const FlyweightStringA {var_name}("{group_name}");')
            lines.append('')

        for prop in properties:
            type_name    = prop['type']
            var_name     = prop['var_name']
            prop_name    = prop['prop_name']
            default_val  = prop['default_value']
            group        = prop['group']

            group_arg   = used_groups[group] if group else 'FlyweightStringA::Empty'
            default_arg = default_val if default_val is not None else f'{type_name}{{}}'

            lines.append(f'\t\tmetaObject.addReflectProperty<{class_name}, {type_name}>(')
            lines.append(f'\t\t\tFlyweightStringA("{prop_name}"),')
            lines.append(f'\t\t\t{group_arg},')
            lines.append(f'\t\t\t{default_arg},')
            lines.append(f'\t\t\t[]({class_name}& obj) -> {type_name}& {{ return obj.{var_name}; }},')
            lines.append(f'\t\t\t[](const {class_name}& obj) -> const {type_name}& {{ return obj.{var_name}; }}')
            lines.append('\t\t);')
            lines.append('')

        lines.append('\t\treturn metaObject;')
        lines.append('\t}')
        lines.append('}')
        lines.append('')

    return '\n'.join(lines)


# ---------------------------------------------------------------------------
# Header auto-patch: manage generated #include directives
# ---------------------------------------------------------------------------

def patch_header_with_include(filepath, include_line):
    """
    Remove stale generated .inl includes from *filepath* and ensure that the
    current *include_line* (e.g. '#include "reflect_generated.inl"') exists
    exactly once, preserving an already-correct final generated include when
    possible.

    Returns True when the file was modified, False when it was already in the
    expected state (or the file could not be read/written).
    """
    try:
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
    except OSError as exc:
        print(f'[Reflect] Warning: could not read {filepath} for patching: {exc}', file=sys.stderr)
        return False

    newline_match = re.search(r'\r\n|\n|\r', content)
    newline = newline_match.group(0) if newline_match else '\n'
    generated_include_pattern = re.compile(
        r'^[ \t]*(#include\s+"[^"\r\n]+\.reflect_generated\.inl")[ \t]*(?:\r?\n|$)',
        re.MULTILINE
    )
    lines = content.splitlines(keepends=True)
    generated_include_indexes = []
    current_include_indexes = []

    for index, line in enumerate(lines):
        match = generated_include_pattern.fullmatch(line)
        if not match:
            continue

        generated_include_indexes.append(index)
        if match.group(1) == include_line:
            current_include_indexes.append(index)

    preserved_include_index = None
    if current_include_indexes and generated_include_indexes:
        last_generated_index = generated_include_indexes[-1]
        last_current_index = current_include_indexes[-1]
        if last_current_index == last_generated_index:
            preserved_include_index = last_current_index

    kept_lines = []
    include_present = False
    modified = False

    for index, line in enumerate(lines):
        match = generated_include_pattern.fullmatch(line)
        if not match:
            kept_lines.append(line)
            continue

        if index == preserved_include_index:
            include_present = True
            kept_lines.append(line)
        else:
            modified = True

    updated_content = ''.join(kept_lines)

    if not include_present:
        if updated_content and not updated_content.endswith(('\n', '\r')):
            updated_content += newline
        updated_content += include_line + newline
        modified = True
    elif updated_content != content:
        modified = True

    if not modified:
        return False

    try:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(updated_content)
    except OSError as exc:
        print(f'[Reflect] Warning: could not write {filepath} during patching: {exc}', file=sys.stderr)
        return False

    return True


def remove_header_include(filepath, include_line):
    """Remove standalone occurrences of a generated *include_line* from a header."""
    try:
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            content = f.read()
    except OSError as exc:
        print(f'[Reflect] Warning: could not read {filepath} for patching: {exc}', file=sys.stderr)
        return False

    pattern = re.compile(rf'^[ \t]*{re.escape(include_line)}[ \t]*(?:\r?\n|$)', re.MULTILINE)
    updated_content, count = pattern.subn('', content)
    if not count:
        return False

    try:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(updated_content)
    except OSError as exc:
        print(f'[Reflect] Warning: could not write {filepath} during patching: {exc}', file=sys.stderr)
        return False

    return True


def remove_generated_inl(filepath):
    """Delete *filepath* only when it is a reflection generator output."""
    if not os.path.isfile(filepath):
        return False

    try:
        with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
            is_generated = f.readline().strip() == '// AUTO-GENERATED FILE. DO NOT EDIT MANUALLY.'
    except OSError as exc:
        print(f'[Reflect] Warning: could not read generated file {filepath}: {exc}', file=sys.stderr)
        return False

    if not is_generated:
        return False

    try:
        os.remove(filepath)
    except OSError as exc:
        print(f'[Reflect] Warning: could not remove generated file {filepath}: {exc}', file=sys.stderr)
        return False

    return True


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def _inl_name_for_header(header_basename):
    """Return the per-header .inl filename for a given header base name."""
    stem = os.path.splitext(header_basename)[0]
    return f'{stem}.reflect_generated.inl'


def main():
    parser = argparse.ArgumentParser(
        description='KeyhEngine Reflect Code Generator – generates initializeMetaObject() '
                    'specialisations for REFLECTIVE classes and enum trait mappings '
                    'for KEYH_REFLECT_ENUM, one .inl file per header.'
    )
    parser.add_argument(
        '--project-dir', required=True,
        help='Project directory containing .h header files to scan.'
    )
    parser.add_argument(
        '--output-dir', default=None,
        help='Directory for generated .inl files (default: same directory as each header).'
    )
    parser.add_argument(
        '--no-patch-headers', action='store_true',
        help='Do not automatically append the #include directive to REFLECTIVE headers.'
    )
    parser.add_argument(
        '--verbose', action='store_true',
        help='Print detailed progress information.'
    )
    args = parser.parse_args()

    project_dir = os.path.abspath(args.project_dir)
    global_output_dir = os.path.abspath(args.output_dir) if args.output_dir else None

    if not os.path.isdir(project_dir):
        print(f'[Reflect] Error: project directory not found: {project_dir}', file=sys.stderr)
        sys.exit(1)

    if global_output_dir:
        os.makedirs(global_output_dir, exist_ok=True)

    # Collect all .h files under the project directory recursively (skip generated files)
    header_files = sorted(
        os.path.join(root, fname)
        for root, _dirs, files in os.walk(project_dir)
        for fname in files
        if fname.endswith('.h') and not fname.endswith('.generated.h')
    )

    print(f'[Reflect] Starting - scanning {len(header_files)} header(s) in {project_dir}')

    if args.verbose:
        for fp in header_files:
            print(f'[Reflect]   {os.path.relpath(fp, project_dir)}')

    # Group collected symbols by source header path.
    classes_by_header = {}  # filepath -> [(class_name, properties, filepath), ...]
    enums_by_header = {}    # filepath -> [(enum_name, entries, filepath), ...]

    for filepath in header_files:
        try:
            classes = find_reflective_classes(filepath)
            enums = find_reflect_enums(filepath)
        except Exception as exc:
            print(f'[Reflect] Warning: could not parse {filepath}: {exc}', file=sys.stderr)
            continue

        for class_name, properties in classes:
            if properties:
                classes_by_header.setdefault(filepath, []).append(
                    (class_name, properties, filepath)
                )
                if args.verbose:
                    print(
                        f'[Reflect]   {os.path.basename(filepath)}: '
                        f'{class_name} - {len(properties)} property(ies)'
                    )

        for enum_name, entries in enums:
            if entries:
                enums_by_header.setdefault(filepath, []).append(
                    (enum_name, entries, filepath)
                )
                if args.verbose:
                    print(
                        f'[Reflect]   {os.path.basename(filepath)}: '
                        f'{enum_name} - {len(entries)} enum value(s)'
                    )

    source_headers = sorted(set(classes_by_header.keys()) | set(enums_by_header.keys()))
    source_header_set = set(source_headers)
    active_output_paths = {
        os.path.join(
            global_output_dir if global_output_dir else os.path.dirname(header_path),
            _inl_name_for_header(os.path.basename(header_path))
        )
        for header_path in source_headers
    }
    removed_includes = 0
    removed_files = 0

    for header_path in header_files:
        if header_path in source_header_set:
            continue

        output_name = _inl_name_for_header(os.path.basename(header_path))
        output_path = os.path.join(
            global_output_dir if global_output_dir else os.path.dirname(header_path),
            output_name
        )
        include_directive = f'#include "{output_name}"'

        if not args.no_patch_headers and remove_header_include(header_path, include_directive):
            removed_includes += 1
            if args.verbose:
                print(f'[Reflect]   Patched {os.path.basename(header_path)} – removed {include_directive}')

        # Do not remove an output name that is still generated for another header.
        if output_path not in active_output_paths and remove_generated_inl(output_path):
            removed_files += 1
            if args.verbose:
                print(f'[Reflect]   Removed stale {output_name}')

    if not source_headers:
        print(
            '[Reflect] Done - no REFLECTIVE classes or KEYH_REFLECT_ENUM enums found. '
            f'Removed {removed_files} stale file(s) and {removed_includes} include(s).'
        )
        return

    generated_files = []
    total_classes = 0
    total_props = 0
    total_enums = 0
    total_enum_values = 0
    script_mtime = os.path.getmtime(os.path.abspath(__file__))

    for header_path in source_headers:
        header_basename = os.path.basename(header_path)
        output_name = _inl_name_for_header(header_basename)
        output_dir = global_output_dir if global_output_dir else os.path.dirname(header_path)
        output_path = os.path.join(output_dir, output_name)

        class_list = classes_by_header.get(header_path, [])
        enum_list = enums_by_header.get(header_path, [])

        # Per-header up-to-date check against this header + script timestamp.
        is_up_to_date = False
        if os.path.isfile(output_path):
            output_mtime = os.path.getmtime(output_path)
            try:
                header_mtime = os.path.getmtime(header_path)
            except FileNotFoundError:
                header_mtime = 0.0
            is_up_to_date = max(header_mtime, script_mtime) <= output_mtime

        if is_up_to_date:
            if args.verbose:
                print(f'[Reflect]   Up-to-date: {output_name}')
            if not args.no_patch_headers:
                include_directive = f'#include "{output_name}"'
                patch_header_with_include(header_path, include_directive)
            continue

        content = generate_inl_content(class_list, enum_list, header_basename, output_name)
        with open(output_path, 'w', encoding='utf-8') as f:
            f.write(content)

        generated_files.append(output_name)
        total_classes += len(class_list)
        total_props += sum(len(props) for _, props, _ in class_list)
        total_enums += len(enum_list)
        total_enum_values += sum(len(entries) for _, entries, _ in enum_list)

        if args.verbose:
            print(
                f'[Reflect]   Generated {output_name} '
                f'({len(class_list)} class(es), {sum(len(props) for _, props, _ in class_list)} prop(s), '
                f'{len(enum_list)} enum(s), {sum(len(entries) for _, entries, _ in enum_list)} value(s))'
            )

        if not args.no_patch_headers:
            include_directive = f'#include "{output_name}"'
            if patch_header_with_include(header_path, include_directive) and args.verbose:
                print(f'[Reflect]   Patched {header_basename} – appended {include_directive}')

    cleanup_summary = f'Removed {removed_files} stale file(s) and {removed_includes} include(s).'
    if generated_files:
        print(
            f'[Reflect] Done - generated {len(generated_files)} file(s) / '
            f'{total_classes} class(es) / {total_props} property(ies), '
            f'{total_enums} enum(s) / {total_enum_values} value(s): '
            f'{", ".join(generated_files)}. {cleanup_summary}'
        )
    else:
        print(f'[Reflect] Done - all generated files are up-to-date. {cleanup_summary}')


if __name__ == '__main__':
    main()
