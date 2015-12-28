#include "itemspawner.h"
#include "logger.h"
#include "random.h"
#include "core/objecthelper.h"
#include "core/agents/chest.h"
#include "core/agents/stacked.h"
#include "core/db.h"

void ItemSpawner::Deserialize(const rapidjson::Value &val)
{
    if(val.HasMember("items") && val["items"].IsArray())
    {
        const rapidjson::Value &spawn_iteration = val["items"];
        if(spawn_iteration.IsArray())
            for(decltype(spawn_iteration.Size()) i=0; i < spawn_iteration.Size(); i++)
            {
                std::vector<SpawnInfo> group;
                const rapidjson::Value &spawn_group = spawn_iteration[i];
                for(decltype(spawn_group.Size()) i=0; i < spawn_group.Size(); i++)
                {
                    const rapidjson::Value &spawn_group_part = spawn_group[i];

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

void ItemSpawner::onInit(Object *par, Level *, const glm::vec3 &, const GameTimer &)
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
                    int count = spawn_group_part.high_quantity;
                    if( spawn_group_part.high_quantity != spawn_group_part.low_quantity )
                        count = rand()%(spawn_group_part.high_quantity - spawn_group_part.low_quantity) + spawn_group_part.low_quantity;
                    if(count == 0)
                        continue;

                    std::string oid = spawn_group_part.id;
                    bool tagged = false;
                    if(oid.length() >= 4)
                    {
                        std::string tag = oid.substr(0, 4);
                        if(tag == "tag_")
                        {
                            std::vector<ObjectStatic *> t_ref = DB::tags_ref[oid];
                            if(t_ref.size() > 0)
                            {
                                for(int cc = 0; cc < count; cc++)
                                {
                                    oid = t_ref[rand() % t_ref.size()]->id;
                                    auto item = DB::Create(oid);
                                    ch->items.push_back(item);
                                }
                            }
                            tagged = true;
                        }
                    }
                    if(!tagged)
                    {
                        auto item = DB::Create(oid);

                        if(count > 1)
                        {
                            Item *stack = item->GetAgent<Item>();
                            if(stack)
                                stack->count = count;
                            else
                                LOG(error) << oid << " has no stacks, but count = " << count << " on ItemSpawner requested. Count skipped.";
                        }

                        ch->items.push_back(item);
                    }
                    continue;
                }
                prob += spawn_group_part.probability; //если не выпало, то увеличиваем шенс на значение вероятности предыдущего предмета
            }
        }
        ch->Combine();
    }
}
