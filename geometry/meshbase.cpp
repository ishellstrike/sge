#include "meshbase.h"


MeshBase::MeshBase(VertexInfo &inf) : info(inf)
{

}

void MeshBase::Assign()
{
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
