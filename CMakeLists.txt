# =============================================================================
#  flgl — CMake build (an alternative to the primary `make` / flgl.mk flow).
#
#  builds a static `flgl` target from the same vendored, self-contained tree.
#  flgl.mk is the reference build; this mirrors it closely enough to be a drop-in
#  for projects that prefer cmake. glfw builds from its own bundled cmake.
#
#    standalone:  cmake -S . -B build && cmake --build build
#    as consumer: add_subdirectory(lib/flgl) ; target_link_libraries(app flgl)
#
#  options: -DFLGL_AUDIO=OFF                 drop the coreal (miniaudio) module
#           -DFLGL_PATH=... (+ _ASSET_/_SHADER_)  override the baked-in paths
# =============================================================================
cmake_minimum_required(VERSION 3.16)
project(flgl CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(FLGL_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

option(FLGL_AUDIO "build the coreal audio module (miniaudio)" ON)

# runtime paths, baked in as -D defines (same defaults as flgl.mk; override freely)
set(FLGL_PATH        "lib/flgl/"   CACHE STRING "path from run cwd to flgl (default assets)")
set(FLGL_ASSET_PATH  "assets/tex/" CACHE STRING "texture asset path")
set(FLGL_SHADER_PATH "shaders/"    CACHE STRING "shader path")

# ---- vendored deps ----------------------------------------------------------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
add_subdirectory(${FLGL_ROOT}/lib/glfw)

add_library(flgl_glad STATIC ${FLGL_ROOT}/lib/glad/src/glad.c)
target_include_directories(flgl_glad PUBLIC ${FLGL_ROOT}/lib/glad/include)

# ---- discover sources (mirrors flgl.mk: src/core is always built; every dir
#      under foundation/ and framework/ is a toggleable module) ----------------
file(GLOB_RECURSE FLGL_SRC CONFIGURE_DEPENDS ${FLGL_ROOT}/src/core/*.cpp)
file(GLOB FLGL_MODULE_DIRS LIST_DIRECTORIES true
     ${FLGL_ROOT}/src/foundation/* ${FLGL_ROOT}/src/framework/*)

set(FLGL_MODULE_DEFS "")
foreach(dir ${FLGL_MODULE_DIRS})
    if(NOT IS_DIRECTORY ${dir})
        continue()
    endif()
    get_filename_component(mod ${dir} NAME)
    if(mod STREQUAL "coreal" AND NOT FLGL_AUDIO)
        continue()                       # audio off: skip the module entirely
    endif()
    file(GLOB_RECURSE mod_src CONFIGURE_DEPENDS ${dir}/*.cpp)
    list(APPEND FLGL_SRC ${mod_src})
    string(TOUPPER ${mod} mod_up)
    list(APPEND FLGL_MODULE_DEFS FLGL_MODULE_${mod_up})
endforeach()

# ---- the library ------------------------------------------------------------
add_library(flgl STATIC ${FLGL_SRC})

target_include_directories(flgl PUBLIC
    ${FLGL_ROOT}/inc
    ${FLGL_ROOT}/lib/glm
    ${FLGL_ROOT}/lib/stb/include
    ${FLGL_ROOT}/lib/miniaudio)

target_link_libraries(flgl PUBLIC flgl_glad glfw)

# defines are PUBLIC: a consumer compiling flgl headers needs the same
# FLGL_MODULE_* (public headers #error without them) and FLGL_HAS_AUDIO.
target_compile_definitions(flgl PUBLIC
    ${FLGL_MODULE_DEFS}
    _FLGL_PATH=\"${FLGL_PATH}\"
    _FLGL_ASSET_PATH=\"${FLGL_ASSET_PATH}\"
    _FLGL_SHADER_PATH=\"${FLGL_SHADER_PATH}\")
if(FLGL_AUDIO)
    target_compile_definitions(flgl PUBLIC FLGL_HAS_AUDIO)
endif()

if(NOT MSVC)
    target_compile_options(flgl PRIVATE -Wall -Wextra -Wpedantic -Werror=return-type)
endif()

# ---- platform link libs (mirror flgl.mk WINDOW_LIBS / AUDIO_LIBS) ------------
if(APPLE)
    target_link_libraries(flgl PUBLIC
        "-framework Cocoa" "-framework IOKit" "-framework CoreFoundation" "-framework OpenGL")
    if(FLGL_AUDIO)
        target_link_libraries(flgl PUBLIC "-framework CoreAudio" "-framework AudioToolbox")
    endif()
elseif(WIN32)
    target_link_libraries(flgl PUBLIC gdi32 opengl32)
    if(FLGL_AUDIO)
        target_link_libraries(flgl PUBLIC ole32)
    endif()
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(flgl PUBLIC X11 OpenGL::GL)
    if(FLGL_AUDIO)
        target_link_libraries(flgl PUBLIC dl pthread m)
    endif()
endif()
