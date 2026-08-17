# Chimia 

A C++ framework for 3D graphics applications. This is a young framework still, so there is quite a bit of work in progress.

This framework is composed by multiple supporting modules, with different specialized purposes. The intent with this design is so the consumer applications can combine, for example, the graphics module of this framework with their own windowing implementation.

The currently available modules in the framework are:
- Core: Define base types and functionalities. This module is depended by all the other modules in the framework;
- Media: Defines types and functionalities for importing assets, like images and models;
- Rendering: A module intended for abstracting the graphics API. Currently it's built on top of OpenGL only, however the module's API is mostly stateless, so it's a bit prepared to wrap other APIs like Vulkan, Metal and D3D in the close future;
- Draw3D: A slightly higher level module for graphics, made on top of the Rendering module. This module is designed to support both immediate and retained mode approaches;
- App: Provides windowing, UI (with ImGUI) and input functionality. For graphics you can use the Rendering or Draw3D modules, or implement your own as well;

There are more modules planned to be implemented in the close future:
- Sound;
- Collision;
- Physics;
- Game/Application engine;

Also, there's a extensive list of new features improvements I'm working actively. Let me know if you have any feedbacks.

## Build Instructions

A quick background before: this project relies in CMake as build system and vcpkg for third party package management, aside from that you'll only need C++ development tools installed on your environment.

Personally, I use a tool called "CppM" for building this project, so I'll provide intructions on how to get started using it.

First, I assume you have these tools/dependencies installed on your environment:
- C++ development tooling (e.g Visual studio on Windows, XCode on MacOS, compiler and build toolchain on Linux);
- OpenGL drivers: usually on Windows and MacOS you don't need to explicitly install this, on Linux however you might need to install mesa drivers;
- git;
- python;

If you use CppM, you will not need to install cmake, since this tool borrows it from vcpkg.

Having this at hand, we can just install CppM:

```bash
python -m pip install cppm
```

Clone this repo and go to its root dir, and then:

```bash
cppm init
cppm build
```

or:

```bash
cppm init
cppm build --cmake_args -DBUILD_EXTRAS=ON
```

to build the extras (samples and tests)

