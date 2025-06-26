# Stencil Shadows
Stencil Shadows, also known as Shadow Volumes, use models, the depth buffer, and the stencil buffer to create shadows.

A "shadow volume" is created from a base model, which represents the area the light can't see in form of a 3D mesh. This shadow volume is then rendered after all other geometry, and will either pass or fail the depth test. The result of the depth test will then affect the stencil buffer, which will blot out all lighting where it detects a shadow.

My implementation uses OpenGL's vertex shader to create the shadow volumes, and forward rendering for the rendering pipeline.

## Build
### Linux
This project requires the following software to compile:
- CMake - C++ build management
- a C++ toolchain

And the following C++ libraries:
- glm - math library for graphics programming
- SDL3 - library for interfacing hardware

Then run the following command:

    mkdir build; cd build; cmake ..; cmake --build .

This will create an executable within the build directory.
