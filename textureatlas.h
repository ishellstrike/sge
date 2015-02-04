#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H
#include <string>
#include "texture.h"
#include <memory>

class TextureAtlas
{
public:
    TextureAtlas();
    ~TextureAtlas();
    static std::shared_ptr<Texture> tex;

    void LoadAll();
};

#endif // TEXTUREATLAS_H
