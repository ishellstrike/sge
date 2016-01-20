#include "sge_inventory.h"

sge_inventory::sge_inventory(WContainer *par) :
    Win(par)
{
}

void sge_inventory::Draw() const
{
    Win::Draw();
}

void sge_inventory::Update(const GameTimer& gt, const MouseState &ms)
{
    Win::Update(gt, ms);
}


