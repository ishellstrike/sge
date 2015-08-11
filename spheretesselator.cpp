#include "spheretesselator.h"

Mesh* Tesselator::Tesselate(int iters, Mesh &mesh)
{
    Mesh* m = new Mesh(mesh);
    for (int i = 0; i< iters; ++i)
    {
        auto temp = SubTesselate(*m);
        delete m;
        m = temp;
    }
    return m;
}

void Tesselator::SphereTesselate(int iters, std::shared_ptr<Mesh> &mesh)
{
    for (int i = 0; i< iters; ++i)
    {
        SphereSubTesselate(mesh);
    }
}

static const int Seed = 123;

inline float Noise2D__(float x, float y) {
    int n = (int) (x + y*57);
    n = (n << 13) ^ n;
    float value = (1.0f - ((n*(n*n*15731 + 789221) + 1376312589) & 0x7fffffff)/1073741824.0f);
    return fabs(value);
}

inline float Noise2D(float x, float y){
    return Noise2D__(x,y)/10+0.9f;
}

inline float SmoothedNoise2D(float x, float y) {
    float corners = (Noise2D(x - 1, y - 1) + Noise2D(x + 1, y - 1) + Noise2D(x - 1, y + 1) +
        Noise2D(x + 1, y + 1))/16;
    float sides = (Noise2D(x - 1, y) + Noise2D(x + 1, y) + Noise2D(x, y - 1) + Noise2D(x, y + 1))/8;
    float center = Noise2D(x, y)/4;
    return corners + sides + center;
}

void Tesselator::SphereSubTesselate(std::shared_ptr<Mesh> &mesh)
{
    std::shared_ptr<Mesh> m = std::make_shared<Mesh>();

    int off = 0;
    for (size_t i =0; i< mesh->Indices.size() -2; i+= 3)
    {
        VertPosNormUvUv t;

        VertPosNormUvUv v0 = mesh->Vertices[mesh->Indices[i]];
        VertPosNormUvUv v1 = mesh->Vertices[mesh->Indices[i+1]];
        VertPosNormUvUv v2 = mesh->Vertices[mesh->Indices[i+2]];

        t = v0;
        t.position = glm::normalize(t.position);
        //	t.Position *= SmoothedNoise2D(t.Position.x, t.Position.y + t.Position.z);
        m->Vertices.push_back(t);
        t = v1;
        t.position = glm::normalize(t.position);
        //	t.Position *= SmoothedNoise2D(t.Position.x, t.Position.y + t.Position.z);
        m->Vertices.push_back(t);
        t = v2;
        t.position = glm::normalize(t.position);
        //t.Position *= SmoothedNoise2D(t.Position.x, t.Position.y + t.Position.z);
        m->Vertices.push_back(t);

        t = (v0 + v1)/2;
        t.position = glm::normalize(t.position);
        //t.Position *= SmoothedNoise2D(t.Position.x, t.Position.y + t.Position.z);
        m->Vertices.push_back(t);

        t = (v0 + v2)/2;
        t.position = glm::normalize(t.position);
        //t.Position *= SmoothedNoise2D(t.Position.x, t.Position.y + t.Position.z);
        m->Vertices.push_back(t);

        t = (v1 + v2)/2;
        t.position = glm::normalize(t.position);
        //t.Position *= SmoothedNoise2D(t.Position.x, t.Position.y + t.Position.z);
        m->Vertices.push_back(t);

        m->Indices.push_back(off+0);
        m->Indices.push_back(off+3);
        m->Indices.push_back(off+4);

        m->Indices.push_back(off+3);
        m->Indices.push_back(off+1);
        m->Indices.push_back(off+5);

        m->Indices.push_back(off+5);
        m->Indices.push_back(off+2);
        m->Indices.push_back(off+4);

        m->Indices.push_back(off+3);
        m->Indices.push_back(off+5);
        m->Indices.push_back(off+4);

        off+=6;
    }
    mesh = m;
}

Mesh* Tesselator::SubTesselate(const Mesh& mesh)
{
    Mesh* m = new Mesh();

    int off = 0;
    for (size_t i =0; i< mesh.Indices.size() -2; i+= 3)
    {
        VertPosNormUvUv t;

        m->Vertices.push_back(mesh.Vertices[mesh.Indices[i]]);
        m->Vertices.push_back(mesh.Vertices[mesh.Indices[i+1]]);
        m->Vertices.push_back(mesh.Vertices[mesh.Indices[i+2]]);

        t.position.x = (mesh.Vertices[mesh.Indices[i]].position.x + mesh.Vertices[mesh.Indices[i+1]].position.x)/2;
        t.position.y = (mesh.Vertices[mesh.Indices[i]].position.y + mesh.Vertices[mesh.Indices[i+1]].position.y)/2;
        t.position.z = (mesh.Vertices[mesh.Indices[i]].position.z + mesh.Vertices[mesh.Indices[i+1]].position.z)/2;
        t.uv.x = (mesh.Vertices[mesh.Indices[i]].uv.x + mesh.Vertices[mesh.Indices[i+1]].uv.x)/2;
        t.uv.y = (mesh.Vertices[mesh.Indices[i]].uv.y + mesh.Vertices[mesh.Indices[i+1]].uv.y)/2;
        m->Vertices.push_back(t);

        t.position.x = (mesh.Vertices[mesh.Indices[i]].position.x + mesh.Vertices[mesh.Indices[i+2]].position.x)/2;
        t.position.y = (mesh.Vertices[mesh.Indices[i]].position.y + mesh.Vertices[mesh.Indices[i+2]].position.y)/2;
        t.position.z = (mesh.Vertices[mesh.Indices[i]].position.z + mesh.Vertices[mesh.Indices[i+2]].position.z)/2;
        t.uv.x = (mesh.Vertices[mesh.Indices[i]].uv.x + mesh.Vertices[mesh.Indices[i+2]].uv.x)/2;
        t.uv.y = (mesh.Vertices[mesh.Indices[i]].uv.y + mesh.Vertices[mesh.Indices[i+2]].uv.y)/2;
        m->Vertices.push_back(t);

        t.position.x = (mesh.Vertices[mesh.Indices[i+1]].position.x + mesh.Vertices[mesh.Indices[i+2]].position.x)/2;
        t.position.y = (mesh.Vertices[mesh.Indices[i+1]].position.y + mesh.Vertices[mesh.Indices[i+2]].position.y)/2;
        t.position.z = (mesh.Vertices[mesh.Indices[i+1]].position.z + mesh.Vertices[mesh.Indices[i+2]].position.z)/2;
        t.uv.x = (mesh.Vertices[mesh.Indices[i+1]].uv.x + mesh.Vertices[mesh.Indices[i+2]].uv.x)/2;
        t.uv.y = (mesh.Vertices[mesh.Indices[i+1]].uv.y + mesh.Vertices[mesh.Indices[i+2]].uv.y)/2;
        m->Vertices.push_back(t);

        m->Indices.push_back(off+0);
        m->Indices.push_back(off+3);
        m->Indices.push_back(off+4);

        m->Indices.push_back(off+3);
        m->Indices.push_back(off+1);
        m->Indices.push_back(off+5);

        m->Indices.push_back(off+5);
        m->Indices.push_back(off+2);
        m->Indices.push_back(off+4);

        m->Indices.push_back(off+3);
        m->Indices.push_back(off+5);
        m->Indices.push_back(off+4);

        off+=6;
    }
    return m;
}
