#include "chest.h"
#include <map>
#include "stacked.h"
#include "core/db.h"
#include "functional"

void Chest::Deserialize(rapidjson::Value &val)
{

}

std::shared_ptr<Agent> Chest::Instantiate() const
{
    Chest *c = new Chest();
    return std::unique_ptr<Chest>(c);
}

void Chest::Combine()
{
    for( int i = 0; i < items.size(); ++i )
        for( int j = 0; j < items.size() - i; ++j )
            if( i != j )
            {
                if(items[i] && items[i]->Equals(items[j].get()))
                {
                    Stacked *s1 = items[i]->GetAgent<Stacked>();
                    Stacked *s2 = items[j]->GetAgent<Stacked>();
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
    for( int i = 0; i < items.size(); ++i )
    {
        if( items[i] != nullptr )
            updated.push_back(std::move(items[i]));
    }

    items.clear();
    items = updated;
}
