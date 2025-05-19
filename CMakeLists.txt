cmake_minimum_required(VERSION 3.10)
project(FLGL)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

if (WIN32)
    message(STATUS "Checking for OpenAL at C:/Program Files (x86)/OpenAL 1.1 SDK")

    find_path(OPENAL_INCLUDE_DIR al.h
        PATHS "C:/Program Files (x86)/OpenAL 1.1 SDK/include"
    )

    find_library(OPENAL_LIBRARY OpenAL32
        PATHS "C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win64"
            "C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win32"
    )


    message(STATUS "Resolved OPENAL_INCLUDE_DIR to: ${OPENAL_INCLUDE_DIR}")
    message(STATUS "Resolved OPENAL_LIBRARY to: ${OPENAL_LIBRARY}")

    if(NOT OPENAL_INCLUDE_DIR OR NOT OPENAL_LIBRARY)
        message(FATAL_ERROR
            "Could not find OpenAL in 'C:/Program Files (x86)/OpenAL 1.1 SDK'.\n"
            "Please install the OpenAL SDK there (default) and try again."
        )
    endif()
else()
    find_package(OpenAL REQUIRED)
endif()

add_subdirectory(lib/glfw)

add_library(GLAD lib/glad/src/glad.c)
target_include_directories(GLAD PUBLIC lib/glad/include)

include_directories(inc lib/glfw/include lib/glad/include ${OPENGL_INCLUDE_DIR})

file(GLOB_RECURSE FLGL_SOURCES "src/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-variable -Werror=unused-variable")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-qual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wc99-extensions -Werror=c99-extensions -Wvla-extension -Werror=vla-extension")
endif()

add_library(${PROJECT_NAME} ${FLGL_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_gl_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/include)

target_include_directories(${PROJECT_NAME} PUBLIC ${OPENAL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OPENAL_LIBRARY})

target_link_libraries(${PROJECT_NAME} PUBLIC GLAD glfw ${OPENGL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
