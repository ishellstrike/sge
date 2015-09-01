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
#include <string>
#include "logger.h"
#include "shader.h"
#include "spritebatch.h"
#include "fpscounter.h"
#include "gametimer.h"
#include "textureatlas.h"
#include "font.h"
#include "camera.h"
#include "gbuffer.h"
#include "sge_ui/wins.h"
#include "sge_perfomance.h"
#include "experimental/quadsphere.h"
#include "experimental/scattering.h"
#include "space/spacesystem.h"
#include "space/space_solver.h"
#include "experimental/starfield.h"
#include "FrameBuffer.h"
#include "geometry/ubillboard.h"

#include "mouse.h"
#include "keyboard.h"
#include "prefecences.h"
#include "shader.h"
#include "spritebatch.h"
#include "glm/gtx/transform.hpp"
#include "colorextender.h"
#include "textureatlas.h"
#include <thread>
#include <chrono>
#include <algorithm>
#include "ClassicNoise.h"
#include "geometry/model.h"
#include "resources/resourcecontroller.h"
#include "TextureGenerator.h"
#include "resources/random_noise.h"
#include "space/space_object.h"
#include "space/spacesystem.h"
#include <glm/gtx/compatibility.hpp>
#include "sge_texlab_toolbox.h"

#define MAJOR 2
#define MINOR 1
#define NO_SCATT
#define NO_STARFIELD

class GameWindow {
public:
    enum {
        spartial_rendering = 2
    };

    int main_is_debug = false;

    void Mainloop()
    {
        BaseInit();
        while(!glfwWindowShouldClose(window))
        {
            update();

            if(main_is_debug == spartial_rendering)
            {
                static int ii = 0, jj = 0;
                ii++;
                if(ii >= 10) { ii = 0; jj++;}
                if(jj >= 10) {jj = 0; }
                glScissor(ii*RESX/10,jj*RESY/10,RESX/10,RESY/10);
                glEnable(GL_SCISSOR_TEST);
            }

            draw();

            Swap();

            gt.Update(static_cast<float>(glfwGetTime()));
            fps.Update(gt);
            perf->UpdateTimer(fps, gt);
            // std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        Resources::drop();
    }

    void make_debug()
    {
        update = [=](){BaseUpdate<true>();};
        draw = [=](){BaseDraw<true>();};
        main_is_debug = true;
        glfwWindowHint(GLFW_SAMPLES, 1);
    }

    void make_release()
    {
        update = [=](){BaseUpdate<false>();};
        draw = [=](){BaseDraw<false>();};
        main_is_debug = false;
        glfwWindowHint(GLFW_SAMPLES, 1);
    }

    void make_spartial()
    {
        update = [=](){BaseUpdate<spartial_rendering>();};
        draw = [=](){BaseDraw<spartial_rendering>();};
        main_is_debug = spartial_rendering;
        glfwWindowHint(GLFW_SAMPLES, 16);
    }

    bool BaseInit()
    {
        update = [=](){BaseUpdate<false>();};
        draw = [=](){BaseDraw<false>();};

        srand(123);
        LOG(info) << "Jarg initialization start";
        LOG(info) << "User-preferred locale setting is " << std::locale("").name().c_str();
        glfwSetErrorCallback([](int /*a*/,const char* description){LOG(error) << description;});
        int glfwErrorCode = glfwInit();
        if (!glfwErrorCode)
        {
            LOG(error) << "glfwInit error " << glfwErrorCode;
            return false;
        }
        glfwWindowHint(GLFW_SAMPLES, 16);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

        monitor = nullptr;

        int rx = Prefecences::Instance()->resolution.x;
        int ry = Prefecences::Instance()->resolution.y;
        window = glfwCreateWindow(rx, ry, string_format("sge %dx%d", rx, ry).c_str(), monitor, nullptr);
        if (!window)
        {
            glfwTerminate();
            LOG(fatal) << "GLFW window fatal error. Minimum feature level " << MAJOR << "." << MINOR;
            return false;
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        int err = glewInit();
        if (err != GLEW_OK)
        {
            LOG(fatal) << "GLEW init fatal etrror";
            LOG(fatal) << "GLEW Error : " << std::string((const char*)glewGetErrorString(err));
            return false;
        }


        int glVersion[2] = {-1, -1};
        int ntex, nuni;
        glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
        glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ntex);
        glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &nuni);
        LOG(info) << "Renderer: " << glGetString(GL_RENDERER);
        LOG(info) << "Vendor: " << glGetString(GL_VENDOR);
        LOG(info) << "Version: " << glGetString(GL_VERSION);
        LOG(info) << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
        LOG(info) << "using OpenGL: " << glVersion[0] << "." << glVersion[1];
        LOG(info) << "GLFW: " << glfwGetVersionString();
        LOG(info) << "GLEW: " << glewGetString(GLEW_VERSION);
        LOG(info) << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: " << ntex;
        LOG(info) << "GL_MAX_UNIFORM_LOCATIONS: " << nuni;
        if(ntex < 16)
            throw;
        LOG(info) << EXT_CHECK(GLEW_ARB_multi_bind);
        LOG(info) << EXT_CHECK(GLEW_ARB_tessellation_shader);
        LOG(info) << EXT_CHECK(GLEW_ARB_texture_float);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Keyboard::Initialize();
        glfwSetKeyCallback(window, [](GLFWwindow *, int key, int scancode, int action, int mods){
            Keyboard::SetKey(key, scancode, action, mods);
        });
        Mouse::initialize(window);
        //Mouse::SetFixedPosState(true);
        glfwSetCursorPosCallback(window, [](GLFWwindow *, double xpos, double ypos){
            Mouse::SetCursorPos(xpos, ypos);
        });
        glfwSetMouseButtonCallback(window, [](GLFWwindow *, int a, int b, int c){
            Mouse::SetButton(a, b, c);
        });
        glfwSetWindowSizeCallback(window, [](GLFWwindow *, int a, int b){
            GameWindow::Resize(a, b);
        });
        glfwSetScrollCallback(window, [](GLFWwindow *, double , double b){
            Mouse::Scroll(b);
        });

        Resources::instance();
        Resources::instance()->Init();

        gb = std::make_shared<GBuffer>();
        gb->Init(RESX, RESY);

        batch = std::make_shared<SpriteBatch>();

        f12 = std::make_shared<Font>();
        f12->initFreeType(12);
        f12->renderAtlas();

        ws = std::make_shared<WinS>(batch.get());
        ws->f = f12.get();

        perf = new sge_perfomance(ws.get());
        new sge_texlab_toolbox(ws.get());

        cam1 = std::make_shared<Camera>();
        cam2 = std::make_shared<Camera>();
        cam = cam1.get();

        cam1->Position({3,3,3});
        cam1->LookAt({0,0,0});

        cam2->Position({3,3,3});
        cam2->LookAt({0,0,0});
        Resize(RESX, RESY);

        std::shared_ptr<Material> mat = std::make_shared<Material>();
        mat->texture = Resources::instance()->Get<Texture>("grass");
        mat->low = Resources::instance()->Get<Texture>("grass");
        mat->medium = Resources::instance()->Get<Texture>("soil");
        mat->high = Resources::instance()->Get<Texture>("snow");
        mat->side = Resources::instance()->Get<Texture>("rock");

        if(Prefecences::Instance()->hdr_on)
            PreloadHdr();

        std::shared_ptr<Material> mat_star = std::make_shared<Material>();
        mat_star->emission = Color::White;

        auto &wm = std::make_shared<Material>();

        qs = std::make_shared<QuadSphere>(mat);
        qs->max_divide = 4;
        qs_w = std::make_shared<QuadSphere>(wm);
        qs_w->max_divide = 4;
        qs_w->s = 1;
        qs_w->R = 1010;
        wm->diffuse = Color::SeaBlue;
        wm->shininess = 80;

#ifndef NO_SCATT
        scat.Precompute();
#endif

#ifndef NO_STARFIELD
        sf = std::unique_ptr<Starfield>(new Starfield());
#endif

        ss.system.push_back(std::make_shared<SpaceObject>(5000.f, 5510.f , glm::vec3{0,0,0}));
        ss.system.back()->dominant = true;
        ss.system.back()->InitRender(mat);

        for(int i = 0; i < 2; i++)
        {
            ss.system.push_back(std::make_shared<SpaceObject>(random::next<float>()/5.0f,
                                                              3200.f,
                                                              glm::vec3{random::next<float>()*30 - 15,
                                                                        0,
                                                                        random::next<float>()*30 - 15}));

            ss.system.back()->speed = ssolver::make_orbital_vector<float>(*ss.system[0], *ss.system[i+1], ssolver::randomize_orbital<float>(*ss.system[0])*1000);
            ss.system.back()->InitRender(mat);
        }

        bill.vertices = {{{0,0,0},{0,0}},{{0,0,0},{1,0}},{{0,0,0},{1,1}},{{0,0,0},{0,1}}};
        bill.indices = {0,2,1,0,3,2};
        bill.billboards = {{{1, 1},{0,1,0}}};
        bill.material = mat;
        bill.shader = Resources::instance()->Get<BasicJargShader>("corona");
        bill.Bind();

        return true;
    }

    std::function<void()> draw;
    std::function<void()> update;

    template<int is_debug>
    void BaseUpdate()
    {
        glfwPollEvents();

        //m->World = glm::rotate(m->World, (float)gt.elapsed / 100, glm::vec3(0.f,1.f,0.f));

        if(Keyboard::isKeyPress(GLFW_KEY_F2))
        {
            wire = !wire;
            if(wire)
            {
                for(size_t i = 0; i < ss.system.size(); i++)
                {
                    ss.system[i]->render->basic = Resources::instance()->Get<BasicJargShader>("default_planet_render_wire");
                }
            }
            else
            {
                for(size_t i = 0; i < ss.system.size(); i++)
                {
                    ss.system[i]->render->basic = Resources::instance()->Get<BasicJargShader>("default_planet_render_nowire");
                }
            }
        }

        if(Keyboard::isKeyPress(GLFW_KEY_F5))
            Prefecences::Instance()->hdr_on = Prefecences::Instance()->hdr_on ? (DropHdr(), false) : (PreloadHdr(), true);

        if(Keyboard::isKeyPress(GLFW_KEY_F6))
            Prefecences::Instance()->defered_debug = !Prefecences::Instance()->defered_debug;

        if(Keyboard::isKeyPress(GLFW_KEY_F9))
            make_release();
        if(Keyboard::isKeyPress(GLFW_KEY_F10))
            make_debug();
        if(Keyboard::isKeyPress(GLFW_KEY_F11))
            make_spartial();

        glEnable(GL_CULL_FACE);

        if(Mouse::isWheelUp())
            speed *= 1.1f;

        if(Mouse::isWheelDown())
            speed /= 1.1f;

        cam->camera_scale = speed;
        cam->camera_scale *= Keyboard::isKeyDown(GLFW_KEY_LEFT_SHIFT) ? 10.f : 1.f;
        cam->camera_scale *= gt.elapsed;

        if(Keyboard::isKeyDown(GLFW_KEY_W))
            cam->Move(Camera::FORWARD);
        if(Keyboard::isKeyDown(GLFW_KEY_A))
            cam->Move(Camera::LEFT);
        if(Keyboard::isKeyDown(GLFW_KEY_S))
            cam->Move(Camera::BACKWARD);
        if(Keyboard::isKeyDown(GLFW_KEY_D))
            cam->Move(Camera::RIGHT);
        if(Keyboard::isKeyDown(GLFW_KEY_Q))
            cam->Roll(-gt.elapsed);
        if(Keyboard::isKeyDown(GLFW_KEY_E))
            cam->Roll(gt.elapsed);

        if(Keyboard::isKeyPress(GLFW_KEY_F3))
        {
            cam = cam1.get() == cam ? cam2.get() : cam1.get();
        }

        if(Keyboard::isKeyDown(GLFW_KEY_F1))
            cam->Position(glm::vec3(1));

        if(Keyboard::isKeyPress(GLFW_KEY_F2))
            wire = !wire;

        if(Mouse::isRightDown())
            Mouse::SetFixedPosState(true);
        else
            Mouse::SetFixedPosState(false);

        if(Mouse::isRightDown())
        {
            cam->Yaw(Mouse::getCursorDelta().x);
            cam->Pitch(Mouse::getCursorDelta().y);
        }

        if (Mouse::isMiddleJustPressed())
        {
            cam->Reset();
        }

        if(Mouse::isWheelUp())
        {
            cam->Zoom(cam->Zoom() + 1);
        }

        if(Mouse::isWheelDown())
        {
            cam->Zoom(cam->Zoom() - 1);
        }

        for(size_t i = 0; i < ss.system.size(); i++)
        {
            ss.system[i]->Update(ss, gt, *cam);
        }

        qs->world = glm::rotate(qs->world, gt.elapsed/100, glm::vec3(1));
        qs_w->world = glm::rotate(qs_w->world, gt.elapsed/100, glm::vec3(1));
        qs->Update(*cam);
        qs_w->Update(*cam);
        cam1->Update(gt);
        cam2->Update(gt);
        ws->Update(gt);

        Mouse::dropState();
    }

    template<int is_debug>
    void BaseDraw()
    {
        GeometryPass();

        if(!Prefecences::Instance()->defered_debug)
        {
            ShadingPass();
            AftereffectPass();
        }
        else
        {
            BlitGBuffer();
        }

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        batch->setUniform(proj * model);

        ws->Draw();
        //batch->drawText(qs->out, {0,0}, f12.get(), {0,0,0,1});
        //batch->drawText(qs->out, {0,0}, f12.get(), {0,0,0,1});
        batch->render();
    }

    GameTimer gt;
    FPSCounter fps;

    static void Resize(int w, int h)
    {
        if(h == 0)
            h = 1;
        Prefecences::Instance()->resolution = glm::vec2(w, h);
        GameWindow::wi->proj = glm::ortho(0.0f, (float)w, (float)h, 0.0f, -1.f, 1.0f);//.perspective(45, (float)w/float(h), 1, 1000);
        GameWindow::wi->proj_per = glm::perspective(45.0f, w /(float) h, 0.1f, 100.f);
        GameWindow::wi->cam->Viewport(glm::vec4(0,0,w,h));
        GameWindow::wi->view = glm::lookAt(glm::vec3(2,2,2), glm::vec3(0,0,0), glm::vec3(0,1,0));

        GameWindow::wi->model = glm::mat4(1.f);

        if(GameWindow::wi->gb)
            GameWindow::wi->gb->Resize(RESX, RESY);
    }

    void SetTitle(const std::string &str)
    {
        glfwSetWindowTitle(window, str.c_str());
    }

    GLFWmonitor *monitor = nullptr;
    GLFWwindow *window = nullptr;
    std::shared_ptr<GBuffer> gb;

    glm::mat4 view = glm::mat4(1), proj = glm::mat4(1), proj_per = glm::mat4(1), model = glm::mat4(1);
    static GameWindow *wi;

    GameWindow()
    {
        GameWindow::wi = this;
    }
    ~GameWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    SpaceSystem ss;

    std::shared_ptr<SpriteBatch> batch;
    std::shared_ptr<Texture> tex1;
    float speed = 1;

    bool wire = false;
    std::shared_ptr<Camera> cam1, cam2;
    Camera *cam;
    glm::vec3 moving;
    std::vector<glm::vec3> tail;
    Scattering scat;
    std::unique_ptr<Starfield> sf;

    UBillboard<VertPosUv> bill;

    std::shared_ptr<FrameBuffer> fbo_blur, fbo_blur2, fbo_extract, fbo_main;
    std::shared_ptr<Texture> texture_blur, texture_blur2, texture_extract, texture_main;
    std::shared_ptr<Texture> texture_pipeline;

    std::shared_ptr<Font> f12;
    std::shared_ptr<WinS> ws;
    sge_perfomance *perf;
    std::shared_ptr<QuadSphere> qs, qs_w;
    static void Swap()
    {
        glfwSwapBuffers(wi->window);
    }
    void GeometryPass()
    {
        gb->BindForWriting();
        glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 0, 0.f);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        for(size_t i = 0; i < ss.system.size(); i++)
        {
            ss.system[i]->Render(*cam);
        }

        //water->Use();
        //glUniform1f(glGetUniformLocation(water->program, "time"), gt.current);
        //qs_w->Render(*cam);


        bill.Prepare(*cam);
        bill.Bind();
        bill.Render(*cam);

        // When we get here the depth buffer is already populated and the stencil pass
        // depends on it, but it does not write to it.
        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
    }
    void BlitGBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        gb->BindForBlit();

        GLsizei HalfWidth = (GLsizei)(RESX_float / 2.0f);
        GLsizei HalfHeight = (GLsizei)(RESY_float / 2.0f);

        gb->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
        glBlitFramebuffer(0, 0, RESX, RESY,
                          0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

        gb->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
        glBlitFramebuffer(0, 0, RESX, RESY,
                          0, HalfHeight, HalfWidth, RESY, GL_COLOR_BUFFER_BIT, GL_LINEAR);

        gb->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
        glBlitFramebuffer(0, 0, RESX, RESY,
                          HalfWidth, HalfHeight, RESX, RESY, GL_COLOR_BUFFER_BIT, GL_LINEAR);

        gb->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
        glBlitFramebuffer(0, 0, RESX, RESY,
                          HalfWidth, 0, RESX, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
    void ShadingPass()
    {
        gb->BindForReading();
        if(Prefecences::Instance()->hdr_on)
        {
            fbo_main->Bind();
            texture_pipeline = texture_main;
        }
        else
        {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0.f);

        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        glDisable(GL_DEPTH_TEST);

        if(Prefecences::Instance()->starnest_on)
        {
            const auto &nest = Resources::instance()->Get<BasicJargShader>("starnest");
            nest->Use();
            nest->SetUniform("transform_VP", cam->View());
            drawScreenQuad();
        }

        //Resources::instance()->Get<BasicJargShader>("corona")->Use();
        //Resources::instance()->Get<BasicJargShader>("corona")->SetUniform("transform_VP", cam->View());
        //Resources::instance()->Get<BasicJargShader>("corona")->SetUniform("iGlobalTime", gt.current);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, Resources::instance()->Get<Texture>("noise_map")->textureId);
        //drawScreenQuad();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        const auto &deff = Resources::instance()->Get<BasicJargShader>("defered");
        deff->Use();
        deff->SetUniform("transform_lightPos", glm::vec3(0,1,0));
        drawScreenQuad();

#ifndef NO_SCATT
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        scat.redisplayFunc(*cam);
#endif
    }
    void BeginLightPasses();
    void AftereffectPass()
    {
        if(Prefecences::Instance()->hdr_on)
        {
            fbo_extract->Bind();
            glClampColor( GL_CLAMP_VERTEX_COLOR, GL_FALSE );
            Resources::instance()->Get<Shader>("extract_glow")->Use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_pipeline->textureId);
            drawScreenQuad();


            fbo_blur->Bind();
            Resources::instance()->Get<Shader>("gausian_blur")->Use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_extract->textureId);
            drawScreenQuad();


            fbo_blur2->Bind();
            Resources::instance()->Get<Shader>("gausian_blur2")->Use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_blur->textureId);
            drawScreenQuad();


            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            Resources::instance()->Get<Shader>("tone_maping")->Use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_main->textureId);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture_blur2->textureId);
            drawScreenQuad();
        }
    }
    void PreloadHdr()
    {
        fbo_blur    = std::make_shared<FrameBuffer>();
        fbo_blur2   = std::make_shared<FrameBuffer>();
        fbo_main    = std::make_shared<FrameBuffer>();
        fbo_extract = std::make_shared<FrameBuffer>();

        texture_main    = std::make_shared<Texture>(glm::vec2(RESX,RESY), true, false, GL_TEXTURE_2D, GL_RGBA, GL_UNSIGNED_BYTE);
        texture_blur    = std::make_shared<Texture>(glm::vec2(RESX,RESY), true, false, GL_TEXTURE_2D, GL_RGBA, GL_UNSIGNED_BYTE);
        texture_blur2   = std::make_shared<Texture>(glm::vec2(RESX,RESY), true, false, GL_TEXTURE_2D, GL_RGBA, GL_UNSIGNED_BYTE);
        texture_extract = std::make_shared<Texture>(glm::vec2(RESX,RESY), true, false, GL_TEXTURE_2D, GL_RGBA32F, GL_FLOAT, GL_RGBA);

        fbo_main->bindTexture(*texture_main);
        fbo_blur->bindTexture(*texture_blur);
        fbo_blur2->bindTexture(*texture_blur2);
        fbo_extract->bindTexture(*texture_extract);

        fbo_main->CheckErrors();
        fbo_blur->CheckErrors();
        fbo_blur2->CheckErrors();
        fbo_extract->CheckErrors();
    }
    void DropHdr()
    {
        fbo_blur.reset();
        fbo_blur2.reset();
        fbo_main.reset();
        fbo_extract.reset();
        texture_main.reset();
        texture_blur.reset();
        texture_blur2.reset();
        texture_extract.reset();
    }
};

#endif // GAMEWINDOW_H
