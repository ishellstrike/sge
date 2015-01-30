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
#include "mesh.h"

class GameWindow {

    bool BaseInit();
    void BaseUpdate();
    void BaseDraw();

public:
    void Mainloop();

    GameTimer gt;
    FPSCounter fps;

    static void Resize(int w, int h);

    void SetTitle(const std::string &str);

    GLFWmonitor *monitor = nullptr;
    GLFWwindow *window = nullptr;
    TextureAtlas atlas;

    glm::mat4 view = glm::mat4(1), proj = glm::mat4(1), proj_per = glm::mat4(1), model = glm::mat4(1);
    static GameWindow *wi;

    GameWindow();
    ~GameWindow();

    std::shared_ptr<BasicJargShader> basic;
    std::shared_ptr<SpriteBatch> batch;

    Mesh m;

    std::shared_ptr<Font> f12;
};

#endif // GAMEWINDOW_H
