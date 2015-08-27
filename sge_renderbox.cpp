/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "sge_renderbox.h"
#include "helper.h"

sge_texlab_renderbox::sge_texlab_renderbox(WContainer *par) :
    Win(par)
{
    text = "renderbox";

    ib = new ImageBox(this);
    in = new Connector<std::shared_ptr<Texture>>(this);
    label = new Label(this);
    label->pos = {2, 2};

    resizable = true;
}

void sge_texlab_renderbox::Draw() const
{
    Win::Draw();
}

void sge_texlab_renderbox::Update(const GameTimer &gt)
{
    ib->size = size - glm::vec2(5 + 20, 5 - 20);
    ib->pos = {20, 20};
    if(!in->linked.expired())
    {
        ib->tex = in->linked_value;
        if(ib->tex)
            label->text( string_format("input %dx%d", ib->tex->width, ib->tex->height) );
    } else {
        label->text("no input");
    }

    in->pos = {0, (size/2.f).y - (in->size/2.f).y};

    Win::Update(gt);
}
