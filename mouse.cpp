/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "mouse.h"
#include <glm/glm.hpp>
#include "prefecences.h"

GLFWwindow *Mouse::sm_window = nullptr;

double Mouse::sm_xpos = 0;
double Mouse::sm_ypos = 0;

bool Mouse::sm_stateFixedMousePos = false;

double Mouse::offset = 0;
double Mouse::last_offset = 0;

void Mouse::initialize(GLFWwindow *win )
{
    sm_window = win;
    sm_xpos = 1.0;
    sm_ypos = 1.0;
    sm_stateFixedMousePos = false;

    glfwSetCursorPos(sm_window, sm_xpos, sm_ypos);
}


void Mouse::SetButton( int button, int state , int )
{
    Mouse::sm_buttons[button].pressed = state == GLFW_PRESS;
}

void Mouse::Scroll(double a){
    offset += a;
}

void Mouse::SetCursorPos( double x, double y )
{
    sm_xpos = x;
    sm_ypos = y;
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
}

//  reset mouse delta
void Mouse::dropState(){
    if(sm_stateFixedMousePos)
    {
        sm_xpos = double(RESX) / 2.0;
        sm_ypos = double(RESY) / 2.0;
        glfwSetCursorPos(sm_window, sm_xpos, sm_ypos);
    }

    last_offset = offset;
    sm_lastxpos = sm_xpos;
    sm_lastypos = sm_ypos;

    for(auto t : {GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_MIDDLE})
    {
        sm_buttons[t].last_pressed = sm_buttons[t].pressed;
        //sm_buttons[t].clicks = 0;
    }
}

bool Mouse::isWheelUp()
{
    return offset > last_offset;
}

bool Mouse::isWheelDown()
{
    return offset < last_offset;
}

bool Mouse::isMiddleJustPressed()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_MIDDLE].pressed && !sm_buttons[GLFW_MOUSE_BUTTON_MIDDLE].last_pressed;
}

bool Mouse::isRightJustPressed()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_RIGHT].pressed && !sm_buttons[GLFW_MOUSE_BUTTON_RIGHT].last_pressed;
}

bool Mouse::isLeftJustPressed()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_LEFT].pressed && !sm_buttons[GLFW_MOUSE_BUTTON_LEFT].last_pressed;
}

bool Mouse::isMiddleJustReleased()
{
    return !sm_buttons[GLFW_MOUSE_BUTTON_MIDDLE].pressed && sm_buttons[GLFW_MOUSE_BUTTON_MIDDLE].last_pressed;
}

bool Mouse::isRightJustReleased()
{
    return !sm_buttons[GLFW_MOUSE_BUTTON_RIGHT].pressed && sm_buttons[GLFW_MOUSE_BUTTON_RIGHT].last_pressed;
}

bool Mouse::isLeftJustReleased()
{
    return !sm_buttons[GLFW_MOUSE_BUTTON_LEFT].pressed && sm_buttons[GLFW_MOUSE_BUTTON_LEFT].last_pressed;
}




bool Mouse::isMiddleDown()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_MIDDLE].pressed;
}

bool Mouse::isRightDown()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_RIGHT].pressed;
}

bool Mouse::isLeftDown()
{
    return sm_buttons[GLFW_MOUSE_BUTTON_LEFT].pressed;
}

bool Mouse::isMiddleUp()
{
    return !sm_buttons[GLFW_MOUSE_BUTTON_MIDDLE].pressed;
}

bool Mouse::isRightUp()
{
    return !sm_buttons[GLFW_MOUSE_BUTTON_RIGHT].pressed;
}

bool Mouse::isLeftUp()
{
    return !sm_buttons[GLFW_MOUSE_BUTTON_LEFT].pressed;
}

glm::vec2 Mouse::getCursorDelta()
{
    return glm::vec2(sm_xpos - sm_lastxpos, sm_ypos - sm_lastypos );
}

glm::vec2 Mouse::getCursorLastPos()
{
    return glm::vec2(sm_lastxpos, sm_lastypos);
}

double Mouse::sm_lastypos;
double Mouse::sm_lastxpos;
Mouse::STATE Mouse::state = Mouse::STATE_MOUSE;
Key Mouse::sm_buttons[10];

