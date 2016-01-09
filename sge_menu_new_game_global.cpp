#include "sge_menu_new_game_global.h"

sge_menu_new_game_global::sge_menu_new_game_global(WContainer *par) :
    Win(par)
{
}

void sge_menu_new_game_global::Draw() const
{
    Win::Draw();
}

void sge_menu_new_game_global::Update(const GameTimer& gt, const MouseState &ms)
{
    Win::Update(gt, ms);
}
