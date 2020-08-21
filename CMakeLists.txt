cmake_minimum_required(VERSION 3.10)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(raylib_template C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(libs/raylib)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} raylib)

if(APPLE)
    # Create app bundle
    set(MACOSX_BUNDLE_NAME "raylib_template")
    set(MACOSX_BUNDLE_PACKAGE_NAME "rangeli.raylib_template")

    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_NAME ${MACOSX_BUNDLE_NAME}
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/MacOSXBundleInfo.plist.in)
endif()


