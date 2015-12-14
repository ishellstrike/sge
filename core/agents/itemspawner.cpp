#include "itemspawner.h"
#include "logger.h"
#include "random.h"
#include "core/objecthelper.h"
#include "core/agents/chest.h"
#include "core/agents/stacked.h"
#include "core/db.h"

void ItemSpawner::Deserialize(rapidjson::Value &val)
{
    if(val.HasMember("items") && val["items"].IsArray())
    {
        rapidjson::Value &spawn_iteration = val["items"];
        if(spawn_iteration.IsArray())
            for(decltype(spawn_iteration.Size()) i=0; i < spawn_iteration.Size(); i++)
            {
                std::vector<SpawnInfo> group;
                rapidjson::Value &spawn_group = spawn_iteration[i];
                for(decltype(spawn_group.Size()) i=0; i < spawn_group.Size(); i++)
                {
                    rapidjson::Value &spawn_group_part = spawn_group[i];

                    SpawnInfo si;

                    si.id            = spawn_group_part.Begin()->GetString();
                    si.low_quantity  = spawn_group_part[1].GetInt();
                    si.high_quantity = spawn_group_part[2].GetInt();
                    si.probability   = spawn_group_part[3].GetDouble();

                    group.push_back(si);
                }
                items.push_back(group);
            }
        else throw;
    }
    else throw;
}

std::shared_ptr<Agent> ItemSpawner::Instantiate()
{
    ItemSpawner *i = new ItemSpawner();
    return std::unique_ptr<ItemSpawner>(i);
}

void ItemSpawner::onInit(ObjectHelper *par)
{
    auto ch = par->GetAgent<Chest>();
    if(ch)
    {
        for(const auto &spawn_iteration : items)
        {
            auto prob = random::next<float>();
            for(const auto &spawn_group_part : spawn_iteration)
            {
                if(prob < spawn_group_part.probability)
                {
                    int count = rand()%(spawn_group_part.high_quantity - spawn_group_part.low_quantity) + spawn_group_part.low_quantity;
                    if(count == 0)
                        continue;

                    auto item = DB::Create(spawn_group_part.id);

                    if(count > 1)
                    {
                        Stacked *stack = item->GetAgent<Stacked>();
                        if(stack)
                            stack->count = count;
                        else
                            LOG(error) << spawn_group_part.id << " has no stacks, but count = " << count << " on ItemSpawner requested. Count skipped.";
                    }

                    ch->items.push_back(std::move(item));
                    continue;
                }
                prob += spawn_group_part.probability; //если не выпало, то увеличиваем шенс на значение вероятности предыдущего предмета
            }
        }
    }
}
