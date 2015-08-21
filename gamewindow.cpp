#include "gamewindow.h"
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
//#define NO_SCATT
//#define NO_STARFIELD

GameWindow::GameWindow()
{
    GameWindow::wi = this;
}

GameWindow::~GameWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool GameWindow::BaseInit()
{
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
    //glfwWindowHint(GLFW_SAMPLES, 16);
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
    int ntex;
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ntex);
    LOG(info) << "Renderer: " << glGetString(GL_RENDERER);
    LOG(info) << "Vendor: " << glGetString(GL_VENDOR);
    LOG(info) << "Version: " << glGetString(GL_VERSION);
    LOG(info) << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
    LOG(info) << "using OpenGL: " << glVersion[0] << "." << glVersion[1];
    LOG(info) << "GLFW: " << glfwGetVersionString();
    LOG(info) << "GLEW: " << glewGetString(GLEW_VERSION);
    LOG(info) << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: " << ntex;
    if(ntex < 16)
        throw;
    LOG(info) << EXT_CHECK(GLEW_ARB_multi_bind);
    LOG(info) << EXT_CHECK(GLEW_ARB_tessellation_shader);
    LOG(info) << EXT_CHECK(GLEW_EXT_geometry_shader4);

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

    cam1->Position({3000,3000,3000});
    cam1->LookAt({0,0,0});

    cam2->Position({3000,3000,3000});
    cam2->LookAt({0,0,0});
    Resize(RESX, RESY);

    std::shared_ptr<Material> mat = std::make_shared<Material>();
    mat->low = Resources::instance()->Get<Texture>("grass");
    mat->medium = Resources::instance()->Get<Texture>("soil");
    mat->high = Resources::instance()->Get<Texture>("snow");
    mat->side = Resources::instance()->Get<Texture>("rock");


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

    return true;
}

void GameWindow::BaseUpdate()
{
    glfwPollEvents();

    //m->World = glm::rotate(m->World, (float)gt.elapsed / 100, glm::vec3(0.f,1.f,0.f));

    if(Keyboard::isKeyPress(GLFW_KEY_F2))
        wire = !wire;
    glEnable(GL_CULL_FACE);
    if(wire)
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    else
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

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

   qs->world = glm::rotate(qs->world, gt.elapsed/100, glm::vec3(1));
   qs_w->world = glm::rotate(qs_w->world, gt.elapsed/100, glm::vec3(1));
   qs->Update(*cam);
   qs_w->Update(*cam);
   cam1->Update(gt);
   cam2->Update(gt);
   ws->Update(gt);

   Mouse::dropState();
}

void GameWindow::Swap()
{
    glfwSwapBuffers(wi->window);
}

void GameWindow::BaseDraw()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0,0,0, 1.f);

#ifndef NO_STARFIELD
    sf->Render(*cam);
#endif
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    for(size_t i = 0; i < ss.system.size(); i++)
    {
        ss.system[i]->Update(ss, gt, *cam);
        ss.system[i]->Render(*cam);
    }

#ifndef NO_SCATT
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    scat.redisplayFunc(*cam);
#endif

    //water->Use();
    //glUniform1f(glGetUniformLocation(water->program, "time"), gt.current);
    //qs_w->Render(*cam);


    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    batch->setUniform(proj * model);

    ws->Draw();
    batch->drawText(qs->out, {0,0}, f12.get(), {0,0,0,1});
    batch->drawText(qs->out, {0,0}, f12.get(), {0,0,0,1});
    batch->render();

    Swap();
    gt.Update(static_cast<float>(glfwGetTime()));
    fps.Update(gt);
    perf->UpdateTimer(fps, gt);
}

void GameWindow::Mainloop()
{
    BaseInit();
    while(!glfwWindowShouldClose(window))
    {
        BaseUpdate();
        BaseDraw();
       // std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    Resources::drop();
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
    glViewport(0, 0, w, h);

    if(GameWindow::wi->gb)
        GameWindow::wi->gb->Resize(RESX, RESY);
}

void GameWindow::SetTitle(const std::string &str)
{
    glfwSetWindowTitle(window, str.c_str());
}

GameWindow *GameWindow::wi = nullptr;
