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
#include "helper.inl"
#include "cube.h"
#include "icosahedron.h"
#include "SphereTesselator.h"

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
    LOG(info) << "Jarg initialization start";
    LOG(info) << "User-preferred locale setting is " << std::locale("").name().c_str();
    glfwSetErrorCallback([](int a,const char* description){LOG(error) << description;});
    int glfwErrorCode = glfwInit();
    if (!glfwErrorCode)
    {
        LOG(error) << "glfwInit error " << glfwErrorCode;
        return false;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
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
    glfwSwapInterval(1);

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
    glfwSetKeyCallback(window, [](GLFWwindow *win, int key, int scancode, int action, int mods){
        Keyboard::SetKey(key, scancode, action, mods);
    });
    Mouse::Initialize(window);
    Resize(Prefecences::Instance()->resolution.x, Prefecences::Instance()->resolution.y);
    //Mouse::SetFixedPosState(true);
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos){
        Mouse::SetCursorPos(xpos, ypos);
    });
    glfwSetCursorEnterCallback(window, [](GLFWwindow *window, int entered){
        Mouse::CursorClientArea(entered);
    });
    glfwSetWindowFocusCallback(window, [](GLFWwindow *window, int focused){
        Mouse::WindowFocus(focused);
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int a, int b, int c){
        Mouse::SetButton(a, b, c);
    });
    glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int a, int b){
        GameWindow::Resize(a, b); Mouse::SetWindowSize(a, b);
    });
    glfwSetScrollCallback(window, [](GLFWwindow *window, double a, double b){
        Mouse::Scroll(b);
    });

    batch = std::make_shared<SpriteBatch>();

    f12 = std::make_shared<Font>();
    f12->initFreeType(12);
    f12->renderAtlas();

    atlas.LoadAll();

    basic = std::make_shared<BasicJargShader>();
    basic->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal.glsl");
    basic->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal.glsl");
    basic->Link();
    basic->Use();
    basic->getAttrib();
    basic->locateVars("transformmodel");
    basic->locateVars("transformviewProjection");
    basic->locateVars("material.texture");
    basic->locateVars("transformviewPosition");
    basic->locateVars("transformnormal");
    basic->locateVars("transformlightPosition");
    glUniform1i(basic->vars[2], 0);
    glUniformMatrix4fv(basic->vars[3],  1, GL_FALSE,  &glm::translate(glm::mat4(1), glm::vec3(2.f,2.f,2.f))[0][0]);

    m = Tesselator::SphereTesselate(6, Cube::getMesh());
    m->shader = basic;
    m->Unindex();
    m->computeNormal();
    m->CalcTB();
    m->Bind();

    std::shared_ptr<Material> mat = std::make_shared<Material>();
    std::shared_ptr<Texture> texx = std::make_shared<Texture>();
    texx->Load("data/tex/derevo.png", true, true);
    std::shared_ptr<Texture> texxx = std::make_shared<Texture>();
    texxx->Load("data/tex/aaa.png", true, true);
    mat->texture = texx;
    mat->normal = texxx;
    m->material = mat;
}

void GameWindow::BaseUpdate()
{
    glfwPollEvents();

    m->World = glm::rotate(m->World, (float)gt.elapsed, glm::vec3(0.f,1.f,0.f));

    Mouse::resetDelta();
}

void GameWindow::BaseDraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);


    glEnable(GL_DEPTH_TEST);
    m->Render(glm::mat4(1), proj_per * view);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf((const GLfloat*)&proj_per[0][0]);
    glMatrixMode(GL_MODELVIEW);
    glm::mat4 MV = view * m->World;
    glLoadMatrixf((const GLfloat*)&MV[0][0]);
    glUseProgram(0);
//    glBegin(GL_LINES);
//    for(int i=0;i<m->Indices.size();i++)
//    {
//        glColor3f(1,0,0);
//        glm::vec3 p = m->Vertices[m->Indices[i]].Position;
//        glVertex3fv(&p.x);
//        glm::vec3 o = glm::normalize(m->Vertices[m->Indices[i]].Normal);
//        p+=o*0.1f;
//        glVertex3fv(&p.x);


//        glColor3f(0,1,0);
//        p = m->Vertices[m->Indices[i]].Position;
//        glVertex3fv(&p.x);
//        o = glm::normalize(m->Vertices[m->Indices[i]].Tangent);
//        p+=o*0.1f;
//        glVertex3fv(&p.x);

//        glColor3f(0,0,1);
//        p = m->Vertices[m->Indices[i]].Position;
//        glVertex3fv(&p.x);
//        o = glm::normalize(m->Vertices[m->Indices[i]].Binormal);
//        p+=o*0.1f;
//        glVertex3fv(&p.x);
//    }
//    glEnd();

    glDisable(GL_DEPTH_TEST);
    batch->setUniform(proj * model);
    batch->drawRect({100,100}, {100,100}, Color::CornflowerBlue);
    batch->renderText(std::to_string(fps.GetCount()), 100, 100, f12.get(), Color::White);
    batch->render();

    glfwSwapBuffers(window);
    gt.Update(glfwGetTime());
    fps.Update(gt);
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
}

void GameWindow::Resize(int w, int h)
{
    if(h == 0)
        h = 1;
    Prefecences::Instance()->resolution = glm::vec2(w, h);
    glViewport(0, 0, w, h);
    GameWindow::wi->proj = glm::ortho(0.0f, (float)w, (float)h, 0.0f, -1.f, 1.0f);//.perspective(45, (float)w/float(h), 1, 1000);
    GameWindow::wi->proj_per = glm::perspective(45.0f, w /(float) h, 0.1f, 100.f);
    GameWindow::wi->view = glm::mat4(1);
    GameWindow::wi->view = glm::lookAt(glm::vec3(2,2,2), glm::vec3(0,0,0), glm::vec3(0,1,0));

    GameWindow::wi->model = glm::mat4(1.f);
}

void GameWindow::SetTitle(const std::string &str)
{
    glfwSetWindowTitle(window, str.c_str());
}

GameWindow *GameWindow::wi = nullptr;
