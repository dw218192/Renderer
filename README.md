# Overview
This is a simple renderer library that supports the following functionalities:
1. render images from a single .obj mesh file.
2. accept user commands to change the view.

# Requirement
1. Operating Systems: Windows or Linux
2. The OpenGL Library
3. (If on Windows) Visual Studio 2015 or above
4. (If on Linux) CMake 3.11 or above, Make, GCC/G++ 7.3.0 or above

# How to Build
1. Clone the repository
2. Make a build directory in the top level directory: `mkdir build`
3. Change to the build directory: `cd build`
4. Run cmake to configure the project: `cmake ..`
5. Build the project
    * On Windows, open the generated .sln file in Visual Studio and build the project.
    * On Linux, run `make` in the build directory.