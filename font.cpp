/*******************************************************************************
        Copyright (C) 2014 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt" or "http://copyfree.org/LICENSEs/mit/LICENSE.txt".
*******************************************************************************/

#include "font.h"
#include "logger.h"
#include "utfcpp/utf8.h"
#include <fstream>

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
    LOG(trace) << "FT " << size << " start";
    if(FT_Init_FreeType(&ft))
    {
        LOG(fatal) << "could not init free type library.";
        return;
    }
    else
        LOG(trace) << "FT " << size << " init OK";
    char fontPath[] = "data/fonts/DejaVuSansMono.ttf";
    if(FT_New_Face(ft, fontPath, 0, &m_ftFace))
    {
        LOG(fatal) << "Could not open font " << fontPath;
        return;
    }
    else
        LOG(trace) << "FT " << size << " face OK";

    FT_Select_Charmap(m_ftFace, ft_encoding_unicode);
    FT_Set_Pixel_Sizes(m_ftFace, 0, size);

    spacing = m_ftFace->size->metrics.height >> 6;
}

void Font::renderAtlas()
{
    float x = 0;
    float y = 0;
    float x_start = x;
    const char32_t *p;
    FT_GlyphSlot ftGlyph = m_ftFace->glyph;
    std::string text8;

    {
        std::ifstream fs("data/fonts/glyphs.txt");
        if(!fs.good())
            LOG(fatal) << "data/fonts/glyphs.txt missed";
        text8 = std::string((std::istreambuf_iterator<char>(fs)),
                             std::istreambuf_iterator<char>());
    }

    std::string::iterator end_it = utf8::find_invalid(text8.begin(), text8.end());
    if (end_it != text8.end())
    {
        LOG(fatal) << "Invalid utf-8 encoding in data/fonts/glyphs.txt";
    }

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
