#include "mouse.h"
#include <glm/glm.hpp>
#include "prefecences.h"

GLFWwindow *Mouse::sm_window = nullptr;

double Mouse::sm_xpos = 0;
double Mouse::sm_ypos = 0;

double Mouse::sm_dxpos = 0;
double Mouse::sm_dypos = 0;

unsigned int Mouse::sm_windowWidth = 0;
unsigned int Mouse::sm_windowHeight = 0;

bool Mouse::sm_stateFixedMousePos = false;
bool Mouse::sm_isCursorClientArea = false;
bool Mouse::sm_isWindowFocused = false;

double Mouse::offset = 0;
double Mouse::last_offset = 0;


void Mouse::initialize(GLFWwindow *win )
{
    sm_window = win;
    sm_xpos = 1.0;
    sm_ypos = 1.0;
    sm_dxpos = 0.0;
    sm_dypos = 0.0;
    sm_windowWidth = 0;
    sm_windowHeight = 0;
    sm_stateFixedMousePos = false;
    sm_isCursorClientArea = true;
    sm_isWindowFocused = true;

    glfwSetCursorPos(sm_window, sm_xpos, sm_ypos);
}


void Mouse::SetButton( int button, int state , int )
{
    Mouse::sm_buttons[button] = state;
}

void Mouse::Scroll(double a){
    offset += a;
}

void Mouse::SetCursorPos( double x, double y )
{
    if(!sm_isWindowFocused)
        return;

    sm_dxpos = x - sm_xpos;
    sm_dypos = y - sm_ypos;
    sm_deltaxpos = sm_dxpos;
    sm_deltaypos = sm_dypos;
    sm_lastxpos = sm_xpos;
    sm_lastypos = sm_ypos;

    if(!sm_isCursorClientArea)
    {
        sm_dxpos = 0.0;
        sm_dypos = 0.0;
    }

    if(sm_stateFixedMousePos)
    {
        sm_xpos = double(RESX) / 2.0;
        sm_ypos = double(RESY) / 2.0;
        glfwSetCursorPos(sm_window, sm_xpos, sm_ypos);
    }
    else
    {
        sm_xpos = x;
        sm_ypos = y;
    }
}

double Mouse::isMoveCursorX()
{
    double dx = sm_dxpos;
    sm_dxpos = 0.0;
    return dx;
}

double Mouse::isMoveCursorY()
{
    double dy = sm_dypos;
    sm_dypos = 0.0;
    return dy;
}

void Mouse::getCursorPos( double &x, double &y )
{
    x = sm_xpos;
    y = sm_ypos;
}

glm::vec2 Mouse::getCursorPos()
{
    return glm::vec2(sm_xpos, sm_ypos);
}

void Mouse::SetFixedPosState( bool state )
{
    sm_stateFixedMousePos = state;
    if(sm_stateFixedMousePos)
    {
        sm_xpos = double(RESX) / 2.0;
        sm_ypos = double(RESY) / 2.0;
        sm_dxpos = 0.0;
        sm_dypos = 0.0;
        glfwSetCursorPos(sm_window, sm_xpos, sm_ypos);
    }
}

void Mouse::setWindowSize( unsigned int width, unsigned int height )
{
    sm_windowWidth = width;
    sm_windowHeight = height;
}

void Mouse::cursorClientArea( int entered )
{
    if(entered == GL_TRUE)
    {
        sm_isCursorClientArea = true;
        glfwGetCursorPos(sm_window, &sm_xpos, &sm_ypos);
    }
    else
    {
        sm_isCursorClientArea = false;
    }
}

//  reset mouse delta
void Mouse::dropState(){
    sm_deltaypos = 0;
    sm_deltaxpos = 0;
    last_offset = offset;

    for(auto t : {GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_MIDDLE})
    {
        if(sm_buttons[t] == GLFW_PRESS)
            sm_buttons[t] = GLFW_REPEAT;

        //if(sm_buttons[t] == GLFW_RELEASE)
        //    sm_buttons[t] = 0;
    }
}

bool Mouse::isWheelUp()
{
    return offset > last_offset;
}

bool Mouse::isMiddleDown()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_MIDDLE] == GLFW_PRESS || sm_buttons[GLFW_MOUSE_BUTTON_MIDDLE] == GLFW_REPEAT;
}

bool Mouse::isWheelDown()
{
    return offset < last_offset;
}

void Mouse::windowFocus( int focused )
{
    if(focused == GL_TRUE)
    {
        sm_isWindowFocused = true;
        glfwGetCursorPos(sm_window, &sm_xpos, &sm_ypos);
    }
    else
    {
        sm_isWindowFocused = false;
    }
}

bool Mouse::isRightPressed()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS;
}

bool Mouse::isLeftDown()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS || sm_buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_REPEAT;
}

bool Mouse::isRightDown()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS || sm_buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_REPEAT;
}

bool Mouse::isLeftUp()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_RELEASE;
}

bool Mouse::isRightUp()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_RELEASE;
}

bool Mouse::isLeftPressed()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS;
}

glm::vec2 Mouse::getCursorDelta()
{
    return glm::vec2(sm_deltaxpos, sm_deltaypos);
}

glm::vec2 Mouse::getCursorLastPos()
{
    return glm::vec2(sm_lastxpos, sm_lastypos);
}

double Mouse::sm_lastypos;
double Mouse::sm_lastxpos;
double Mouse::sm_deltaypos;
double Mouse::sm_deltaxpos;
Mouse::STATE Mouse::state = Mouse::STATE_MOUSE;
int Mouse::sm_buttons[10];

