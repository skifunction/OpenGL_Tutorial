cmake_minimum_required(VERSION 3.10)
project(OpenGLTutorial)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)
add_executable(OpenGLTutorialExe ${SRC_FILES})

# External libraries
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/vendor/glfw")
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/vendor/glad")
set(GLM_DIR "${CMAKE_SOURCE_DIR}/vendor/glm")
add_subdirectory(${GLFW_DIR})
add_subdirectory(${GLAD_DIR})

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
