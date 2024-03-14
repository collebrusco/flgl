cmake_minimum_required(VERSION 3.10)
project(FLGL)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_subdirectory(lib/glfw)

add_library(GLAD lib/glad/src/glad.c)
target_include_directories(GLAD PUBLIC lib/glad/include)

include_directories(inc lib/glfw/include lib/glad/include ${OPENGL_INCLUDE_DIR})

file(GLOB_RECURSE FLGL_SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} ${FLGL_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_gl_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/include)

target_link_libraries(${PROJECT_NAME} PUBLIC GLAD glfw ${OPENGL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
