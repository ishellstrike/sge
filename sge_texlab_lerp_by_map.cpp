/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "sge_texlab_lerp_by_map.h"
#include "resources/resourcecontroller.h"

sge_texlab_lerp_by_map::sge_texlab_lerp_by_map(WContainer *par) :
    Win(par)
{
    text = "lerp by map";

    ib = new ImageBox(this);
    ib->pos = {40,20};

    map1 = new Connector<std::shared_ptr<Texture>>(this);
    map1->name = "map 1";
    map1->pos = {0, 20};

    map2 = new Connector<std::shared_ptr<Texture>>(this);
    map2->name = "map 2";
    map2->pos = {0, 60};

    lerp = new Connector<std::shared_ptr<Texture>>(this);
    lerp->name = "lerp";
    lerp->pos = {0, 100};

    out = new Connector<std::shared_ptr<Texture>>(this);
    out->pos = {155, 20};
    out->name = "out";
    out->passive = false;

    size = {180,150};
    resizable = false;
}

void sge_texlab_lerp_by_map::Draw() const
{
    Win::Draw();
}

void sge_texlab_lerp_by_map::Update(const GameTimer &gt, const MouseState &ms)
{
    if(!map1->linked.expired() && !map2->linked.expired() && !lerp->linked.expired())
    {
        std::shared_ptr<Texture> textu = std::make_shared<Texture>(glm::vec2{128,128},true);
        tg.SetShader(Resources::instance()->Get<BasicJargShader>("lerp_rgb_map"));
        tg.Reset();
        if(map1->linked_value)
            tg.AddTexture("rgb_map", map1->linked_value);
        if(map2->linked_value)
            tg.AddTexture("rgb_map2", map2->linked_value);
        if(lerp->linked_value)
            tg.AddTexture("lerp_map", lerp->linked_value);
        tg.SetResultTexture(textu);
        tg.RenderOnTempFbo();

        ib->tex = textu;
    }

    out->linked_value = ib->tex;

    Win::Update(gt, ms);
}
