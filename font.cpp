#include "font.h"
#include "logger.h"
#include "utfcpp/utf8.h"

Font::Font() :
    font(std::make_shared<Texture>())
{
    font->IdOnly();
}

Font::~Font()
{
    FT_Done_FreeType(ft);
}

void Font::initFreeType(int size)
{
    LOG(info) << "FT " << size << " start";
    if(FT_Init_FreeType(&ft))
    {
        LOG(fatal) << "could not init free type library.";
        return;
    }
    else
        LOG(fatal) << "FT " << size << " init OK";
    char fontPath[] = "data/fonts/DejaVuSansMono.ttf";
    if(FT_New_Face(ft, fontPath, 0, &m_ftFace))
    {
        LOG(fatal) << "Could not open font " << fontPath;
        return;
    }
    else
        LOG(info) << "FT " << size << " face OK";

    FT_Select_Charmap(m_ftFace, ft_encoding_unicode);
    FT_Set_Pixel_Sizes(m_ftFace, 0, size);
}

void Font::renderAtlas()
{
    float x = 0, sx = 1;
    float y = 0, sy = 1;
    float x_start = x;
    float y_start = y;
    float x_max = 0;
    const char32_t *p;
    FT_GlyphSlot ftGlyph = m_ftFace->glyph;
    std::string  text8(R"xxx( `1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm,
                       ./~!@#$%^&*()_+QWERTYUIOP{}ASDFGHJKL:"ZXCVBNM<>?\|
                       йцукенгшщзхъфывапролджэячсмитьбю
                       ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ123)xxx");

    std::u32string text;

    utf8::utf8to32(text8.begin(), text8.end(), std::back_inserter(text));

    int px=0, py=0;

    glBindTexture(GL_TEXTURE_2D, font->textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, FDIM, FDIM, 0, GL_ALPHA, GL_UNSIGNED_BYTE, (void*)0);

    for(p = &text[0]; *p; p++)
    {
        if(*p == '\n')
        {
            y+=ftGlyph->bitmap.pitch;
            x=x_start;
            continue;
        }
        if(FT_Load_Char(m_ftFace, *p, FT_LOAD_RENDER))
        {
            LOG(error) << "Could not load character" << *p;
            continue;
        }

        px += ftGlyph->advance.x >> 6;
        if(px > FDIM)
        {
            px = 0;
            py += 20;
        }

        CharInfo ci;
        ci.pos = {px/(float)FDIM, py/(float)FDIM};
        ci.size = {ftGlyph->bitmap.width/(float)FDIM, ftGlyph->bitmap.rows/(float)FDIM};
        ci.advance = {ftGlyph->advance.x >> 6, ftGlyph->advance.y >> 6};
        ci.bearing = {ftGlyph->metrics.horiBearingX >> 6, ftGlyph->metrics.horiBearingY >> 6};
        chars[*p] = ci;

        glTexSubImage2D(GL_TEXTURE_2D, 0, px, py, ftGlyph->bitmap.pitch, ftGlyph->bitmap.rows,
                        GL_ALPHA, GL_UNSIGNED_BYTE, ftGlyph->bitmap.buffer);
    }
}
