#include "sge_level_debug_info.h"

sge_level_debug_info::sge_level_debug_info(WContainer *par) :
    Win(par)
{
    text = "sge_level_debug_info";
    size = {200,220};
    pos = {200,0};
    fps1 = new Label(this);
    fps1->pos = {10,10};
}

void sge_level_debug_info::Draw() const
{
    Win::Draw();
}

void sge_level_debug_info::Update(const GameTimer &gt, const MouseState &ms)
{
    Win::Update(gt, ms);
}

void sge_level_debug_info::UpdateLevelInfo(const Level &fps)
{
    fps1->text(sge::string_format("active sector count: %d", fps.map.size()));
}
