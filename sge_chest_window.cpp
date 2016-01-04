#include "sge_chest_window.h"
#include "core/agents/chest.h"
#include "core/agents/itembase.h"
#include "core/objectstatic.h"
#include "core/agents/stacked.h"

sge_chest_window::sge_chest_window(WContainer *par) :
    Win(par)
{
    lc = new ListContainer(this);
}

void sge_chest_window::Draw() const
{
    Win::Draw();
}

void sge_chest_window::Update(const GameTimer& gt, const MouseState &ms)
{
    lc->size = size - glm::vec2(0,20);
    Win::Update(gt, ms);
}

void sge_chest_window::Unlink()
{
    c = nullptr;
    linked.reset();
    lc->ItemsClear();
}

void sge_chest_window::Link( std::shared_ptr<Object> &o )
{
    linked = o;
    lc->ItemsClear();
    c = o->GetAgent<Chest>();
    if( c )
    {
        for( const std::shared_ptr<Object> &i : c->items )
        {
            ItemBase *ib = i->base->GetAgent<ItemBase>();
            Item     *it = i->GetAgent<Item>();
            Label    *l  = new Label(lc);
            l->text(sge::string_format("%s x%d", ib->name.c_str(), it->count));
        }
    }
}
