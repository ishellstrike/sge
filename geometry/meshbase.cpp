#include "meshbase.h"


MeshBase::MeshBase(VertexInfo &inf) : info(inf)
{

}

void MeshBase::Bind()
{
    assert(false);
}

void MeshBase::ForgetBind()
{
    assert(false);
}

void MeshBase::Render(const Camera &cam, const glm::mat4 &mod, bool aabb_culling)
{
    assert(false);
}

void MeshBase::Assign()
{
    assert(shader && "need shader to bind");

    for(size_t i = 0; i < info.attrib.size(); ++i)
    {
        const VertexAttribute &a = info.attrib[i];
        a.shader_pos = glGetAttribLocation(shader->program, a.name.c_str());
        if(a.shader_pos == -1)
        {
            LOG(error) << a.name << " in UMesh<" << info.name << "> for \"" << shader->shaderfile_name << "\" missed!";
            has_errors = true;
        }
    }
    assigned = true;
}
