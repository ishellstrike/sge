/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include "colorextender.h"
#include "font.h"
#include "fpscounter.h"
#include "FrameBuffer.h"
#include "gametimer.h"
#include "gbuffer.h"
#include "glm/gtx/transform.hpp"
#include "keyboard.h"
#include "logger.h"
#include "mouse.h"
#include "prefecences.h"
#include "resources/resourcecontroller.h"
#include "sge_perfomance.h"
#include "sge_texlab_toolbox.h"
#include "sge_ui/wins.h"
#include "shader.h"
#include "shader.h"
#include "spritebatch.h"
#include "spritebatch.h"
#include "textureatlas.h"
#include "TextureGenerator.h"

#include <algorithm>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <memory>
#include <string>
#include <thread>
#include "core/level.h"


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

    void make_debug()
    {
        update = [=](){BaseUpdate<true>();};
        draw = [=](){BaseDraw<true>();};
        main_is_debug = true;
        glfwWindowHint(GLFW_SAMPLES, 1);
    }

    void make_release();

    void make_spartial();

    bool BaseInit();

    std::function<void()> draw;
    std::function<void()> update;

    template<int is_debug>
    void BaseUpdate();

    template<int is_debug>
    void BaseDraw();

    GameTimer gt;
    FPSCounter fps;

    static void Resize(int w, int h);

    void SetTitle(const std::string &str);

    GLFWmonitor *monitor = nullptr;
    GLFWwindow *window = nullptr;
    std::shared_ptr<GBuffer> gb;

    glm::mat4 view = glm::mat4(1), proj = glm::mat4(1), proj_per = glm::mat4(1), model = glm::mat4(1);
    static GameWindow *wi;

    GameWindow();
    ~GameWindow();

    std::shared_ptr<SpriteBatch> batch;
    float speed = 1;

    bool wire = false, no_ui = false;
    glm::vec2 offset;

    std::shared_ptr<FrameBuffer> fbo_blur, fbo_blur2, fbo_extract, fbo_main;
    std::shared_ptr<Texture> texture_blur, texture_blur2, texture_extract, texture_main;
    std::shared_ptr<Texture> texture_pipeline;

    std::shared_ptr<Font> f12;
    std::shared_ptr<WinS> ws;
    sge_perfomance *perf;
    Level level;

    static void Swap();
    void BlitGBuffer();
    void ShadingPass();
    void GeometryPass();
};

#endif // GAMEWINDOW_H
