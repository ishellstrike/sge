#ifndef FONT_H
#define FONT_H
#include <vector>
#include <glm/glm.hpp>
#include "texture.h"
#include <unordered_map>
#include <memory>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype2/config/ftheader.h>
#define FDIM 1024

struct CharInfo
{
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec2 advance;
    glm::vec2 bearing;
};


class Font
{
public:
    Font();
    ~Font();

    std::shared_ptr<Texture> font;
    std::unordered_map<char32_t, CharInfo> chars;

    FT_Library ft;
    FT_Face m_ftFace;
    void initFreeType(int size);
    void renderAtlas();
};

#endif // FONT_H
