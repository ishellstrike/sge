/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "sge_texlab_toolbox.h"
#include "sge_ui/treeview.h"

sge_texlab_toolbox::sge_texlab_toolbox(WContainer *par) :
    Win(par)
{
    text = "toolbox";

    lc = new ListContainer(this);

    auto t = new Label(lc);
    t->text("renderbox");
    t->onMouseClick.connect( [=](const ClickHandler&)->bool{ new sge_texlab_renderbox(par); return true;});

    t = new Label(lc);
    t->text("rgb to luminance");
    t->onMouseClick.connect( [=](const ClickHandler&)->bool{ new sge_texlab_rgb_to_luminance(par); return true;});

    t = new Label(lc);
    t->text("heightmap");
    t->onMouseClick.connect( [=](const ClickHandler&)->bool{ new sge_texlab_heightmap(par); return true;});

    t = new Label(lc);
    t->text("float selector");
    t->onMouseClick.connect( [=](const ClickHandler&)->bool{ new sge_texlab_float_selector(par); return true;});

    t = new Label(lc);
    t->text("lerp by map");
    t->onMouseClick.connect( [=](const ClickHandler&)->bool{ new sge_texlab_lerp_by_map(par); return true;});

    t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);t = new Label(lc);

    resizable = true;
    //lc->hidden = true;

    tree = new TreeView(this);
}

void sge_texlab_toolbox::Draw() const
{
    Win::Draw();
}

void sge_texlab_toolbox::Update(const GameTimer &gt, const MouseState &ms)
{
    lc->size = size - glm::vec2(0,20);
    tree->size = size - glm::vec2(0,20);
    Win::Update(gt, ms);
}

