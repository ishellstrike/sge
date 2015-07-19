#pragma once
#include "resources/texture.h"
#include <vector>
#include "FrameBuffer.h"
#include "basicjargshader.h"
#include <memory>
#include <functional>

class TextureGenerator
{
public:
    TextureGenerator(void);
    ~TextureGenerator(void);
    void PushBackTexture(std::shared_ptr<Texture> tex);
    void AddTexture(std::string __name, std::shared_ptr<Texture> &a);
    void SetResultTexture(std::shared_ptr<Texture> _tex);
    void SetShader(std::shared_ptr<BasicJargShader> _shader);
    void RenderOnTempFbo(std::function<void()> func = [](){}) const;
    void SetParams(float tex);
    void Reset();
private:
    std::vector<std::shared_ptr<Texture>> texes;
    std::vector<float> params;
    std::shared_ptr<BasicJargShader> shader;
    std::shared_ptr<Texture> target;
    std::vector<std::pair<std::string, std::shared_ptr<Texture>>> named_textures;
};

