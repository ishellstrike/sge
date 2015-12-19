#include "levelgen.h"


void Generate(Sector &s)
{
    for(int i = 0; i < RX; i++)
        for(int j = 0; j < RY; j++)
            for(int k = 0; k < RZ; k++)
            {
                s.data[ONEDIM(i,j,k)] = DB::Create("air");
            }

    for(int j = 0; j < RY; j++)
        for(int k = 0; k < RZ; k++)
        {
            s.data[ONEDIM(k,j,0)] = DB::Create("grass");
        }

    s.PlaceScheme(DB::scheme_db["house1_room"][0], {0,0,1});

    s.RebuildMax();
}
