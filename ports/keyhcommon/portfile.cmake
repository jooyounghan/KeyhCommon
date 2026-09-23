vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

set(SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../..")
set(KEYHCOMMON_PROJECT_ROOT "${SOURCE_PATH}/KeyhCommon")

if(NOT EXISTS "${KEYHCOMMON_PROJECT_ROOT}/KeyhCommon.slnx")
    message(FATAL_ERROR "Expected source tree was not found at ${KEYHCOMMON_PROJECT_ROOT}")
endif()

foreach(project IN ITEMS CommonBase ReflectSystem AppSystem)
    vcpkg_install_msbuild(
        SOURCE_PATH "${SOURCE_PATH}"
        PROJECT_SUBPATH "KeyhCommon/${project}/${project}.vcxproj"
        LICENSE_SUBPATH "LICENSE"
        ALLOW_ROOT_INCLUDES
    )
endforeach()

file(MAKE_DIRECTORY "${CURRENT_PACKAGES_DIR}/include/keyhcommon")

foreach(project IN ITEMS CommonBase ReflectSystem AppSystem)
    file(GLOB project_headers
        LIST_DIRECTORIES false
        "${KEYHCOMMON_PROJECT_ROOT}/${project}/*.h"
        "${KEYHCOMMON_PROJECT_ROOT}/${project}/*.hpp"
        "${KEYHCOMMON_PROJECT_ROOT}/${project}/*.inl"
    )
    file(INSTALL ${project_headers} DESTINATION "${CURRENT_PACKAGES_DIR}/include/keyhcommon")
endforeach()

file(INSTALL
    "${KEYHCOMMON_PROJECT_ROOT}/CommonBase/CommonBase.natvis"
    DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
)

file(INSTALL
    "${CMAKE_CURRENT_LIST_DIR}/KeyhCommonConfig.cmake"
    DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
)

file(INSTALL
    "${CMAKE_CURRENT_LIST_DIR}/usage"
    DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
