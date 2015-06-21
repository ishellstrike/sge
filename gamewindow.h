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
#include "geometry/mesh.h"
#include "camera.h"
#include "gbuffer.h"

class GameWindow {
public:
    void Mainloop();
    bool BaseInit();
    void BaseUpdate();
    void BaseDraw();

    GameTimer gt;
    FPSCounter fps;

    static void Resize(int w, int h);

    void SetTitle(const std::string &str);

    GLFWmonitor *monitor = nullptr;
    GLFWwindow *window = nullptr;
    TextureAtlas atlas;
    std::shared_ptr<GBuffer> gb;

    glm::mat4 view = glm::mat4(1), proj = glm::mat4(1), proj_per = glm::mat4(1), model = glm::mat4(1);
    static GameWindow *wi;

    GameWindow();
    ~GameWindow();

    std::shared_ptr<BasicJargShader> basic;
    std::shared_ptr<SpriteBatch> batch;

    std::shared_ptr<Mesh> m;
    bool wire = true;
    Camera cam;

    std::shared_ptr<Font> f12;
};

#endif // GAMEWINDOW_H
