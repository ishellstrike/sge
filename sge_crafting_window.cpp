#include "sge_crafting_window.h"
#include "core/db.h"
#include "sge_ui/label.h"
#include "core/agents/itembase.h"

sge_crafting_window::sge_crafting_window(WContainer *par) :
    Win(par)
{

    info = new Label(this);
    lc = new ListContainer(this);
    resizable = true;

    Rebuild();
}

void sge_crafting_window::Draw() const
{
    Win::Draw();
}



void sge_crafting_window::Update(const GameTimer &gt, const MouseState &ms)
{
    lc->size = glm::vec2(size.x/2, size.y - 20);
    info->pos = glm::vec2(size.x/2 + 5, 10);

    Win::Update(gt, ms);
}

std::string get_recipepart_name(RecipePart &rp)
{
    if(rp.type == RecipePart::Item)
    {
        std::stringstream s;
        s << DB::Get(rp.value)->GetAgent<ItemBase>()->name << " x" << rp.count << "\n";
        return s.str();
    }

    if(rp.type == RecipePart::Tool)
    {
        std::stringstream s;
        s << "tool " << rp.value << " minimum level " << rp.level << "\n";
        return s.str();
    }
}

void sge_crafting_window::Select(const std::shared_ptr<Recipe> &r)
{
    selected = r;

    if(r == nullptr)
        info->text("");

    std::stringstream ss;

    ss << r->name << "\n";
    if(!r->category.empty())
        ss << "|6495ED" << r->category << "|000000" <<"\n";
    if(!r->descr.empty())
        ss << r->descr << "\n";

    ss << "Require:\n";
    for(size_t i = 0; i < r->recipe.size(); ++i)
    {
        if(r->recipe[i].size() > 1)
        {
            ss << get_recipepart_name(r->recipe[i][0]);
            for(size_t j = 1; j < r->recipe[i].size(); ++j)
                ss << "OR\n" << get_recipepart_name(r->recipe[i][0]);
        }
        else
            ss << get_recipepart_name(r->recipe[i][0]);
    }

    info->text(ss.str());
}

void sge_crafting_window::Rebuild()
{
    lc->Clear();
    by_category.clear();
    for(const std::shared_ptr<Recipe> &a : DB::recipe_db)
    {
       by_category[a->category].push_back(a);
    }

    for(const std::pair<std::string, std::vector<std::shared_ptr<Recipe>>> &a : by_category)
    {
        Label *l = new Label(lc);
        if(a.first.empty())
            l->text("|6495EDOther:");
        else
            l->text(std::string("|6495ED")+a.first+":");

        for(const std::shared_ptr<Recipe> &b : a.second)
        {
            Label *l = new Label(lc);
            l->text(std::string(" ")+b->name);
            l->tag = b;
            l->onMouseClick.connect([&, b](const ClickHandler &ch)->bool{
                if(ch.button == GLFW_MOUSE_BUTTON_LEFT)
                {
                    Select(b);
                    return true;
                }
                return false;
            }
            );
        }
    }

    Select((*by_category.begin()).second[0]);
}


