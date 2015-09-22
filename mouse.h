/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#pragma once
#ifndef Mouse_h__
#define Mouse_h__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <chrono>

struct Key
{
    bool pressed = false;
    bool last_pressed = false;
};

struct ClickHandler
{
    ClickHandler(glm::vec2 p, int b) : pos(p), button(b){}
    glm::vec2 pos = glm::vec2(0);
    int button = GLFW_MOUSE_BUTTON_LEFT;
    bool repeat = false;
};

class Mouse
{
private:
    static GLFWwindow *sm_window;

    static double sm_xpos;
    static double sm_ypos;

    static double sm_lastxpos;
    static double sm_lastypos;

    static double offset;
    static double last_offset;

    static bool sm_stateFixedMousePos;
    static Key sm_buttons[10];

public:

    enum STATE
    {
        STATE_MOUSE,
        STATE_RESIZE
    };

    static STATE state;

    static void SetButton(int a, int b, int c);
    static void SetCursorPos(double xpos, double ypos);

    static void initialize( GLFWwindow *win );

    static void SetFixedPosState( bool state);

    static bool GetFixedPosState()
    {
        return sm_stateFixedMousePos;
    }

    static void getCursorPos(double &xpos, double &ypos);
    static glm::vec2 getCursorPos();
    static glm::vec2 getCursorLastPos();
    static glm::vec2 getCursorDelta();

    static void Scroll(double a);

    static bool isMiddleJustPressed();
    static bool isRightJustPressed();
    static bool isLeftJustPressed();

    static bool isMiddleJustReleased();
    static bool isLeftJustReleased();
    static bool isRightJustReleased();

    static bool isMiddleUp();
    static bool isRightUp();
    static bool isLeftUp();

    static bool isMiddleDown();
    static bool isLeftDown();
    static bool isRightDown();

    static bool isWheelDown();
    static bool isWheelUp();

    static void dropState();
};


#endif // Mouse_h__
