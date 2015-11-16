#ifndef SMESHAGENT_H
#define SMESHAGENT_H
#include <core/sagent_base.h>
#include <geometry/umesh.h>

class SMeshAgent : public SAgentBase
{
public:
    std::shared_ptr<MeshBase> mesh;

    template<typename _Ty>
    void SetMesh(_Ty &&__mesh)
    {
        mesh = std::unique_ptr<MeshBase>(__mesh);
    }

    // SAgentBase interface
 public:
    AGENT(SMeshAgent)
};

#endif // SMESHAGENT_H
