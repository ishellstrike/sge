#ifndef VOXELSTRUCTURE_H
#define VOXELSTRUCTURE_H
#include <vector>
#include "ClassicNoise.h"
#include <glm/glm.hpp>

class Voxel
{
public:
    unsigned char data;
};

class VoxelStructure
{
public:
    VoxelStructure()
    {}

    VoxelStructure(int a, int b, int c)
    {
        blocks.resize(a);
        for(auto &d1 : blocks)
        {
            d1.resize(b);
            for(auto &d2 : d1)
            {
                d2.resize(c);
            }
        }
    }

    void fillnoise()
    {
        for(int i = 0; i < blocks.size(); ++i)
            for(int j = 0; j < blocks[0].size(); ++j)
                for(int k = 0; k < blocks[0][0].size(); ++k)
                {
                    blocks[i][j][k].data = glm::clamp(Noise::noise(i/12.f+4,j/12.f+4,k/12.f+4)*5,0.f,1.f)*255;
                }
    }

    void fillsphere()
    {
        for(int i = 0; i < blocks.size(); ++i)
            for(int j = 0; j < blocks[0].size(); ++j)
                for(int k = 0; k < blocks[0][0].size(); ++k)
                {
                    blocks[i][j][k].data = pow(i-blocks.size()/2.f-1, 2) + pow(j-blocks.size()/2.f-1, 2) + pow(k-blocks.size()/2.f-1, 2) < pow((blocks.size()-1)/2.f, 2) ? 128 : 0;
                }
    }

    std::vector<std::vector<std::vector<Voxel>>> blocks;
    float downscale = 1;
};

#endif // VOXELSTRUCTURE_H
