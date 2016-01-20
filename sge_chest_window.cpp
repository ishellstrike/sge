#include "sge_chest_window.h"
#include "core/agents/chest.h"
#include "core/agents/itembase.h"
#include "core/objectstatic.h"
#include "core/agents/stacked.h"

sge_chest_window::sge_chest_window(WContainer *par) :
    Win(par)
{
    lc = new Table(this);
    lc->Init(3);
    resizable = true;
    lc->columns[0] = {"name", 1, 0};

    lc->columns[1] = {"count", 0, 50};

    lc->columns[2] = {"rand", 0, 50};
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
    lc->Clear();
}

void sge_chest_window::Link( std::shared_ptr<Object> &o )
{
    linked = o;
    lc->Clear();
    c = o->GetAgent<Chest>();
    if( c )
    {
        for( const std::shared_ptr<Object> &i : c->items )
        {
            ItemBase *ib = i->base->GetAgent<ItemBase>();
            Item     *it = i->GetAgent<Item>();
            //Label    *l  = new Label(lc);
            //l->text();

            std::vector<boost::any> t;
            t.push_back(ib->name.c_str());
            t.push_back(it->count);
            t.push_back(rand());

            lc->AddRow(t);
        }
    }
}
