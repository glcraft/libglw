#include <memory>
#include "windowHelper.h"
#include <libglw/GLWrapper.h>
#include <libglw/Shaders.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/color_space.hpp>
struct Vertex
{
    glm::vec2 pos;
    glm::vec4 color;
};
using MyBuffer = gl::ArrayBuffer<Vertex>;
std::unique_ptr<MyBuffer> genVBO()
{
    using namespace glm;
    std::unique_ptr<MyBuffer> vbo = std::make_unique<MyBuffer>();
    
    vbo->reserve(3); // 3 vertices
    vbo->map(GL_WRITE_ONLY);
    Vertex* data = vbo->data();
    for(int i=0;i<3;i++)
    {
        float percent = static_cast<float>(i)/3.f;
        float angle = percent*pi<float>()*2.f;
        data[i].pos = vec2(cos(angle), sin(angle));
        data[i].color = vec4(rgbColor(vec3(percent*360.f, 1.f, 1.f)), 1.f);
    }
    vbo->unmap();
    auto vao = std::make_shared<gl::VertexArray>();
    vbo->attachVertexArray(vao);
    vbo->set_attrib(MyBuffer::Attrib<0>(offsetof(Vertex, pos), 2, GL_FLOAT));
    vbo->set_attrib(MyBuffer::Attrib<1>(offsetof(Vertex, color), 4, GL_FLOAT));
    return vbo;
}
gl::sl::Program genShaders() 
{
    const char* vert = R"s(
#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec4 color;

uniform mat4 modelmat;

out vec4 inColor;
void main()
{
    gl_Position = modelmat * vec4(pos, 0., 1.);
    inColor = color;
}
    )s";
    const char* frag = R"s(
#version 330 core
in vec4 inColor;
out vec4 outColor;
void main()
{
    outColor=inColor;
}
    )s";
    gl::sl::Program prog;
    TRY_GLSL
    prog << gl::sl::Shader<gl::sl::Vertex>(vert, false)<< gl::sl::Shader<gl::sl::Fragment>(frag, false) << gl::sl::link;
    CATCH_GLSL
    return prog;
}
int main(int argv, char** argc)
{
    //Init window
    std::unique_ptr<Window> winMain = std::make_unique<Window>();
    //Init glew
    glewExperimental = true;
    glewInit();
    //Init vars
    auto buffer = genVBO();
    auto program = genShaders();
    glm::mat4 modelmat(1.f);
    //loop
    while (!winMain->isClosed())
    {
        modelmat = glm::rotate(modelmat,glm::radians(0.1f), glm::vec3(0,0,1));
        winMain->updateEvents();
        gl::ClearColor(glm::vec3(0.1f), 1.f);
        program << gl::sl::use
            << gl::UniformRef<glm::mat4>("modelmat", modelmat);
        buffer->draw(GL_TRIANGLES);
        winMain->swapBuffers();
    }
    winMain.reset();
    return 0;
}