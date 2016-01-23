/*******************************************************************************
        Copyright (C) 2016 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "chest.h"
#include <map>
#include "stacked.h"
#include "core/db.h"
#include "functional"
#include "sge_chest_window.h"
#include "gamewindow.h"

void Chest::Deserialize(const rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Chest::Instantiate() const
{
    Chest *c = new Chest();
    for(const auto &i : items)
    {
        c->items.push_back(i->Instantiate());
    }
    return std::unique_ptr<Chest>(c);
}

void Chest::onInteract(std::shared_ptr<Object> &par, Level *, const glm::vec3 &, const GameTimer &)
{
    GameWindow::wi->chest->hidden = false;
    GameWindow::wi->chest->Link(par, GameWindow::Hero);
}

void Chest::Combine()
{
    for( size_t i = 0; i < items.size(); ++i )
        for( size_t j = 0; j < items.size() - i; ++j )
            if( i != j )
            {
                if(items[i] && items[i]->Equals(items[j]))
                {
                    Item *s1 = items[i]->GetAgent<Item>();
                    Item *s2 = items[j]->GetAgent<Item>();
                    if(!s1)
                    {
                        LOG(error) << "item " << items[i]->base->id << " has no ItemBase! All items must have it";
                        continue;
                    }
                    if(!s2)
                    {
                        LOG(error) << "item " << items[j]->base->id << " has no ItemBase! All items must have it";
                        continue;
                    }
                    s2->count += s1->count;
                    items[i] = nullptr;
                }
            }

    std::vector<std::shared_ptr<Object>> updated;
    for( size_t i = 0; i < items.size(); ++i )
    {
        if( items[i] != nullptr )
            updated.push_back(std::move(items[i]));
    }

    items.clear();
    items = updated;
}
