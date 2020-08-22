cmake_minimum_required(VERSION 3.10)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(raylib_invaders C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(libs/raylib)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE RESOURCE_FILES "resources/*")

add_executable(${PROJECT_NAME} ${SOURCES} ${RESOURCE_FILES})
target_link_libraries(${PROJECT_NAME} raylib)

if(APPLE)
    # Create app bundle
    set(MACOSX_BUNDLE_NAME "Raylib Invaders")
    set(MACOSX_BUNDLE_PACKAGE_NAME "rangeli.raylib_invaders")

    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE ON
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/MacOSXBundleInfo.plist.in)
endif()


