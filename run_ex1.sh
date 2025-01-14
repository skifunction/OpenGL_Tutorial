#!/bin/bash
# This script configures and builds the project using CMake and GCC.

cd exercise

# Navigate to the build directory
if [ ! -d "build" ]; then
    mkdir build
fi
cd build

# Run CMake to generate Makefiles
cmake ..

# Build the project using the generated Makefiles
cmake --build .

echo "Running OpenGLTutorialExe..."
./OpenGLTutorialExe

# Return to the original directory
cd ../..