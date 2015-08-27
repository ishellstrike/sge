/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "sge_texture_lab.h"
#include "resources/resourcecontroller.h"
#include "sge_ui/dragnumbox.h"
#include "sge_ui/connector.h"
#include "TextureGenerator.h"

sge_texlab_heightmap::sge_texlab_heightmap(WContainer *par) :
    Win(par)
{
    text = "sge_texture_lab";

    ib = new ImageBox(this);
    ib->pos = {40,20};


    scale = new Connector<float>(this);
    scale->name = "scale";
    scale->pos = {0, 20};

    offsetx = new Connector<float>(this);
    offsetx->name = "x";
    offsetx->pos = {0, 60};

    offsety = new Connector<float>(this);
    offsety->name = "y";
    offsety->pos = {0, 100};

    out = new Connector<std::shared_ptr<Texture>>(this);
    out->pos = {155, 20};
    out->name = "out";
    out->passive = false;

    size = {180,150};
    resizable = false;
}

void sge_texlab_heightmap::Draw() const
{
    Win::Draw();
}

void sge_texlab_heightmap::Update(const GameTimer &gt)
{
    std::shared_ptr<Texture> textu = std::make_shared<Texture>(glm::vec2{128,128},true);
    tg.SetShader(Resources::instance()->Get<BasicJargShader>("height_shader"));
    tg.Reset();
    tg.AddTexture("samplerPerlinPerm2D", Resources::instance()->Get<Texture>("noise_map"));
    tg.AddTexture("samplerPerlinGrad2D", Resources::instance()->Get<Texture>("grad_map"));
    tg.SetParams(offsetx->linked_value);
    tg.SetParams(offsety->linked_value);
    tg.SetParams(scale->linked_value);
    tg.SetResultTexture(textu);
    tg.RenderOnTempFbo();

    ib->tex = textu;
    out->linked_value = ib->tex;

    Win::Update(gt);
}

