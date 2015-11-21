/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "gamewindow.h"
#include "geometry/cube.h"

GameWindow *GameWindow::wi = nullptr;


void GameWindow::Mainloop()
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

void GameWindow::make_release()
{
    update = [=](){BaseUpdate<false>();};
    draw = [=](){BaseDraw<false>();};
    main_is_debug = false;
    glfwWindowHint(GLFW_SAMPLES, 1);
}

void GameWindow::make_spartial()
{
    update = [=](){BaseUpdate<spartial_rendering>();};
    draw = [=](){BaseDraw<spartial_rendering>();};
    main_is_debug = spartial_rendering;
    glfwWindowHint(GLFW_SAMPLES, 16);
}

bool GameWindow::BaseInit()
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
    window = glfwCreateWindow(rx, ry, sge::string_format("sge %dx%d", rx, ry).c_str(), monitor, nullptr);
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
    int ntex, nuni, texss;
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ntex);
    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &nuni);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texss);
    LOG(info) << "Renderer: " << glGetString(GL_RENDERER);
    LOG(info) << "Vendor: " << glGetString(GL_VENDOR);
    LOG(info) << "Version: " << glGetString(GL_VERSION);
    LOG(info) << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
    LOG(info) << "using OpenGL: " << glVersion[0] << "." << glVersion[1];
    LOG(info) << "GLFW: " << glfwGetVersionString();
    LOG(info) << "GLEW: " << glewGetString(GLEW_VERSION);
    LOG(info) << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: " << ntex;
    LOG(info) << "GL_MAX_UNIFORM_LOCATIONS: " << nuni;
    LOG(info) << "GL_MAX_TEXTURE_SIZE: " << texss;
    if(ntex < 16)
        throw;
    LOG(info) << EXT_CHECK(GLEW_ARB_multi_bind);
    LOG(info) << EXT_CHECK(GLEW_ARB_tessellation_shader);
    LOG(info) << EXT_CHECK(GLEW_ARB_texture_float);
    LOG(info) << EXT_CHECK(GLEW_ARB_get_program_binary);

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
    SRenderSettings::Init();

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

    cam = std::make_shared<Camera>();

    cam->Position({1,1,1});
    cam->LookAt({0,0,0});
    Resize(RESX, RESY);

    //================================

    std::shared_ptr<SObject> o = std::make_shared<SObject>();
    objects.push_back(o);
    std::shared_ptr<SRenderAgent> r = std::make_shared<SRenderAgent>();
    std::shared_ptr<SMeshAgent> m = std::make_shared<SMeshAgent>();
    std::shared_ptr<SRenderAgent> t = std::make_shared<SRenderAgent>();
    o->Add(r);
    o->Add(m);
    o->Add(t);
    r->mesh = m;
    m->mesh = GenerateCube<VertPosNormUvUv>();

    TextureAtlas::LoadAll();

    return true;
}

void GameWindow::Resize(int w, int h)
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

void GameWindow::SetTitle(const std::string &str)
{
    glfwSetWindowTitle(window, str.c_str());
}

GameWindow::GameWindow()
{
    GameWindow::wi = this;
}

GameWindow::~GameWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GameWindow::Swap()
{
    glfwSwapBuffers(wi->window);
}

template<int is_debug>
void GameWindow::BaseDraw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0.f);

    batch->setUniform(proj);


    batch->drawQuad({0,0},{800,800}, *TextureAtlas::tex, Color::White);
    for(int i =0; i<10; i++)
        for(int j =0; j<10; j++)
        {
            int x = i*64/2 - j*64/2 + 300;
            int y = i*32/2 + j*32/2 + 300;

            batch->drawQuadAtlas({x,y}, {64,32}, *TextureAtlas::tex, 0, Color::White);
        }

    if(is_debug)
    {
        batch->drawText(sge::string_format("%d dc UI\n"
                                           "%d dc UM\n"
                                           "=%d\n"
                                           "fps %d",
                                           batch->getDc(),
                                           UMeshDc::getDc(),
                                           UMeshDc::getDc() + batch->getDc(),
                                           fps.GetCount()), {RESX-70, 2+20}, f12.get(), Color::White);
    }

    if(!no_ui) ws->Draw();

    switch(Mouse::state)
    {
    case Mouse::STATE_RESIZE:
        batch->drawQuad(Mouse::getCursorPos(), {32,32}, *Resources::instance()->Get<Texture>("cur_resize"), Color::White);
        break;
    default:
        batch->drawQuad(Mouse::getCursorPos(), {32,32}, *Resources::instance()->Get<Texture>("cur_mouse"), Color::White);
        break;
    }
    //batch->drawText(qs->out, {0,0}, f12.get(), {0,0,0,1});
    //batch->drawText(qs->out, {0,0}, f12.get(), {0,0,0,1});
    batch->render();

    if(is_debug)
    {
        batch->resetDc();
        UMeshDc::resetDc();
    }
    SRenderSettings::Reset();
}

template<int is_debug>
void GameWindow::BaseUpdate()
{
    Mouse::state = Mouse::STATE_MOUSE;
    glfwPollEvents();

    //m->World = glm::rotate(m->World, (float)gt.elapsed / 100, glm::vec3(0.f,1.f,0.f));

    if(Keyboard::isKeyPress(GLFW_KEY_F2))
    {
        wire = !wire;
    }

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
        cam->setRoll(-1);
    if(Keyboard::isKeyDown(GLFW_KEY_E))
        cam->setRoll(1);

    if(Keyboard::isKeyDown(GLFW_KEY_F1))
        cam->Position(glm::vec3(1));

    if(Keyboard::isKeyPress(GLFW_KEY_F2))
        wire = !wire;
    if(Keyboard::isKeyPress(GLFW_KEY_F4))
        no_ui = !no_ui;

    if(Mouse::isRightDown())
        Mouse::SetFixedPosState(true);
    else
        Mouse::SetFixedPosState(false);

    if(Mouse::isRightDown())
    {
        cam->setYaw(Mouse::getCursorDelta().x);
        cam->setPitch(Mouse::getCursorDelta().y);
    }

    if (Mouse::isMiddleJustPressed())
    {
        cam->Reset();
    }

    if(Mouse::isWheelUp())
    {
        cam->setZoom(cam->getZoom() + 1);
    }

    if(Mouse::isWheelDown())
    {
        cam->setZoom(cam->getZoom() - 1);
    }

    cam->Update(gt, true);
    static MouseState s;
    ws->Update(gt, s);

    Mouse::dropState();
}
