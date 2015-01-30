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

class GameWindow {

    bool BaseInit();
    void BaseUpdate();
    void BaseDraw();

public:
    virtual void Draw() = 0;
    virtual void Update() = 0;
    virtual void Init() = 0;
    void Mainloop();

    GameTimer gt;
    FPSCounter fps;

    static void Resize(int w, int h);

    void SetTitle(const std::string &str);

    GLFWmonitor *monitor = nullptr;
    GLFWwindow *window = nullptr;
    TextureAtlas atlas;

    glm::mat4 view, proj, model;
    static GameWindow *wi;

    GameWindow();
    ~GameWindow();

    std::shared_ptr<JargShader> basic, font;
    std::shared_ptr<SpriteBatch> batch;
};

#endif // GAMEWINDOW_H
