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
#include "geometry/cube.h"
#include "geometry/icosahedron.h"
#include "SphereTesselator.h"
#include <algorithm>
#include "ClassicNoise.h"
#include "geometry/model.h"
#include "resources/resourcecontroller.h"
#include "TextureGenerator.h"
#include "resources/random_noise.h"
#include "space/space_object.h"
#include "space/spacesystem.h"
#include <glm/gtx/compatibility.hpp>

#define MAJOR 2
#define MINOR 1

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

    int err = glewInit();
    if (err != GLEW_OK)
    {
        LOG(fatal) << "GLEW init fatal etrror";
        LOG(fatal) << "GLEW Error : " << std::string((const char*)glewGetErrorString(err));
        return false;
    }


    int glVersion[2] = {-1, -1};
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);
    LOG(info) << "Renderer: " << glGetString(GL_RENDERER);
    LOG(info) << "Vendor: " << glGetString(GL_VENDOR);
    LOG(info) << "Version: " << glGetString(GL_VERSION);
    LOG(info) << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION);
    LOG(info) << "using OpenGL: " << glVersion[0] << "." << glVersion[1];
    LOG(info) << "GLFW: " << glfwGetVersionString();
    LOG(info) << "GLEW: " << glewGetString(GLEW_VERSION);

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
    glfwSetCursorEnterCallback(window, [](GLFWwindow *, int entered){
        Mouse::cursorClientArea(entered);
    });
    glfwSetWindowFocusCallback(window, [](GLFWwindow *, int focused){
        Mouse::windowFocus(focused);
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow *, int a, int b, int c){
        Mouse::SetButton(a, b, c);
    });
    glfwSetWindowSizeCallback(window, [](GLFWwindow *, int a, int b){
        GameWindow::Resize(a, b); Mouse::setWindowSize(a, b);
    });
    glfwSetScrollCallback(window, [](GLFWwindow *, double , double b){
        Mouse::Scroll(b);
    });

    gb = std::make_shared<GBuffer>();
    gb->Init(RESX, RESY);

    batch = std::make_shared<SpriteBatch>();

    f12 = std::make_shared<Font>();
    f12->initFreeType(12);
    f12->renderAtlas();

    ws = std::make_shared<WinS>(batch.get());
    ws->f = f12.get();

    perf = new sge_perfomance(ws.get());

    atlas.LoadAll();

    cam1 = std::make_shared<Camera>();
    cam2 = std::make_shared<Camera>();
    cam = cam1.get();

    cam1->Position({3000,3000,3000});
    cam1->LookAt({0,0,0});

    cam2->Position({3000,3000,3000});
    cam2->LookAt({0,0,0});
    Resize(RESX, RESY);

    Resources::instance();

    basic = std::make_shared<BasicJargShader>();
    basic->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal.glsl");
    basic->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal.glsl");
    basic->Link();
    basic->Use();
    basic->Afterlink();

    water = std::make_shared<BasicJargShader>();
    water->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal_watertest.glsl");
    water->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal_watertest.glsl");
    water->Link();
    water->Use();
    water->Afterlink();

    m = std::make_shared<Mesh>(Icosahedron::getMesh());
    //Tesselator::SphereTesselate(4, m);
    m->shader = basic;
    m->World = glm::scale(glm::mat4(1), glm::vec3(4,4,4));

    std::shared_ptr<Material> mat = std::make_shared<Material>();
    std::shared_ptr<Texture> texx = std::make_shared<Texture>();
    texx->Load("data/derevo.png", true, true);
    std::shared_ptr<Texture> texxx = std::make_shared<Texture>();
    texxx->Load("data/aaa.png", true, true);
    mat->texture = texx;
    mat->normal = texxx;
    m->material = mat;

    auto wm = std::make_shared<Material>();
    auto wt = std::make_shared<Texture>();
    auto wn = std::make_shared<Texture>();
    wt->Load("data/water.png", true, true);
    wn->Load("data/normal.png", true, true);
    wm->texture = wt;
    wm->normal = wn;

    qs = std::make_shared<QuadSphere>(basic, mat);
    qs->max_divide = 5;
    qs_w = std::make_shared<QuadSphere>(water, wm);
    qs_w->max_divide = 5;
    qs_w->s = 1;
    qs_w->R = 1010;
    wm->diffuse = Color::SeaBlue;
    wm->shininess = 80;

    scat.Precompute();

    ss.system.push_back(std::make_shared<SpaceObject>(1000, 1 , glm::vec3{0,0,0}));
    ss.system.push_back(std::make_shared<SpaceObject>(50, 1 , glm::vec3{1000,1000,1000}));
    ss.system.back()->speed = glm::vec3(1000000,1000000,0);
    for(int i = 0 ; i < 10; i++)
    ss.system.push_back(std::make_shared<SpaceObject>(trand<float>() * 1, 1 , glm::vec3{trand<float>() * 3000, trand<float>() * 3000, trand<float>() * 3000}));

    return true;
}

std::vector<glm::vec3> getTail(glm::vec3 start, glm::vec3 delta)
{
    int sss = 100000;
    std::vector<glm::vec3> a;
    a.resize(sss);
    for(int i = 0; i < sss; ++i)
    {
        a.push_back(start);
        start += delta*1.0f;
        delta += glm::normalize(-start)*0.009f;
    }
    return a;
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

    cam->camera_scale = Keyboard::isKeyDown(GLFW_KEY_LEFT_SHIFT) ? 10.f : 1.f;

    cam->camera_scale /= Keyboard::isKeyDown(GLFW_KEY_LEFT_CONTROL) ? 100.f : 1.f;

    if(Keyboard::isKeyDown(GLFW_KEY_W))
        cam->Move(Camera::FORWARD);
    if(Keyboard::isKeyDown(GLFW_KEY_A))
        cam->Move(Camera::LEFT);
    if(Keyboard::isKeyDown(GLFW_KEY_S))
        cam->Move(Camera::BACK);
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
   Mouse::SetFixedPosState(true);
   cam->Yaw(Mouse::getCursorDelta().x);
   cam->Pitch(Mouse::getCursorDelta().y);

   if (Mouse::isMiddleDown())
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
    ws->Update();

    Mouse::dropState();
}

void GameWindow::BaseDraw()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(100/255.f, 149/255.f, 237/255.f, 1.f);
    glClearColor(0,0,0, 1.f);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    scat.redisplayFunc(*cam);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    for(int i = 0; i < ss.system.size(); i++)
    {
        qs->world = glm::translate(glm::mat4(1), glm::vec3(ss.system[i]->pos));
        qs->s *= ss.system[i]->R()/qs->R*10;
        qs->R = ss.system[i]->R()*10;
        qs->Render(*cam);
        ss.system[i]->Update(ss, gt);

        batch->drawText(ss.system[i]->GetDebugInfo(), {250*i,0}, f12.get(), {1,0,1,1});
    }

    //water->Use();
    //glUniform1f(glGetUniformLocation(water->program, "time"), gt.current);
    //qs_w->Render(*cam);


    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    batch->setUniform(proj * model);

    ws->Draw();
    batch->drawText(qs->out, {0,0}, f12.get(), {0,0,0,1});
    batch->drawText(std::to_string(glm::length(moving)).append(" km/s"), {0, 100}, f12.get(), {0,0,0,1});        
    batch->render();


    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf((const GLfloat*)&cam->Projection());
    glMatrixMode(GL_MODELVIEW);
    glm::mat4 MV = cam->View();
    glLoadMatrixf((const GLfloat*)&MV[0][0]);
    glUseProgram(0);
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,1);
    glEnd();

    for(int i = 0; i < ss.system.size(); i++)
    {
        glBegin(GL_LINES);
            for(int b = ss.system[i]->cur_h; b < ss.system[i]->max_h + ss.system[i]->cur_h; b++)
            {
                auto a = b % ss.system[i]->max_h;
                if(a == 0) continue;
                glColor4fv(&glm::lerp(Color::Clear, ss.system[i]->color, (b - ss.system[i]->cur_h)/(float)ss.system[i]->hist.size())[0]);
                glVertex3dv(&ss.system[i]->hist[a-1][0]);
                glVertex3dv(&ss.system[i]->hist[a][0]);
            }
        glEnd();
    }

//    glEnable(GL_DEPTH_TEST);
//    glBegin(GL_LINES);
//        if(tail.size())
//        for(size_t i = 0; i < tail.size() - 1; ++i)
//        {
//            glVertex3fv(&tail[i][0]);
//            glVertex3fv(&tail[i+1][0]);
//        }
//    glEnd();

    glfwSwapBuffers(window);
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
        //std::this_thread::sleep_for(std::chrono::milliseconds(16));
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
