#include "levelgen.h"
#include "core/agents/entity.h"
#include "random.h"

void Generate(Sector &s)
{
    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
            for(int k = 0; k < RZ; k++)
            {
                s.block[ONEDIM(i,j,k)] = DB::Create("air");
                s.ground[ONEDIM(i,j,k)] = DB::Create("air");
            }

    for(int j = 0; j < RY; j++)
        for(int i = 0; i < RY; i++)
        {
            s.ground[ONEDIM(i,j,0)] = DB::Create("grass");
            if(rand()%20 == 1)
                s.block[ONEDIM(i,j,0)] = DB::Create("tree");
            if(rand()%20 == 1)
                s.block[ONEDIM(i,j,0)] = DB::Create("bush");
        }

    s.PlaceScheme(DB::scheme_db["house1_room"][0], {0,0,0});

    for(int i=0; i<3; ++i)
    {
        std::shared_ptr<Object> o = DB::Create("zombie");
        o->GetAgent<Entity>()->pos = glm::vec3(s.offset.x*RX + random::next()*(RX-1),
                                               s.offset.y*RY + random::next()*(RY-1),
                                               0);
        s.entities.push_back(o);
    }

    s.RebuildMax();
}
