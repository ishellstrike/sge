#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES
#define GLM_SWIZZLE
#include "gamewindow.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main()
#endif
{
    GameWindow gw;
    gw.Mainloop();
    return 0;
}
