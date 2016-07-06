/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>

#include "fpscounter.h"
#include "gametimer.h"
#include "textureatlas.h"
#include "camera.h"
#include "gbuffer.h"
#include "experimental/quadsphere.h"
#include "experimental/scattering.h"
#include "space/spacesystem.h"
#include "space/solver.h"
#include "experimental/starfield.h"
#include "FrameBuffer.h"

#include "glm/gtx/transform.hpp"
#include <thread>

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

    SpaceSystem ss;

    std::shared_ptr<Texture> tex1;
    float speed = 1;

    bool wire = false, no_ui = false;
    std::shared_ptr<Camera> cam1, cam2;
    Camera *cam;
    glm::vec3 moving;
    std::vector<glm::vec3> tail;
    std::unique_ptr<Starfield> sf;

    UMesh<VertPosNormUvUv> bill;

    std::shared_ptr<FrameBuffer> fbo_blur, fbo_blur2, fbo_extract, fbo_main;
    std::shared_ptr<Texture> texture_blur, texture_blur2, texture_extract, texture_main;
    std::shared_ptr<Texture> texture_pipeline;

    std::shared_ptr<QuadSphere> qs;
    static void Swap();
    void GeometryPass();
    void BlitGBuffer();
    void ShadingPass();
    void BeginLightPasses();
    void AftereffectPass();
    void PreloadHdr();
    void DropHdr();
};

#endif // GAMEWINDOW_H
