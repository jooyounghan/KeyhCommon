get_filename_component(_KEYHCOMMON_PREFIX "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
set(_KEYHCOMMON_INCLUDE_ROOT "${_KEYHCOMMON_PREFIX}/include/keyhcommon")
set(KeyhCommon_TOOLS_DIR "${_KEYHCOMMON_PREFIX}/tools/keyhcommon")
set(KeyhCommon_REFLECT_CODEGEN "${KeyhCommon_TOOLS_DIR}/reflect_codegen.py")

if(NOT TARGET KeyhCommon::CommonBase)
    add_library(KeyhCommon::CommonBase STATIC IMPORTED)
    set_target_properties(KeyhCommon::CommonBase PROPERTIES
        IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
        IMPORTED_LOCATION_DEBUG "${_KEYHCOMMON_PREFIX}/debug/lib/CommonBase.lib"
        IMPORTED_LOCATION_RELEASE "${_KEYHCOMMON_PREFIX}/lib/CommonBase.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${_KEYHCOMMON_INCLUDE_ROOT}/commonbase"
    )
endif()

if(NOT TARGET KeyhCommon::ReflectSystem)
    add_library(KeyhCommon::ReflectSystem STATIC IMPORTED)
    set_target_properties(KeyhCommon::ReflectSystem PROPERTIES
        IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
        IMPORTED_LOCATION_DEBUG "${_KEYHCOMMON_PREFIX}/debug/lib/ReflectSystem.lib"
        IMPORTED_LOCATION_RELEASE "${_KEYHCOMMON_PREFIX}/lib/ReflectSystem.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${_KEYHCOMMON_INCLUDE_ROOT}/reflectsystem"
        INTERFACE_LINK_LIBRARIES "KeyhCommon::CommonBase"
    )
endif()

if(NOT TARGET KeyhCommon::AppSystem)
    add_library(KeyhCommon::AppSystem STATIC IMPORTED)
    set_target_properties(KeyhCommon::AppSystem PROPERTIES
        IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
        IMPORTED_LOCATION_DEBUG "${_KEYHCOMMON_PREFIX}/debug/lib/AppSystem.lib"
        IMPORTED_LOCATION_RELEASE "${_KEYHCOMMON_PREFIX}/lib/AppSystem.lib"
        INTERFACE_INCLUDE_DIRECTORIES "${_KEYHCOMMON_INCLUDE_ROOT}/appsystem"
        INTERFACE_LINK_LIBRARIES "KeyhCommon::CommonBase"
    )
endif()

if(NOT TARGET KeyhCommon::KeyhCommon)
    add_library(KeyhCommon::KeyhCommon INTERFACE IMPORTED)
    set_target_properties(KeyhCommon::KeyhCommon PROPERTIES
        INTERFACE_LINK_LIBRARIES "KeyhCommon::CommonBase;KeyhCommon::ReflectSystem;KeyhCommon::AppSystem"
    )
endif()
