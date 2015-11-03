#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H
#include "geometry/umesh.h"
#include "voxelstructure.h"
#include "logger.h"
#include "ClassicNoise.h"

class MarchingCubes
{
public:

    typedef struct {
       glm::vec3 p[8];
       double val[8];
    } GRIDCELL;

    template <typename _Vtype>
    static void Polygonise(GRIDCELL grid, double isolevel, UMesh<_Vtype> &mesh)
    {
        int i;
        int cubeindex;
        glm::vec3 vertlist[12];

    #include "marchingcubeshelper.h"

        cubeindex = 0;
        if (grid.val[0] < isolevel) cubeindex |= 1;
        if (grid.val[1] < isolevel) cubeindex |= 2;
        if (grid.val[2] < isolevel) cubeindex |= 4;
        if (grid.val[3] < isolevel) cubeindex |= 8;
        if (grid.val[4] < isolevel) cubeindex |= 16;
        if (grid.val[5] < isolevel) cubeindex |= 32;
        if (grid.val[6] < isolevel) cubeindex |= 64;
        if (grid.val[7] < isolevel) cubeindex |= 128;

        /* Cube is entirely in/out of the surface */
        if (edgeTable[cubeindex] == 0)
            return;

        /* Find the vertices where the surface intersects the cube */
        if (edgeTable[cubeindex] & 1)
            vertlist[0] =
                    VertexInterp(isolevel, grid.p[0], grid.p[1], grid.val[0], grid.val[1]);
        if (edgeTable[cubeindex] & 2)
            vertlist[1] =
                    VertexInterp(isolevel, grid.p[1], grid.p[2], grid.val[1], grid.val[2]);
        if (edgeTable[cubeindex] & 4)
            vertlist[2] =
                    VertexInterp(isolevel, grid.p[2], grid.p[3], grid.val[2], grid.val[3]);
        if (edgeTable[cubeindex] & 8)
            vertlist[3] =
                    VertexInterp(isolevel, grid.p[3], grid.p[0], grid.val[3], grid.val[0]);
        if (edgeTable[cubeindex] & 16)
            vertlist[4] =
                    VertexInterp(isolevel, grid.p[4], grid.p[5], grid.val[4], grid.val[5]);
        if (edgeTable[cubeindex] & 32)
            vertlist[5] =
                    VertexInterp(isolevel, grid.p[5], grid.p[6], grid.val[5], grid.val[6]);
        if (edgeTable[cubeindex] & 64)
            vertlist[6] =
                    VertexInterp(isolevel, grid.p[6], grid.p[7], grid.val[6], grid.val[7]);
        if (edgeTable[cubeindex] & 128)
            vertlist[7] =
                    VertexInterp(isolevel, grid.p[7], grid.p[4], grid.val[7], grid.val[4]);
        if (edgeTable[cubeindex] & 256)
            vertlist[8] =
                    VertexInterp(isolevel, grid.p[0], grid.p[4], grid.val[0], grid.val[4]);
        if (edgeTable[cubeindex] & 512)
            vertlist[9] =
                    VertexInterp(isolevel, grid.p[1], grid.p[5], grid.val[1], grid.val[5]);
        if (edgeTable[cubeindex] & 1024)
            vertlist[10] =
                    VertexInterp(isolevel, grid.p[2], grid.p[6], grid.val[2], grid.val[6]);
        if (edgeTable[cubeindex] & 2048)
            vertlist[11] =
                    VertexInterp(isolevel, grid.p[3], grid.p[7], grid.val[3], grid.val[7]);

        for (i = 0; triTable[cubeindex][i] != -1; i += 3) {
            _Vtype v1;
            v1.position = vertlist[triTable[cubeindex][i]];
            mesh.vertices.push_back(v1);

            _Vtype v2;
            v2.position = vertlist[triTable[cubeindex][i + 1]];
            mesh.vertices.push_back(v2);

            _Vtype v3;
            v3.position = vertlist[triTable[cubeindex][i + 2]];
            mesh.vertices.push_back(v3);

            mesh.indices.push_back(mesh.vertices.size()-3);
            mesh.indices.push_back(mesh.vertices.size()-2);
            mesh.indices.push_back(mesh.vertices.size()-1);
        }
    }

    static glm::vec3 VertexInterp(double isolevel, const glm::vec3 &p1, const glm::vec3 &p2, double valp1, double valp2);

    template <typename _Vtype>
    static UMesh<_Vtype> generate(const VoxelStructure &str)
    {
        assert(str.blocks.size() >= 2     ||
               str.blocks[0].size() >= 2  ||
                str.blocks[0][0].size() >= 2 && "2x2x2 minimum voxel size");

        UMesh<_Vtype> m;
        m.vertices.reserve(str.blocks.size()*str.blocks[0].size()*str.blocks[0][0].size());
        m.indices.reserve(str.blocks.size()*str.blocks[0].size()*str.blocks[0][0].size());

        for(int i = 0; i < str.blocks.size() - 1; ++i)
            for(int j = 0; j < str.blocks[0].size() - 1; ++j)
                for(int k = 0; k < str.blocks[0][0].size() - 1; ++k)
                {
                    GRIDCELL gc;
                    gc.p[0] = {i,j+1,k};
                    gc.p[1] = {i+1,j+1,k};
                    gc.p[2] = {i+1,j+1,k+1};
                    gc.p[3] = {i,j+1,k+1};
                    gc.p[4] = {i,j,k};
                    gc.p[5] = {i+1,j,k};
                    gc.p[6] = {i+1,j,k+1};
                    gc.p[7] = {i,j,k+1};
                    for(int mm=0; mm<8; mm++)
                    {
                        gc.val[mm] = str.blocks[gc.p[mm].x][gc.p[mm].y][gc.p[mm].z].data;
                        gc.p[mm] *= str.downscale;
                    }
                    Polygonise(gc, 128, m);
                }

        return m;
    }
};

#endif // MARCHINGCUBES_H
