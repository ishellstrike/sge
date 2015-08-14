/*******************************************************************************
        Copyright (C) 2014 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt" or "http://copyfree.org/LICENSEs/mit/LICENSE.txt".
*******************************************************************************/

#ifndef FONT_H
#define FONT_H
#include <vector>
#include <glm/glm.hpp>
#include "resources/texture.h"
#include <unordered_map>
#include <memory>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype2/config/ftheader.h>
#define FDIM 256
class Font
{
public:

    struct CharInfo
    {
        glm::vec2 pos;
        glm::vec2 size;
        glm::vec2 advance;
        glm::vec2 bearing;
    };

    Font();
    ~Font();

    std::shared_ptr<Texture> font;
    std::unordered_map<char32_t, CharInfo> chars;

    FT_Library ft;
    FT_Face m_ftFace;
    void initFreeType(int size);
    void renderAtlas();
    unsigned short spacing;
    int m_size;
};

#endif // FONT_H
