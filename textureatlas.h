#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H
#include <string>
#include "texture.h"

class TextureAtlas
{
public:
    TextureAtlas();
    ~TextureAtlas();
    static Texture tex;

    void LoadAll();
};

#endif // TEXTUREATLAS_H
