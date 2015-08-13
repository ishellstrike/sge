#include "sge_texture_lab.h"
#include "resources/resourcecontroller.h"
#include "sge_ui/dragnumbox.h"
#include "sge_ui/connector.h"

sge_texture_lab::sge_texture_lab(WContainer *par) :
    Win(par)
{
    text = "sge_texture_lab";

    ib = new ImageBox(this);
    ib->tex = Resources::instance()->Get<Texture>("ro3ck");
    auto d = new DragNumbox(this);

    auto b = new Connector(this);
    auto c = new Connector(this);

    b->pos = {100, 100};
    c->pos = {124, 100};
}

void sge_texture_lab::Draw() const
{
    Win::Draw();
}

void sge_texture_lab::Update(const GameTimer &gt)
{
    Win::Update(gt);
}

