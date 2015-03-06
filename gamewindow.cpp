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
#include "cube.h"
#include "icosahedron.h"
#include "SphereTesselator.h"
#include "helper.h"
#include "voronoi/Voronoi.h"
#include <algorithm>
#include "ClassicNoise.h"
#include "voronoi/Lloyd.h"

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

std::vector<glm::vec2> Generate(const unsigned int count, const glm::uvec2 &size)
{

  std::vector<glm::vec2> points;
  points.reserve(count);

  unsigned int seed = static_cast<unsigned int>(time(NULL));
  srand(seed);

  struct Generator
  {
    const glm::uvec2 size;
    Generator(const glm::uvec2 &s)
      : size(s)
    {}
    glm::vec2 operator()() {return glm::vec2(rand() % (size.x * 10) / 10.f, rand() % (size.y * 10) / 10.f);}
  } generator(size);

  std::generate_n(std::back_inserter(points), count, generator);


  std::sort(points.begin(), points.end(),
    [](const glm::vec2 &p1, const glm::vec2 &p2) -> bool
  {
    if(p1.y == p2.y)
      return p1.x > p2.x;
    return p1.y > p2.y;
  });

  auto it = std::unique(points.begin(), points.end(),
    [](const glm::vec2 &p1, const glm::vec2 &p2)
    {
      return p1.x == p2.x && p1.y == p2.y;
    });

  points.resize(std::distance(points.begin(), it));

  return std::move(points);
}

double RotationPoint(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
{
    return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x);
}

std::vector<glm::vec2> SortPointCcw(const std::vector<glm::vec2> &points)
{
    // Ищем самую левую точку.
    // Если таких точек несколько - выбираем верхную
    auto itPos = points.begin();
    for(auto it = points.begin(); it != points.end(); ++it)
    {
        if((*itPos).x > (*it).x)
        {
            itPos = it;
        }
        else if((*itPos).x == (*it).x)
        {
            if((*itPos).y <= (*it).y)
            {
                itPos = it;
            }
        }
    }
    glm::vec2 point = *itPos;
    std::multiset<glm::vec2, std::function<bool(const glm::vec2 &, const glm::vec2 &)> >
    sortedPoints([point](const glm::vec2 &a,const glm::vec2 &b)
    {
        return RotationPoint(point, a, b) > 0;
    });
    for(auto it = points.begin(); it != points.end(); ++it)
    {
        if(itPos != it)
        sortedPoints.insert(*it);
    }
    std::vector<glm::vec2> output;
    output.reserve(points.size());
    output.push_back(point);
    output.insert(output.end(), sortedPoints.begin(), sortedPoints.end());
    return output;
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
    Mouse::initialize(window);
    Resize(Prefecences::Instance()->resolution.x, Prefecences::Instance()->resolution.y);
    //Mouse::SetFixedPosState(true);
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos){
        Mouse::SetCursorPos(xpos, ypos);
    });
    glfwSetCursorEnterCallback(window, [](GLFWwindow *window, int entered){
        Mouse::cursorClientArea(entered);
    });
    glfwSetWindowFocusCallback(window, [](GLFWwindow *window, int focused){
        Mouse::windowFocus(focused);
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int a, int b, int c){
        Mouse::SetButton(a, b, c);
    });
    glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int a, int b){
        GameWindow::Resize(a, b); Mouse::setWindowSize(a, b);
    });
    glfwSetScrollCallback(window, [](GLFWwindow *window, double a, double b){
        Mouse::Scroll(b);
    });

    batch = std::make_shared<SpriteBatch>();

    f12 = std::make_shared<Font>();
    f12->initFreeType(12);
    f12->renderAtlas();

    atlas.LoadAll();

    cam.SetPosition({1,1,1});
    cam.SetLookAt({0,0,0});
    cam.MaxPitch = glm::two_pi<float>();

    basic = std::make_shared<BasicJargShader>();
    basic->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal.glsl");
    basic->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal.glsl");
    basic->Link();
    basic->Use();
    basic->Afterlink();

    m = std::make_shared<Mesh>();
    //Tesselator::SphereTesselate(4, m);
    m->shader = basic;


    glm::vec2 size = {40,40};
    std::vector<glm::vec2> mListSite = Generate(1000, size);

    for(int i = 0; i < 44; i++)
        mListSite = Lloyd(mListSite, size);

    std::vector<std::vector<unsigned int> > listVertex;

    for(unsigned int i = 0; i < 1; ++i)
    {
      // Строим диаграмму
      Voronoi voronoi(mListSite, size);
      voronoi();

      listVertex.clear();
      listVertex.resize(mListSite.size());
      mListSite.clear();

      // Строим список вершин для каждой точки.
      auto edges = voronoi.GetEdges();

      for(auto it = edges.begin(); it != edges.end(); ++it)
      {
        const Voronoi::Edge &edge = (*it);

        if(std::find(listVertex[edge.site1].begin(), listVertex[edge.site1].end(), edge.vertex1) ==
           listVertex[edge.site1].end())
        {
          listVertex[edge.site1].push_back(edge.vertex1);
        }
        if(std::find(listVertex[edge.site1].begin(), listVertex[edge.site1].end(), edge.vertex2) ==
           listVertex[edge.site1].end())
        {
          listVertex[edge.site1].push_back(edge.vertex2);
        }
        if(std::find(listVertex[edge.site2].begin(), listVertex[edge.site2].end(), edge.vertex1) ==
           listVertex[edge.site2].end())
        {
          listVertex[edge.site2].push_back(edge.vertex1);
        }
        if(std::find(listVertex[edge.site2].begin(), listVertex[edge.site2].end(), edge.vertex2) ==
           listVertex[edge.site2].end())
        {
          listVertex[edge.site2].push_back(edge.vertex2);
        }
      }


      auto vertex = voronoi.GetVertex();
      std::vector<float> height;
      height.resize(vertex.size());
      for(int i = 0; i < vertex.size(); i++)
      {
          height[i] = rand()%100 / 100.f;
      }
      // Вычисляем новые значения точек.
      for(unsigned int j = 0; j < listVertex.size(); ++j)
      {
        auto const &poligon = listVertex[j];
        if(poligon.empty()) {
            continue;
        }
        glm::vec2 point;
        int count = 0;
        int start = m->Vertices.size();
        std::vector<glm::vec2> verteces;
        for(auto jt = poligon.begin(); jt != poligon.end(); ++jt)
        {
          point += vertex[*jt];
          verteces.push_back(vertex[*jt]);
          count++;
        }
        verteces = SortPointCcw(verteces);
        for(int i = 0; i < verteces.size(); i++){
            m->Vertices.push_back(VertPosNormTanBiTex({verteces[i].x - size.x / 2,
                                                       //Noise::simplexnoise(verteces[i].x,verteces[i].y) +
                                                       //Noise::simplexnoise(verteces[i].x/16.f, verteces[i].y/16.f)*3,
                                                       0,
                                                       verteces[i].y - size.y / 2}
                                                      ,{verteces[i].x / size.x, verteces[i].y / size.y}));
        }
        if(count >= 3)
        {
            for(int n = 1; n < count -1; n++)
            {
                m->Indices.push_back(start);
                m->Indices.push_back(start + n);
                m->Indices.push_back(start + n + 1);
            }
        }

        mListSite.push_back(point / static_cast<float>(poligon.size()));
      }
    }

    m->Bind();
    m->Unindex();
    m->computeNormal();

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

    m->World = glm::rotate(m->World, (float)gt.elapsed / 100, glm::vec3(0.f,1.f,0.f));

    if(Keyboard::isKeyPress(GLFW_KEY_F2)){
        wire = wire ? (glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ), false) : (glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ), true);
    }

    if (Keyboard::isKeyDown(GLFW_KEY_LEFT_ALT))
   {
   Mouse::SetFixedPosState(true);
   cam.pitch += Mouse::getCursorDelta().y / 1000.f;
   }
   else
   Mouse::SetFixedPosState(false);
   if (Mouse::isMiddleDown())
   {
   cam.Reset();
   }
   if (Mouse::isRightDown() || Keyboard::isKeyDown(GLFW_KEY_LEFT_ALT))
   {
   cam.yaw += Mouse::getCursorDelta().x / 1000.f;
   cam.viewMatrixDirty = true;
   }

    Mouse::resetDelta();
}

void GameWindow::BaseDraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 1);


    glEnable(GL_DEPTH_TEST);
    m->Render(cam.VP);
    cam.Update();

//    glMatrixMode(GL_PROJECTION);
//    glLoadMatrixf((const GLfloat*)&cam.projection[0][0]);
//    glMatrixMode(GL_MODELVIEW);
//    glm::mat4 MV = cam.view * m->World;
//    glLoadMatrixf((const GLfloat*)&MV[0][0]);
//    glUseProgram(0);
//    glBegin(GL_LINES);
//    for(int i=0;i<m->Indices.size();i++)
//    {
//        glColor3f(1,0,0);
//        glm::vec3 p = m->Vertices[m->Indices[i]].position;
//        glVertex3fv(&p.x);
//        glm::vec3 o = glm::normalize(m->Vertices[m->Indices[i]].normal);
//        p+=o*0.1f;
//        glVertex3fv(&p.x);


//        glColor3f(0,1,0);
//        p = m->Vertices[m->Indices[i]].position;
//        glVertex3fv(&p.x);
//        o = glm::normalize(m->Vertices[m->Indices[i]].tangent);
//        p+=o*0.1f;
//        glVertex3fv(&p.x);

//        glColor3f(0,0,1);
//        p = m->Vertices[m->Indices[i]].position;
//        glVertex3fv(&p.x);
//        o = glm::normalize(m->Vertices[m->Indices[i]].binormal);
//        p+=o*0.1f;
//        glVertex3fv(&p.x);
//    }
//    glEnd();

    glDisable(GL_DEPTH_TEST);
    batch->setUniform(proj * model);
    batch->drawRect({100,100}, {100,100}, Color::CornflowerBlue);
    batch->drawText(std::to_string(fps.GetCount()), {100, 100}, f12.get(), Color::White);
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
    GameWindow::wi->view = glm::lookAt(glm::vec3(2,2,2), glm::vec3(0,0,0), glm::vec3(0,1,0));

    GameWindow::wi->model = glm::mat4(1.f);
}

void GameWindow::SetTitle(const std::string &str)
{
    glfwSetWindowTitle(window, str.c_str());
}

GameWindow *GameWindow::wi = nullptr;
