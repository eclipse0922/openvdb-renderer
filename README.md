# OpenVDB renderer

A minimalist OpenVDB renderer using OpenGL and ImGui.

![screenshot cube](img/vdb_renderer_armadillo.png)

(Armadillo VDB model downloaded at : https://www.openvdb.org/download/)

<img src="img/vdb_renderer_capture.gif" alt="vdb_renderer_capture.gif" style="zoom:150%;" />

## Description

This project was made at ArtFX - TD 4.

## Getting Started

### Dependencies

The necessary libraries are handled by vcpkg and cmake

all you need to do is

```shell
mkdir build
cd build
cmake ..
```

https://academysoftwarefoundation.github.io/openvdb/build.html#buildUsingOpenVDB

### Executing the program

From the build folder, you can launch the program : 

```shell
$ ./openvdb-renderer
```

## Sources
- https://github.com/morizotter/imgui-opengl-glfw-glew-cmake-demo
- https://retifrav.github.io/blog/2019/05/26/sdl-imgui/
- https://academysoftwarefoundation.github.io/openvdb/codeExamples.html
- https://cmake.org/cmake/help/latest/guide/tutorial/index.html
