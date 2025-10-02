# AppToolkit

A simple library to develop graphical applications. 

This library creates a window context and handles user input with GLFW. Also, it has support for developing UI with ImGui. 

## Handling dependencies

This toolkit has the following dependencies:
- GLFW;
- glew;
- glm;
- Imgui;

You can create a installation script like this:

```
git clone https://github.com/Microsoft/vcpkg.git

cd vcpkg

call bootstrap-vcpkg.bat

vcpkg --triplet x64-windows install glfw3
vcpkg --triplet x64-windows install glew
vcpkg --triplet x64-windows install glm

vcpkg install imgui[core,glfw-binding,opengl3-binding]:x64-windows

vcpkg install glfw3
vcpkg install glew
vcpkg install glm

vcpkg install imgui[core,glfw-binding,opengl3-binding]
```

## Building an application with this toolkit

With the dependencies installed with vcpkg, you can create a build script like the following:

```
SET VCPKG_TOOLCHAIN=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake

mkdir BUILD
cd BUILD

cmake ../Src -DCMAKE_TOOLCHAIN_FILE=%VCPKG_TOOLCHAIN%
cmake --build . 

pause
```

## Examples

In the following repository there are a few simple examples using AppToolkit: https://github.com/Kape94/AppToolkitExamples

Also, in this example repository it is possible to see how to setup a project using AppToolkit as a submodule. 

## Toolkit structure

This lib permits the user code to extend the application using 4 main functionalities:
- Initialization;
- UI presentation;
- Update;
- Rendering;

The initialization phase is executed only one time, before the app starts running. The other 3 functionalities are executed every frame.

For extending those 4 functionalities, AppToolkit offers 4 different interfaces: IAppInitializer (Initialization), IuiPresenter (UI presentation), IAppUpdater (Update) and IAppRenderer (Rendering). The user code can implement some of these interfaces or all of them, according the application needs.

The motivation for modeling the extension of these functionalities with interfaces are mainly because this way the user code can implement functors, that can carry state, which is a capability that simple function pointers don't permit. Also, other motivation for this is to give the user the freedom to choose the class modeling that better suit her/his needs (ex: can be just one class that implements all the 4 functionalities, or 4 different classes that communicates between each other).

