#include "sge_chest_window.h"
#include "core/agents/chest.h"
#include "core/agents/itembase.h"
#include "core/objectstatic.h"
#include "core/agents/stacked.h"
#include "keyboard.h"
#include "keyconfig.h"

bool sge_chest_window::TakeSelected()
{
    if(!linked.expired() && !hero.expired())
    {
        Chest *h = hero.lock()->GetAgent<Chest>();
        Chest *c = linked.lock()->GetAgent<Chest>();
        if(c->items.empty() || c->items.size() <= lc->selected)
            return false;

        std::shared_ptr<Object> o = boost::any_cast<std::shared_ptr<Object>>(lc->table[lc->selected][2]);
        h->items.push_back(o);

        for(int i = 0; i < c->items.size(); ++i)
        {
            if(c->items[i] == o)
            {
                c->items[i] = *(--c->items.end());
                c->items.pop_back();
                break;
            }
        }
    }

    Link(linked.lock(), hero.lock());
    return true;
}

bool sge_chest_window::TakeAll()
{
    if(!linked.expired() && !hero.expired())
    {
        Chest *h = hero.lock()->GetAgent<Chest>();
        Chest *c = linked.lock()->GetAgent<Chest>();
        for(auto &i : c->items)
            h->items.push_back(std::move(i));

        c->items.clear();
    }

    Link(linked.lock(), hero.lock());
    return true;
}

sge_chest_window::sge_chest_window(WContainer *par) :
    Win(par)
{
    lc = new Table(this);
    lc->Init(3);
    resizable = true;
    size = glm::vec2(200, 200);

    lc->columns[0] = {"name", 1, 0};

    lc->columns[1] = {"count", 0, 70};

    take = new Button(this);
    take->text = "Take";
    take->size = glm::vec2(80, 30);
    take->onMouseClick.connect( [&](const ClickHandler &mh)->bool{
        if(mh.button == GLFW_MOUSE_BUTTON_LEFT)
        {
            return TakeSelected();
        }
        return false;
    });

    takeall = new Button(this);
    takeall->text = "Take a11";
    takeall->size = glm::vec2(80, 30);
    takeall->onMouseClick.connect( [&](const ClickHandler &mh)->bool{
        if(mh.button == GLFW_MOUSE_BUTTON_LEFT)
        {
            return TakeAll();
        }
        return false;
    });
}

void sge_chest_window::Draw() const
{
    Win::Draw();
}

void sge_chest_window::Update(const GameTimer& gt, const MouseState &ms)
{
    lc->size = size - glm::vec2(20, 50);
    take->pos = glm::vec2(0, size.y - 50);
    takeall->pos = glm::vec2(80, size.y - 50);

    if(Keyboard::isKeyPress(Keybind::GetBind(Keybind::ACT_TAKE_ALL)))
        TakeAll();

    Win::Update(gt, ms);
}

void sge_chest_window::Unlink()
{
    c = nullptr;
    linked.reset();
    hero.reset();
    lc->Clear();
}

void sge_chest_window::Link( std::shared_ptr<Object> &o, std::shared_ptr<Object> &h )
{
    hero = h;
    linked = o;
    lc->Clear();
    c = o->GetAgent<Chest>();
    if( c )
    {
        for( std::shared_ptr<Object> &i : c->items )
        {
            ItemBase *ib = i->base->GetAgent<ItemBase>();
            Item     *it = i->GetAgent<Item>();
            //Label    *l  = new Label(lc);
            //l->text();

            std::vector<boost::any> t;
            t.push_back(ib->name.c_str());
            t.push_back(it->count);
            t.push_back(i);

            lc->AddRow(t);
        }
    }
}
