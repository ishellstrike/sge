/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "gamewindow.h"
#include "helper.h"
#include <fstream>
#include "core/db.h"
#include "core/remoteclient.h"
#include "core/agents/agents.hpp"
#include "remsnd.h"

#define GLM_SWIZZLE

GameWindow *GameWindow::wi = nullptr;


void GameWindow::Mainloop()
{
    AL::InitializeOpenAL();
    BaseInit();

    while(!glfwWindowShouldClose(window))
    {
        update_pass += gt.elapsed;
        if(update_pass >= 0.03)
        {
            update();
            update_pass = 0;
        }

        if(main_is_debug == spartial_rendering)
        {
            static int ii = 0, jj = 0;
            ii++;
            if(ii >= 10) { ii = 0; jj++;}
            if(jj >= 10) {jj = 0; }
            glScissor( ii*RESX/10, jj*RESY/10, RESX/10, RESY/10 );
            glEnable( GL_SCISSOR_TEST );
        }

        draw();

        Swap();

        gt.Update(static_cast<float>(glfwGetTime()));
        fps.Update(gt);
        perf->UpdateTimer(fps, gt);
        // std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    Resources::drop();
    AL::DestroyOpenAL();
}

void GameWindow::make_release()
{
    update = [=](){
        BaseUpdate<false>();
    };
    draw = [=](){
        BaseDraw<false>();
    };
    main_is_debug = false;
    //glfwWindowHint(GLFW_SAMPLES, 1);
}

void GameWindow::make_spartial()
{
    update = [=](){BaseUpdate<spartial_rendering>();};
    draw = [=](){BaseDraw<spartial_rendering>();};
    main_is_debug = spartial_rendering;
    //glfwWindowHint(GLFW_SAMPLES, 16);
}

bool GameWindow::BaseInit()
{
    update = [=](){
        BaseUpdate<false>();
    };
    draw = [=](){
        BaseDraw<false>();
    };

    srand(123);
    LOG(info) << "Jarg initialization start";
    LOG(info) << "User-preferred locale setting is " << std::locale("").name().c_str();
    glfwSetErrorCallback([](int /*a*/,const char* description){
        LOG(error) << description;
    });
    int glfwErrorCode = glfwInit();
    if (!glfwErrorCode)
    {
        LOG(error) << "glfwInit error " << glfwErrorCode;
        return false;
    }
    //glfwWindowHint( GLFW_SAMPLES, 16 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, MAJOR );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, MINOR );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE );

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
    linfo = new sge_level_debug_info(ws.get());
    settings = new sge_settings_main(ws.get());

    Resize(RESX, RESY);

    //================================

    TextureAtlas::LoadAll();
    DB::Load();

    //================================

    RemoteClient::instance();
    auto clie = [](){
        while(true)
        {
            RemoteClient::instance().Process();
            std::this_thread::sleep_for( std::chrono::milliseconds(50) );
        }
    };
    std::thread th(clie);
    th.detach();

    Sqvm::instance();

    hero = DB::Create( "hero" );
    level.AddCreature( hero, true );

    return true;
}

void GameWindow::Resize(int w, int h)
{
    if(h == 0)
        h = 1;
    Prefecences::Instance()->resolution = glm::vec2(w, h);
    GameWindow::wi->proj = glm::ortho(0.0f, (float)w, (float)h, 0.0f, -1.f, 1.0f);
    glViewport(0,0,w,h);
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
    batch->setUniform(proj);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,1);

    level.Draw(*batch, offset, hero->GetAgent<Creature>()->pos);

    if( Object *o = level.GetObjectByPos(glm::vec3(offset + Mouse::getCursorPos(), 0)/sscale ) )
    {
        //auto oo = DB::Create("foodpolka");
        //o = oo.get();
        //level.SetObjectAtPos(glm::vec3(offset + Mouse::getCursorPos(), 0)/32.f , oo);
        if( Chest *c = o->GetAgent<Chest>() )
        {
            std::stringstream ss;
            ss << "some" << std::endl;
            for( const std::shared_ptr<Object> &o : c->items )
            {
                ss << o->base->id;
                if(auto st = o->GetAgent<Item>())
                    ss << " x" << st->count;
                ss << std::endl;
            }

            batch->drawText(ss.str(), {100,100}, f12.get(), Color::Red);
        }
    }

    if(hero->GetAgent<Creature>()->current_order.type == Order::Follow)
    {
        std::shared_ptr<Object> o = hero->GetAgent<Creature>()->current_order.target.lock();
        if(o)
        {
            glm::vec3 p = o->GetAgent<Creature>()->pos;
            batch->drawQuadAtlas( glm::vec2(p.x, p.y)*sscale - offset, {32,32}, TextureAtlas::tex[0], 1, Color::White);
        }
    }

    if(is_debug)
    {
        batch->drawText(sge::string_format("%d dc UI\n"
                                           "fps %d",
                                           batch->getDc(),
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
    batch->render();

    if(is_debug)
    {
        batch->resetDc();
    }
}

template<int is_debug>
void GameWindow::BaseUpdate()
{
    Mouse::state = Mouse::STATE_MOUSE;
    glfwPollEvents();

    if(Keyboard::isKeyPress(GLFW_KEY_F2))
        wire = !wire;

    if(Keyboard::isKeyPress(GLFW_KEY_F6))
        Prefecences::Instance()->defered_debug = !Prefecences::Instance()->defered_debug;

    if(Keyboard::isKeyPress(GLFW_KEY_F9))
        make_release();
    if(Keyboard::isKeyPress(GLFW_KEY_F10))
        make_debug();
    if(Keyboard::isKeyPress(GLFW_KEY_F11))
        make_spartial();

    if(Keyboard::isKeyPress(GLFW_KEY_F2))
        wire = !wire;
    if(Keyboard::isKeyPress(GLFW_KEY_F4))
        no_ui = !no_ui;

    if(Keyboard::isKeyDown(GLFW_KEY_DOWN))
        level.DeltaCreature(*hero, {0, 0.4f, 0});
    if(Keyboard::isKeyDown(GLFW_KEY_UP))
        level.DeltaCreature(*hero, {0, -0.4f, 0});
    if(Keyboard::isKeyDown(GLFW_KEY_LEFT))
        level.DeltaCreature(*hero, {-0.4f, 0, 0});
    if(Keyboard::isKeyDown(GLFW_KEY_RIGHT))
        level.DeltaCreature(*hero, {0.4f, 0, 0});

    AL::listener = hero->GetAgent<Creature>()->pos;
    level.Update(GameTimer(update_pass));
    offset = glm::vec2(hero->GetAgent<Creature>()->pos)*sscale - glm::vec2(Prefecences::Instance()->resolution)/2.f;
    for(int i = -2; i < 3; i++)
        for(int j = -2; j < 3; j++)
            level.GetSectorByPos(hero->GetAgent<Creature>()->pos + glm::vec3(i*RX,j*RY,0));
    level.KillFar(hero->GetAgent<Creature>()->pos, RX*4);

    if(Mouse::isLeftJustPressed())
    {
        auto selpos = glm::vec3(offset + Mouse::getCursorPos(), 0)/sscale;
        if( Object *o = level.GetObjectByPos(selpos) )
        {
            if(SimpleInteract *si = o->base->GetAgent<SimpleInteract>())
            {
                si->onInteract(o, &level, selpos, gt);
                level.SetObjectAtPos(selpos, DB::Create(si->afterid));
            }
        }
    }

    if(Mouse::isRightJustPressed())
    {
        auto selpos = glm::vec3(offset + Mouse::getCursorPos(), 0)/sscale;
        level.DamageBlock(selpos, 30, gt);
        std::list<std::weak_ptr<Object>> o = level.GetObjectsInRange(selpos, 1.1);
        if(!o.empty())
        {
            hero->GetAgent<Creature>()->current_order.target = o.begin()->lock();
            hero->GetAgent<Creature>()->current_order.type = Order::Follow;
        }
    }

    linfo->UpdateLevelInfo(level);

    static MouseState s;
    ws->Update(gt, s);

    Mouse::dropState();
}
