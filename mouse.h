#pragma once
#ifndef Mouse_h__
#define Mouse_h__

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Mouse
{
private:
    static GLFWwindow *sm_window;

    static double sm_xpos;
    static double sm_ypos;

    static double sm_dxpos;
    static double sm_dypos;

    static double sm_lastxpos;
    static double sm_lastypos;

    static double sm_deltaxpos;
    static double sm_deltaypos;

    static unsigned int sm_windowWidth;
    static unsigned int sm_windowHeight;

    static double offset;
    static double last_offset;

    static bool sm_stateFixedMousePos;
    static bool sm_isCursorClientArea;
    static bool sm_isWindowFocused;
    static int sm_buttons[10];

public:

    enum STATE
    {
        STATE_MOUSE,
        STATE_RESIZE
    };

    static STATE state;

    static void cursorClientArea(int entered);
    static void SetButton(int a, int b, int c);
    static void SetCursorPos(double xpos, double ypos);
    static void windowFocus(int focused);

    static void initialize( GLFWwindow *win );

    static void SetFixedPosState( bool state);

    static bool GetFixedPosState()
    {
        return sm_stateFixedMousePos;
    }

    static void setWindowSize(unsigned int width, unsigned int height);

    static void getCursorPos(double &xpos, double &ypos);
    static glm::vec2 getCursorPos();
    static glm::vec2 getCursorLastPos();
    static glm::vec2 getCursorDelta();

    static double isMoveCursorX();
    static double isMoveCursorY();

    static void Scroll(double a);

    static bool isRightPressed();
    static bool isLeftPressed();
    static void dropState();
    static bool isRightDown();
    static bool isLeftDown();
    static bool isWheelDown();
    static bool isWheelUp();
    static bool isMiddleDown();
    static bool isRightUp();
    static bool isLeftUp();
};


#endif // Mouse_h__
