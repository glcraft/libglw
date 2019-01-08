# libglw - OpenGL C++11 wrapper

libglw is a C++11 wrapper for OpenGL and OpenGL Shader Language. It provides classes for most OpenGL objects and some function wrapped in a C++11 style.

- [libglw - OpenGL C++ wrapper](#libglw---opengl-c-wrapper)
  - [Features](#features)
    - [OpenGL Buffers](#opengl-buffers)
    - [GLSL Shaders](#glsl-shaders)
    - [Uniforms GLM compatible](#uniforms-glm-compatible)
    - [Textures](#textures)
    - [Renderbuffers and Framebuffers](#renderbuffers-and-framebuffers)
  - [Dependencies](#dependencies)
  - [Demo](#demo)
  - [Release notes](#release-notes)
  - [TODO](#todo)

## Features

### OpenGL Buffers

libglw provides classes for VBO (`gl::ArrayBuffer<Struct>`) and EBO (`gl::ElementBuffer<Integer>`). Those two classes work like vectors : you reserve space (internally allocate more space than `size` in `capacity`), you map then unmap it.
```cpp
struct Vertex
{
    glm::vec2 pos;
    glm::vec4 color;
};
gl::ArrayBuffer<Vertex> buffer;
buffer.reserve(6); // 6 vertices
buffer.reserve_relative(3); // add 3 vertices, these now 9 vertices
Vertex* v = buffer.map();
// change v map
buffer.unmap();
buffer.draw(GL_TRIANGLES);
```
You can easily attach a VAO (`gl::VertexArray`) and add attributs (`gl::ArrayBuffer<Struct>::Attrib<index>`) to the ArrayBuffer.

### GLSL Shaders

GLSL Shader loading are splitted in two classes `gl::sl::Shader<ShaderType>` and `gl::sl::Program`. `Shader` uses to load and compile the shader, `Program` links attached shaders and become the shaders owner to *use*.

Any errors from compile and link throw exceptions you can catch. `TRY_GLSL` and `CATCH_GLSL` are provided to capture gl::sl exceptions and displays errors to error console (`std::cerr`).

You can use stream operator to aply a shader : 

```cpp
gl::sl::Program prog;
TRY_GLSL
prog << gl::sl::Shader<gl::sl::Vertex>(vert, false) << gl::sl::Shader<gl::sl::Fragment>(frag, false) << gl::sl::link;
CATCH_GLSL
prog.use();
```

### Uniforms GLM compatible

Uniforms are packed into two classes to transmit to gl::sl::Program. You can set referenced value (`gl::UniformRef<ValueType>`) or static and r-value (`gl::UniformStatic<ValueType>`).
```cpp
program << gl::sl::use
        << gl::UniformRef<glm::mat4>("modelmat", modelmat);
        << gl::UniformStatic<float>("opacity", 0.5f);
```

### Textures, Renderbuffers and Framebuffers

`Texture`, `Renderbuffer` and `Framebuffer` provide several functionalities following OpenGL ones. You can set format, sampler and size of a texture for example, and attach `Texture` and `Renderbuffer` to `Framebuffer`. Check out GLClass.h for more informations.

## Dependencies

libglw needs some libraries to work.
* OpenGL 3.3
* [GLEW](http://glew.sourceforge.net) : OpenGL Extension, for OGL 3.3 compatibility
* [GLM - OpenGL Mathematics](https://github.com/g-truc/glm)

## glwapp - Demo

A demo is available to test the library. Using SDL2 for Window and events handling, it displays a turning RGB colored triangle.

## Release notes

<h3>Version 0.0.1</h3>

*It all starts one day...*

* Add buffers, shader loading, texture, framebuffers and renderbuffers
* Add functional: Clear and Enable.
* Add demo
* Cmake support

## TODO
- [ ] Linux portage (very soon)
- [ ] Doxygen documentation
- [ ] OpenGL versions compatibilities