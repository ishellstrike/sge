#include "resourcecontroller.h"
#include "prefecences.h"

Resources *Resources::m_inst = nullptr;


void Resources::Init()
{
    const auto &error = new Texture;
    error->Load("/123123123error.png");
    PUSH_NVP(error);

    auto t_dir = Prefecences::Instance()->getTexturesDir();
    auto s_dir = Prefecences::Instance()->getShadersDir();

    const auto & noise_map = new Texture;
    const auto & grad_map = new Texture;
    noise_map->Load("data/textures/PerlinPerm2D.png");
    grad_map->Load("data/textures/PerlinGrad2D.png");
    PUSH_NVP(noise_map);
    PUSH_NVP(grad_map);

    const auto &cur_mouse = new Texture;
    cur_mouse->Load(t_dir + "cur_mouse.png");
    PUSH_NVP(cur_mouse);

    const auto &cur_resize = new Texture;
    cur_resize->Load(t_dir + "cur_resize.png");
    PUSH_NVP(cur_resize);

    const auto &grass = new Texture;
    grass->Load(t_dir + "grass.png");
    PUSH_NVP(grass);


    const auto & basic = new BasicJargShader;
    basic->loadShaderFromSource(GL_VERTEX_SHADER,   s_dir + "basic.glsl");
    basic->loadShaderFromSource(GL_FRAGMENT_SHADER, s_dir + "basic.glsl");
    basic->Link();
    basic->Afterlink();
    basic->Use();
    basic->SetUniform("material_texture", 0);
    PUSH_NVP(basic);

    const auto & defered_jarg = new BasicJargShader;
    defered_jarg->loadShaderFromSource(GL_VERTEX_SHADER,   s_dir + "defered_jarg.glsl");
    defered_jarg->loadShaderFromSource(GL_FRAGMENT_SHADER, s_dir + "defered_jarg.glsl");
    defered_jarg->Link();
    defered_jarg->Afterlink();
    defered_jarg->Use();
    defered_jarg->SetUniform("diffuse_map", 0);
    defered_jarg->SetUniform("normal_map", 1);
    defered_jarg->SetUniform("outline_map", 2);
    PUSH_NVP(defered_jarg);

    const auto & defered = new BasicJargShader;
    defered->loadShaderFromSource(GL_VERTEX_SHADER,   s_dir + "defered.glsl");
    defered->loadShaderFromSource(GL_FRAGMENT_SHADER, s_dir + "defered.glsl");
    defered->Link();
    defered->Afterlink();
    defered->SetUniform("buffer_world_pos", 0);
    defered->SetUniform("buffer_diffuse", 1);
    defered->SetUniform("buffer_normal", 2);
    defered->SetUniform("buffer_tex_coord", 3);
    PUSH_NVP(defered);

    const auto & font = new BasicJargShader;
    font->loadShaderFromSource(GL_VERTEX_SHADER,   s_dir + "font.glsl");
    font->loadShaderFromSource(GL_FRAGMENT_SHADER, s_dir + "font.glsl");
    font->Link();
    font->Afterlink();
    font->Use();
    font->SetUniform("material_texture", 0);
    PUSH_NVP(font);

    const auto & color = new BasicJargShader;
    color->loadShaderFromSource(GL_VERTEX_SHADER,   s_dir + "color.glsl");
    color->loadShaderFromSource(GL_FRAGMENT_SHADER, s_dir + "color.glsl");
    color->Link();
    color->Afterlink();
    PUSH_NVP(color);
}
