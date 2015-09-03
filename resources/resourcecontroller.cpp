#include "resourcecontroller.h"

Resources *Resources::m_inst = nullptr;


void Resources::Init()
{
    const auto &error = new Texture;
    error->Load("/123123123error.png");
    PUSH_NVP(error);


    const auto & height_shader = new BasicJargShader;
    height_shader->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/testgen1.glsl");
    height_shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/testgen1.glsl");
    height_shader->Link();
    height_shader->Afterlink();
    PUSH_NVP(height_shader);

    const auto & grad_shader = new BasicJargShader;
    grad_shader->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/gradient_builder.glsl");
    grad_shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/gradient_builder.glsl");
    grad_shader->Link();
    grad_shader->Afterlink();
    PUSH_NVP(grad_shader);

    const auto & noise_map = new Texture;
    const auto & grad_map = new Texture;
    noise_map->Load("data/textures/PerlinPerm2D.png");
    grad_map->Load("data/textures/PerlinGrad2D.png");
    PUSH_NVP(noise_map);
    PUSH_NVP(grad_map);


    const auto & default_planet_render_nowire = new BasicJargShader;
    default_planet_render_nowire->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal.glsl");
    default_planet_render_nowire->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal.glsl");
    default_planet_render_nowire->loadShaderFromSource(GL_GEOMETRY_SHADER, "data/shaders/minimal.glsl");
    default_planet_render_nowire->Link();
    default_planet_render_nowire->Afterlink();
    PUSH_NVP(default_planet_render_nowire);

    const auto & default_planet_render_wire = new BasicJargShader;
    default_planet_render_wire->AddDefine("WIREFRAME");
    default_planet_render_wire->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal.glsl");
    default_planet_render_wire->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal.glsl");
    default_planet_render_wire->loadShaderFromSource(GL_GEOMETRY_SHADER, "data/shaders/minimal.glsl");
    default_planet_render_wire->Link();
    default_planet_render_wire->Afterlink();
    PUSH_NVP(default_planet_render_wire);

    const auto & default_water_render = new BasicJargShader;
    default_water_render->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/minimal_watertest.glsl");
    default_water_render->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/minimal_watertest.glsl");
    default_water_render->Link();
    default_water_render->Afterlink();
    PUSH_NVP(default_water_render);

    const auto & defered = new BasicJargShader;
    defered->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/defered.glsl");
    defered->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/defered.glsl");
    defered->Link();
    defered->Afterlink();
    defered->SetUniform("buffer_world_pos", 0);
    defered->SetUniform("buffer_diffuse", 1);
    defered->SetUniform("buffer_normal", 2);
    defered->SetUniform("buffer_tex_coord", 3);
    PUSH_NVP(defered);

    const auto & soil = new Texture;
    const auto & grass = new Texture;
    const auto & snow = new Texture;
    const auto & rock = new Texture;
    soil->Load("data/textures/soil.png", true, true);
    grass->Load("data/textures/grass.png", true, true);
    snow->Load("data/textures/snow.png", true, true);
    rock->Load("data/textures/rock.png", true, true);
    PUSH_NVP(soil);
    PUSH_NVP(grass);
    PUSH_NVP(snow);
    PUSH_NVP(rock);

    const auto & rgb_to_luminance = new BasicJargShader;
    rgb_to_luminance->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/rgb_to_luminance.glsl");
    rgb_to_luminance->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/rgb_to_luminance.glsl");
    rgb_to_luminance->Link();
    rgb_to_luminance->Afterlink();
    PUSH_NVP(rgb_to_luminance);

    const auto & lerp_rgb_map = new BasicJargShader;
    lerp_rgb_map->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/lerp_rgb_map.glsl");
    lerp_rgb_map->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/lerp_rgb_map.glsl");
    lerp_rgb_map->Link();
    lerp_rgb_map->Afterlink();
    PUSH_NVP(lerp_rgb_map);

    const auto & starnest = new BasicJargShader;
    starnest->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/starnest.glsl");
    starnest->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/starnest.glsl");
    starnest->Link();
    starnest->Afterlink();
    PUSH_NVP(starnest);

    const auto & corona = new BasicJargShader;
    corona->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/corona.glsl");
    corona->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/corona.glsl");
    corona->Link();
    corona->Afterlink();
    corona->SetUniform("samplerPerlinPerm2D", 0);
    PUSH_NVP(corona);

    const auto & extract_glow = new Shader;
    extract_glow->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/extract_glow.glsl");
    extract_glow->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/extract_glow.glsl");
    extract_glow->Link();
    extract_glow->Use();
    extract_glow->SetUniform("mainMap", 0);
    PUSH_NVP(extract_glow);

    const auto & tone_maping = new Shader;
    tone_maping->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/tone_maping.glsl");
    tone_maping->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/tone_maping.glsl");
    tone_maping->Link();
    tone_maping->Use();
    tone_maping->SetUniform("mainMap", 0);
    tone_maping->SetUniform("blurMap", 1);
    PUSH_NVP(tone_maping);

    const auto & gausian_blur = new Shader;
    gausian_blur->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/gausian_blur.glsl");
    gausian_blur->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/gausian_blur.glsl");
    gausian_blur->Link();
    gausian_blur->Use();
    gausian_blur->SetUniform("mainMap", 0);
    PUSH_NVP(gausian_blur);

    const auto & gausian_blur2 = new Shader;
    gausian_blur2->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/gausian_blur2.glsl");
    gausian_blur2->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/gausian_blur2.glsl");
    gausian_blur2->Link();
    gausian_blur2->Use();
    gausian_blur2->SetUniform("mainMap", 0);
    PUSH_NVP(gausian_blur2);


    const auto & basic = new BasicJargShader;
    basic->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/basic.glsl");
    basic->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/basic.glsl");
    basic->Link();
    basic->Afterlink();
    basic->Use();
    basic->SetUniform("material_texture", 0);
    PUSH_NVP(basic);

    const auto & font = new BasicJargShader;
    font->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/font.glsl");
    font->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/font.glsl");
    font->Link();
    font->Afterlink();
    font->Use();
    font->SetUniform("material_texture", 0);
    PUSH_NVP(font);

    const auto & color = new BasicJargShader;
    color->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/color.glsl");
    color->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/color.glsl");
    color->Link();
    color->Afterlink();
    PUSH_NVP(color);

    const auto & fake_planet = new BasicJargShader;
    fake_planet->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/fake_planet.glsl");
    fake_planet->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/fake_planet.glsl");
    fake_planet->Link();
    fake_planet->Afterlink();
    PUSH_NVP(fake_planet);

    const auto & planet_ring = new BasicJargShader;
    planet_ring->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/planet_ring.glsl");
    planet_ring->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/planet_ring.glsl");
    planet_ring->Link();
    planet_ring->Afterlink();
    PUSH_NVP(planet_ring);
}
