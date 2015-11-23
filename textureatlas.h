/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H
#include <string>
#include "resources/texture.h"
#include <memory>
#include <unordered_map>

struct AtlasPart
{
    std::shared_ptr<Texture> tex, tex_n, tex_o;
};

class TextureAtlas
{
public:
    TextureAtlas();
    ~TextureAtlas();
    static std::vector<AtlasPart> tex;
    static std::unordered_map<std::string, std::tuple<int, int>> refs;
    static std::vector<glm::vec4> uvs;
    static std::vector<glm::vec2> size;
    static std::vector<std::vector<std::vector<bool>>> pixels;

    static void LoadAll();
    static glm::vec2 GetUV(int n);
};

#endif // TEXTUREATLAS_H
