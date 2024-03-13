cmake_minimum_required(VERSION 3.10)
find_package(OpenGL REQUIRED)
project(FLGL)

# Add GLFW
add_subdirectory(lib/glfw)

# Add GLAD
add_library(GLAD lib/glad/src/glad.c)
target_include_directories(GLAD PUBLIC lib/glad/include)

# Include directories for FLGL
include_directories(inc lib/glfw/include lib/glad/include)

# If your FLGL library has source files, list them here
file(GLOB_RECURSE FLGL_SOURCES "src/*.cpp")

# Create the FLGL library
add_library(${PROJECT_NAME} ${FLGL_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw/include)

# Link FLGL with GLFW and GLAD
target_link_libraries(${PROJECT_NAME} PUBLIC glfw GLAD)
target_include_directories(${PROJECT_NAME}  PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}  PUBLIC ${OPENGL_gl_LIBRARY})
