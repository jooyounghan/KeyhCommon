vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_from_git(
    OUT_SOURCE_PATH SOURCE_PATH
    URL "https://github.com/jooyounghan/KeyhCommon.git"
    REF "9838518699b5f662e66671635493f972b0f4a848"
)

set(KEYHCOMMON_PROJECT_ROOT "${SOURCE_PATH}/KeyhCommon")

if(VCPKG_TARGET_ARCHITECTURE STREQUAL "x86")
    set(KEYHCOMMON_PLATFORM "Win32")
elseif(VCPKG_TARGET_ARCHITECTURE STREQUAL "x64")
    set(KEYHCOMMON_PLATFORM "x64")
else()
    message(FATAL_ERROR "Unsupported target architecture: ${VCPKG_TARGET_ARCHITECTURE}")
endif()

foreach(project IN ITEMS CommonBase ReflectSystem AppSystem)
    vcpkg_build_msbuild(
        PROJECT_PATH "${KEYHCOMMON_PROJECT_ROOT}/${project}/${project}.vcxproj"
        PLATFORM "${KEYHCOMMON_PLATFORM}"
    )
endforeach()

function(keyhcommon_find_built_lib output_var project configuration)
    file(GLOB_RECURSE project_libs
        LIST_DIRECTORIES false
        "${KEYHCOMMON_PROJECT_ROOT}/${project}/*.lib"
    )

    set(configuration_matches "")
    foreach(project_lib IN LISTS project_libs)
        cmake_path(NORMAL_PATH project_lib OUTPUT_VARIABLE normalized_project_lib)
        if(configuration STREQUAL "Release" AND normalized_project_lib MATCHES "/Release/")
            list(APPEND configuration_matches "${project_lib}")
        elseif(configuration STREQUAL "Debug" AND normalized_project_lib MATCHES "/Debug/")
            list(APPEND configuration_matches "${project_lib}")
        endif()
    endforeach()

    list(LENGTH configuration_matches configuration_match_count)
    if(configuration_match_count EQUAL 0)
        message(FATAL_ERROR "Unable to locate ${configuration} library output for ${project}")
    endif()

    list(GET configuration_matches 0 selected_library)
    set(${output_var} "${selected_library}" PARENT_SCOPE)
endfunction()

foreach(project IN ITEMS CommonBase ReflectSystem AppSystem)
    keyhcommon_find_built_lib(release_library "${project}" Release)
    keyhcommon_find_built_lib(debug_library "${project}" Debug)

    file(INSTALL
        "${release_library}"
        DESTINATION "${CURRENT_PACKAGES_DIR}/lib"
    )
    file(INSTALL
        "${debug_library}"
        DESTINATION "${CURRENT_PACKAGES_DIR}/debug/lib"
    )

    string(TOLOWER "${project}" project_folder_name)
    file(GLOB project_headers
        LIST_DIRECTORIES false
        "${KEYHCOMMON_PROJECT_ROOT}/${project}/*.h"
        "${KEYHCOMMON_PROJECT_ROOT}/${project}/*.hpp"
        "${KEYHCOMMON_PROJECT_ROOT}/${project}/*.inl"
    )
    file(INSTALL
        ${project_headers}
        DESTINATION "${CURRENT_PACKAGES_DIR}/include/keyhcommon/${project_folder_name}"
    )
endforeach()

file(INSTALL
    "${KEYHCOMMON_PROJECT_ROOT}/Tools/reflect_codegen.py"
    DESTINATION "${CURRENT_PACKAGES_DIR}/tools/${PORT}"
)

# Git source archives can contain LF even when the checkout uses CRLF.
configure_file(
    "${KEYHCOMMON_PROJECT_ROOT}/Tools/run_reflect_codegen.bat"
    "${CURRENT_PACKAGES_DIR}/tools/${PORT}/run_reflect_codegen.bat"
    @ONLY
    NEWLINE_STYLE CRLF
)

file(INSTALL
    "${KEYHCOMMON_PROJECT_ROOT}/CommonBase/CommonBase.natvis"
    DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
)

file(INSTALL
    "${CMAKE_CURRENT_LIST_DIR}/KeyhCommonConfig.cmake"
    DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
)

file(INSTALL
    "${SOURCE_PATH}/LICENSE"
    DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
    RENAME copyright
)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
