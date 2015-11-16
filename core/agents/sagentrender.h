#ifndef SAGENTRENDER_H
#define SAGENTRENDER_H
#include "core/sagent_base.h"
#include "core/agents/smeshagent.h"
#include "camera.h"
#include <memory>
#include "resources/material.h"
#include "shader.h"

class SRenderSettings
{
public:

    static void Init(); //must be called before any render, after resource loading
    void Set();
    static void Reset();

    bool wireframe = false;
    std::shared_ptr<Material> material;
    std::shared_ptr<BasicJargShader> shader;

    static std::shared_ptr<SRenderSettings> basic;
    static std::shared_ptr<SRenderSettings> current;
};

class SRenderAgent : public SAgentBase
{
public:
    std::shared_ptr<SRenderSettings> settings = SRenderSettings::basic;
    std::shared_ptr<SMeshAgent> mesh;

    void Render(const Camera &cam);

    // SAgentBase interface
 public:
    AGENT(SRenderAgent)
};

#endif // SAGENTRENDER_H
