#include "resourcecontroller.h"

Resources *Resources::m_inst = nullptr;


void Resources::Init()
{
    const auto &error = new Texture;
    error->Load("/123123123error.png");
    Push("error", error);


    const auto & height_shader = new BasicJargShader;
    height_shader->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/testgen1.glsl");
    height_shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/testgen1.glsl");
    height_shader->Link();
    height_shader->Afterlink();

    const auto & grad_shader = new BasicJargShader;
    grad_shader->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/gradient_builder.glsl");
    grad_shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/gradient_builder.glsl");
    grad_shader->Link();
    grad_shader->Afterlink();

    Push("height_shader", height_shader);
    Push("grad_shader", grad_shader);

    const auto & noise_map = new Texture;
    const auto & grad_map = new Texture;
    noise_map->Load("data/textures/PerlinPerm2D.png");
    grad_map->Load("data/textures/PerlinGrad2D.png");

    Push("noise_map", noise_map);
    Push("grad_map", grad_map);


    const auto & basic = new BasicJargShader;
    basic->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal.glsl");
    basic->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal.glsl");
    basic->loadShaderFromSource(GL_GEOMETRY_SHADER, "data/shaders/minimal.glsl");
    basic->Link();
    basic->Afterlink();

    const auto & water = new BasicJargShader;
    water->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal_watertest.glsl");
    water->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal_watertest.glsl");
    water->Link();
    water->Afterlink();

    const auto & defered = new BasicJargShader;
    defered->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/defered.glsl");
    defered->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/defered.glsl");
    defered->Link();
    defered->Afterlink();

    defered->SetUniform("buffer_world_pos", 0);
    defered->SetUniform("buffer_diffuse", 1);
    defered->SetUniform("buffer_normal", 2);
    defered->SetUniform("buffer_tex_coord", 3);

    Push("default_planet_render", basic);
    Push("default_water_render", water);
    Push("defered", defered);

    const auto & soil = new Texture;
    const auto & grass = new Texture;
    const auto & snow = new Texture;
    const auto & rock = new Texture;
    soil->Load("data/textures/soil.png", true, true);
    grass->Load("data/textures/grass.png", true, true);
    snow->Load("data/textures/snow.png", true, true);
    rock->Load("data/textures/rock.png", true, true);

    Push("soil", soil);
    Push("grass", grass);
    Push("snow", snow);
    Push("rock", rock);

    const auto & rgb_to_luminance = new BasicJargShader;
    rgb_to_luminance->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/rgb_to_luminance.glsl");
    rgb_to_luminance->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/rgb_to_luminance.glsl");
    rgb_to_luminance->Link();
    rgb_to_luminance->Afterlink();

    Push("rgb_to_luminance", rgb_to_luminance);

    const auto & lerp_rgb_map = new BasicJargShader;
    lerp_rgb_map->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/lerp_rgb_map.glsl");
    lerp_rgb_map->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/lerp_rgb_map.glsl");
    lerp_rgb_map->Link();
    lerp_rgb_map->Afterlink();

    Push("lerp_rgb_map", lerp_rgb_map);

    const auto & starnest = new BasicJargShader;
    starnest->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/starnest.glsl");
    starnest->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/starnest.glsl");
    starnest->Link();
    starnest->Afterlink();

    Push("starnest", starnest);

    const auto & corona = new BasicJargShader;
    corona->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/corona.glsl");
    corona->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/corona.glsl");
    corona->Link();
    corona->Afterlink();
    corona->SetUniform("samplerPerlinPerm2D", 0);

    Push("corona", corona);

    const auto & extract_glow = new Shader;
    extract_glow->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/extract_glow.glsl");
    extract_glow->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/extract_glow.glsl");
    extract_glow->Link();
    extract_glow->Use();
    extract_glow->SetUniform("mainMap", 0);

    Push("extract_glow", extract_glow);

    const auto & tone_maping = new Shader;
    tone_maping->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/tone_maping.glsl");
    tone_maping->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/tone_maping.glsl");
    tone_maping->Link();
    tone_maping->Use();
    tone_maping->SetUniform("mainMap", 0);
    tone_maping->SetUniform("blurMap", 1);

    Push("tone_maping", tone_maping);

    const auto & gausian_blur = new Shader;
    gausian_blur->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/gausian_blur.glsl");
    gausian_blur->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/gausian_blur.glsl");
    gausian_blur->Link();
    gausian_blur->Use();
    gausian_blur->SetUniform("mainMap", 0);

    Push("gausian_blur", gausian_blur);
}
