#include "itemspawner.h"
#include "logger.h"
#include "random.h"
#include "core/objecthelper.h"
#include "core/agents/chest.h"
#include "core/agents/stacked.h"
#include "core/db.h"

/**
*\brief Агент, осуществляющий генерацию предметов при создании блоков-контейнеров
*
*   Example:
*   {
*       "type":"ItemSpawner",
*       "items":[
*           [<string id>, <int low_quantity>, <int high_quantity>, <double probability>],
*           ["foo_block", 1, 10, 0.4]
*       ]
*   }
*/
void ItemSpawner::Deserialize(rapidjson::Value &val)
{
    if(val.HasMember("items"))
    {
        rapidjson::Value &it = val["items"];
        for(decltype(it.Size()) i=0; i < it.Size(); i++)
        {
            rapidjson::Value &p = it[i];

            SpawnInfo si;

            si.id            = p.Begin()->GetString();
            si.low_quantity  = p[1].GetInt();
            si.high_quantity = p[2].GetInt();
            si.probability   = p[3].GetDouble();

            items.push_back(si);
        }
    }
}

std::unique_ptr<Agent> ItemSpawner::Instantiate()
{
    ItemSpawner *i = new ItemSpawner();
    return std::unique_ptr<ItemSpawner>(i);
}

void ItemSpawner::onInit(ObjectHelper *par)
{
    auto ch = par->GetAgent<Chest>();
    if(ch)
    {
        for(const auto &i : items)
        {
            if(random::next<float>() < i.probability)
            {
                int count = rand()%(i.high_quantity - i.low_quantity) + i.low_quantity;
                if(count == 0)
                    continue;

                auto item = DB::Create(i.id);

                if(count > 1)
                {
                    Stacked *stack = item->GetAgent<Stacked>();
                    if(stack)
                        stack->count = count;
                    else
                        LOG(error) << i.id << " has no stacks, but count = " << count << " on ItemSpawner requested. Count skipped.";
                }

                ch->items.push_back(std::move(item));
            }
        }
    }
}
