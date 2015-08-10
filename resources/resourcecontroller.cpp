#include "resourcecontroller.h"

Resources *Resources::m_inst = nullptr;


void Resources::Init()
{
    auto height_shader = new BasicJargShader;
    height_shader->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/testgen1.glsl");
    height_shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/testgen1.glsl");
    height_shader->Link();
    height_shader->Afterlink();

    auto grad_shader = new BasicJargShader;
    grad_shader->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/gradient_builder.glsl");
    grad_shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/gradient_builder.glsl");
    grad_shader->Link();
    grad_shader->Afterlink();

    Push("height_shader", height_shader);
    Push("grad_shader", grad_shader);

    auto noise_map = new Texture;
    auto grad_map = new Texture;
    noise_map->Load("data/textures/PerlinPerm2D.png");
    grad_map->Load("data/textures/PerlinGrad2D.png");

    Push("noise_map", noise_map);
    Push("grad_map", grad_map);


    auto basic = new BasicJargShader;
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

    auto water = new BasicJargShader;
    water->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal_watertest.glsl");
    water->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal_watertest.glsl");
    water->Link();
    water->Afterlink();

    Push("default_planet_render", basic);
    Push("default_water_render", water);

    auto soil = new Texture;
    auto grass = new Texture;
    auto snow = new Texture;
    auto rock = new Texture;
    soil->Load("data/textures/soil.png", true, true);
    grass->Load("data/textures/grass.png", true, true);
    snow->Load("data/textures/snow.png", true, true);
    rock->Load("data/textures/rock.png", true, true);

    Push("soil", soil);
    Push("grass", grass);
    Push("snow", snow);
    Push("rock", rock);
}
