#ifndef AGENTS_H
#define AGENTS_H

#define CASTERS \
    CASTER(Chest) \
    CASTER(ItemSpawner) \
    CASTER(Stacked)

#define PARTS_PARSER                                                                                               \
    if(val.HasMember("parts")) {                                                                                   \
        LOG(verbose) << "found parts";                                                                             \
        rapidjson::Value &arr = val["parts"];                                                                      \
        if(val["parts"].IsArray())                                                                                 \
        for(decltype(arr.Size()) a = 0; a < arr.Size(); a++)                                                       \
        {                                                                                                          \
            rapidjson::Value &part = arr[a];                                                                       \
            if(part.HasMember("type")) {                                                                           \
                CASTERS                                                                                            \
                /*else here*/ LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has unknown \"type\""; \
            }                                                                                                      \
            else                                                                                                   \
                LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has no type";                        \
        }                                                                                                          \
        else                                                                                                       \
            LOG(error) << "record \"" << id << "\" parts is not valid agents array";                               \
    }                                                                                                              \
    if(val.HasMember("static_parts")) {                                                                            \
        LOG(verbose) << "found parts";                                                                             \
        rapidjson::Value &arr = val["parts"];                                                                      \
        if(val["parts"].IsArray())                                                                                 \
        for(decltype(arr.Size()) a = 0; a < arr.Size(); a++)                                                       \
        {                                                                                                          \
            rapidjson::Value &part = arr[a];                                                                       \
            if(part.HasMember("type")) {                                                                           \
                CASTERS                                                                                            \
                /*else here*/ LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has unknown \"type\""; \
            }                                                                                                      \
            else                                                                                                   \
                LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has no type";                        \
        }                                                                                                          \
        else                                                                                                       \
            LOG(error) << "record \"" << id << "\" parts is not valid agents array";                               \
    }

#include "agent.h"

#include "agents/itemspawner.h"
#include "agents/stacked.h"
#include "agents/chest.h"

#endif // AGENTS_H
