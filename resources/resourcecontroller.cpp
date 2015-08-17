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
    //basic->AddExtension("GL_ARB_tessellation_shader");
    basic->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal.glsl");
    basic->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal.glsl");
    //basic->loadShaderFromSource(GL_TESS_CONTROL_SHADER, "data/shaders/minimal.glsl");
    //basic->loadShaderFromSource(GL_TESS_EVALUATION_SHADER, "data/shaders/minimal.glsl");
    basic->loadShaderFromSource(GL_GEOMETRY_SHADER, "data/shaders/minimal.glsl");
    basic->Link();
    basic->Afterlink();
    basic->locateVar("R");
    basic->locateVar("s");

    const auto & water = new BasicJargShader;
    water->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal_watertest.glsl");
    water->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal_watertest.glsl");
    water->Link();
    water->Afterlink();

    Push("default_planet_render", basic);
    Push("default_water_render", water);

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
}
