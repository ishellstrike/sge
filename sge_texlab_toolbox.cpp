#include "sge_texlab_toolbox.h"

sge_texlab_toolbox::sge_texlab_toolbox(WContainer *par) :
    Win(par)
{
    text = "toolbox";

    lc = new ListContainer(this);

    auto t = new Label(lc);
    t->text("renderbox");
    t->onLeftPress = [=](){ new sge_texlab_renderbox(par); };

    t = new Label(lc);
    t->text("rgb to luminance");
    t->onLeftPress = [=](){ new sge_texlab_rgb_to_luminance(par); };

    t = new Label(lc);
    t->text("heightmap");
    t->onLeftPress = [=](){ new sge_texlab_heightmap(par); };

    t = new Label(lc);
    t->text("float selector");
    t->onLeftPress = [=](){ new sge_texlab_float_selector(par); };

    t = new Label(lc);
    t->text("lerp by map");
    t->onLeftPress = [=](){ new sge_texlab_lerp_by_map(par); };

    resizable = true;
}

void sge_texlab_toolbox::Draw() const
{
    Win::Draw();
}

void sge_texlab_toolbox::Update(const GameTimer &gt)
{
    lc->size = size - glm::vec2(0,20);
    Win::Update(gt);
}

