@echo off
REM This script configures and builds the project using CMake and GCC.

REM Navigate to the build directory
if not exist build (
    mkdir build
)
cd build

REM Run CMake to generate Makefiles
cmake -G "MinGW Makefiles" ..

REM Build the project using the generated Makefiles
cmake --build .

echo Running OpenGLTutorialExe...
OpenGLTutorialExe.exe


REM Return to the original directory
cd ..
