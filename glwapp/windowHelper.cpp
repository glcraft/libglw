#include "windowHelper.h"

Window::Window()
{
    SDL_Init(SDL_INIT_VIDEO);
    // Version d'OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Double Buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	
    m_window = SDL_CreateWindow("libglw : C++ OpenGL Wrapper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);

    glcont = SDL_GL_CreateContext(m_window);
}
Window::~Window()
{
    if (glcont)
        SDL_GL_DeleteContext(glcont);
    if (m_window)
        SDL_DestroyWindow(m_window);
    
    SDL_Quit();
}
void Window::updateEvents()
{
    SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
        case SDL_KEYDOWN:
            m_close = m_close || evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE;
            break;
        case SDL_WINDOWEVENT:
            switch (evt.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                m_close = true;
                break;
            }
            break;
        }
    }
}
void Window::swapBuffers()
{
    SDL_GL_SwapWindow(m_window);
}

bool Window::isClosed()
{
    return m_close;
}