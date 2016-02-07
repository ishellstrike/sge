/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <GL/glew.h>
#include <algorithm>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <memory>
#include <string>
#include <thread>

#include "glm/gtx/transform.hpp"
#include "keyboard.h"
#include "prefecences.h"

#include "sge_perfomance.h"
#include "sge_level_debug_info.h"
#include "sge_settings_main.h"
#include "sge_inventory.h"
#include "sge_chest_window.h"
#include "sge_crafting_window.h"
#include "sge_eventbus_log.h"

#define MAJOR 2
#define MINOR 1
//#define NO_SCATT
#define NO_STARFIELD


class GameWindow {
public:
    enum {
        spartial_rendering = 2
    };

    int main_is_debug = false;

    void Mainloop();

    bool BaseInit();

    void BaseUpdate();

    void BaseDraw();

    GameTimer gt;
    FPSCounter fps;

    static void Resize(int w, int h);

    void SetTitle(const std::string &str);

    GLFWmonitor *monitor = nullptr;
    GLFWwindow *window = nullptr;

    glm::mat4 view = glm::mat4(1), proj = glm::mat4(1), proj_per = glm::mat4(1), model = glm::mat4(1);
    static GameWindow *wi;

    GameWindow();
    ~GameWindow();

    std::shared_ptr<SpriteBatch> batch;
    float speed = 1;

    bool no_ui = false;
    glm::vec2 offset;

    std::shared_ptr<Font> f12;
    std::shared_ptr<WinS> ws;
    sge_perfomance *perf;
    sge_level_debug_info *linfo;
    sge_settings_main *settings;
    sge_inventory *inventory;
    sge_chest_window *chest;
    sge_crafting_window *craft;
    sge_eventbus_log *eventbus;


    Level level;
    std::shared_ptr<Object> hero;
    static std::shared_ptr<Object> Hero;
    float update_pass = 0;
};

#endif // GAMEWINDOW_H
