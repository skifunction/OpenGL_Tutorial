cmake_minimum_required(VERSION 3.10)
project(OpenGLTutorial)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)
add_executable(OpenGLTutorialExe ${SRC_FILES})

# External libraries
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/../external/glfw")
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/../external/glad")
set(GLM_DIR "${CMAKE_SOURCE_DIR}/../external/glm")
add_subdirectory(${GLFW_DIR} "${CMAKE_BINARY_DIR}/glfw_build")
add_subdirectory(${GLAD_DIR} "${CMAKE_BINARY_DIR}/glad_build")

# Linking
target_link_libraries(OpenGLTutorialExe PRIVATE glfw Glad)

# Include directories
target_include_directories(OpenGLTutorialExe PRIVATE 
${GLFW_DIR}/include 
${GLAD_DIR}/include 
${GLM_DIR}
)

# On Windows, link additional system libraries
if(WIN32)
    target_link_libraries(OpenGLTutorialExe PRIVATE opengl32)
endif()
