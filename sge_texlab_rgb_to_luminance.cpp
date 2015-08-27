/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "sge_texlab_rgb_to_luminance.h"
#include "TextureGenerator.h"
#include "basicjargshader.h"
#include "resources/resourcecontroller.h"

sge_texlab_rgb_to_luminance::sge_texlab_rgb_to_luminance(WContainer *par) :
    Win(par)
{
    text = "rgb to luminance";

    ib = new ImageBox(this);
    ib->pos = {40,20};


    in = new Connector<std::shared_ptr<Texture>>(this);
    in->name = "in";
    in->pos = {0, 20};

    out = new Connector<std::shared_ptr<Texture>>(this);
    out->pos = {155, 20};
    out->name = "out";
    out->passive = false;

    size = {180,150};
    resizable = false;
}

void sge_texlab_rgb_to_luminance::Draw() const
{
    Win::Draw();
}

void sge_texlab_rgb_to_luminance::Update(const GameTimer &gt)
{
    if(!in->linked.expired())
    {
        std::shared_ptr<Texture> textu = std::make_shared<Texture>(glm::vec2{128,128},true);
        tg.SetShader(Resources::instance()->Get<BasicJargShader>("rgb_to_luminance"));
        tg.Reset();
        if(in->linked_value)
            tg.AddTexture("rgb_map", in->linked_value);
        tg.SetResultTexture(textu);
        tg.RenderOnTempFbo();

        ib->tex = textu;
    }

    out->linked_value = ib->tex;

    Win::Update(gt);
}
