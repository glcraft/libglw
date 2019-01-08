#include <SDL2/SDL.h>
class Window
{
public:
    Window();
    ~Window();
    void updateEvents();
    void swapBuffers();

    bool isClosed();
private:
    SDL_Window *m_window = 0;
    SDL_GLContext glcont;
    bool m_close = false;
};