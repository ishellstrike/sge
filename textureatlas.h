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

class TextureAtlas
{
public:
    TextureAtlas();
    ~TextureAtlas();
    static std::shared_ptr<Texture> tex;
    static std::unordered_map<std::string, int> refs;

    void LoadAll();
};

#endif // TEXTUREATLAS_H
