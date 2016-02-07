/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "spritebatch.h"
#include "logger.h"
#include <locale>
#include "utfcpp/utf8.h"
#include "prefecences.h"
#include <GL/glew.h>
#include "colorextender.h"
#include "resources/resourcecontroller.h"
#include "textureatlas.h"

typedef std::codecvt<wchar_t, char, mbstate_t> cvt;

//#include <unistd.h>

SpriteBatch::SpriteBatch()
{
    vertices.resize(BUFFER_SIZE*4);
    index.resize(BUFFER_SIZE*6);

    basic_program = Resources::instance()->Get<BasicJargShader>("basic");
    color_program = Resources::instance()->Get<BasicJargShader>("color");
    font_program = Resources::instance()->Get<BasicJargShader>("font");

    current_program = basic_program;

    glUseProgram(0);

    glGenBuffers(2, m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vtpc)*BUFFER_SIZE*4, nullptr, GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vtpc), (void*)(offsetof(Vtpc, pos)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vtpc), (void*)(offsetof(Vtpc, uv)));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vtpc), (void*)(offsetof(Vtpc, col)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*BUFFER_SIZE*6, nullptr, GL_STREAM_DRAW);

    LOG(info) << "spritebatch ready";
}

SpriteBatch::~SpriteBatch()
{
    glDeleteBuffers(2, m_vbo);
}

/*!
 * \brief SpriteBatch::drawText
 * \param text
 * \param pos
 * \param font
 * \param col_
 * \param align
 *
 * Прямая отрисовка текста (Align == ALIGN_LEFT)
 */
void SpriteBatch::drawText(const std::string &text, const glm::vec2 &pos,
                           Font *font, const glm::vec4 &col_)
{
    std::u32string text32;
    utf8::utf8to32(text.begin(), text.end(), std::back_inserter(text32));

    drawText(text32, pos.x, pos.y + font->spacing, font, col_, false);
}

/*!
 * \brief SpriteBatch::drawText
 * \param text
 * \param pos
 * \param size
 * \param font
 * \param col_
 * \param align
 *
 * Отрисовка текста в ограничивающем прямоугольнике
 */
void SpriteBatch::drawText(const std::string &text, const glm::vec2 &pos, const glm::vec2 &size,
                           Font *font, const glm::vec4 &col_, ALIGN align)
{
    std::u32string text32;
    utf8::utf8to32(text.begin(), text.end(), std::back_inserter(text32));

    glm::vec2 a = drawText(text32, pos.x, pos.y, font, col_, true);
    glm::vec2 half(0);

    switch (align) {
    case ALIGN_CENTER:
        half = pos + size / 2.f;
        half -= a / 2.f;
        drawText(text32, half.x, half.y + font->spacing, font, col_, false);
        break;
    case ALIGN_HOR_CENTER:
        half = pos + size / 2.f;
        half -= a / 2.f;
        drawText(text32, half.x, pos.y + font->spacing, font, col_, false);
        break;
    default: //LEFT
        drawText(text32, pos.x, pos.y + font->spacing, font, col_, false);
        break;
    }
}

glm::vec2 SpriteBatch::measureText(const std::string &text, Font *font)
{
    std::u32string text32;
    utf8::utf8to32(text.begin(), text.end(), std::back_inserter(text32));
    return drawText(text32, 0, 0, font, glm::vec4(0), true);
}

glm::vec2 SpriteBatch::drawFormatted(const std::string &, glm::vec2 , Font *)
{
    return {0,0};
}

std::string u32_to_ascii(std::u32string const &s)
{
    std::string out;
    std::transform(begin(s), end(s), back_inserter(out), [](char32_t c)
    {
        return c < 128 ? static_cast<char>(c) : '?';
    });
    return "0x"+out;
}

int u32toi(std::u32string const &s)
{
    return stoi(u32_to_ascii(s), 0, 16);
}

glm::vec2 SpriteBatch::drawText(const std::u32string &text32, float x, float y,
                                Font *font, const glm::vec4 &col, bool no_draw)
{
    glm::vec4 col_ = col;
    float x_start = x;
    float y_start = y;
    float x_max = 0;
    const char32_t *p;
    bool color_code = false;

    int pos = 0;
    for(p = &text32[0]; *p; p++, pos++)
    {
        Font::CharInfo cc = font->chars[*p];
        switch(*p)
        {
        case '\n':
            y += font->spacing;
            x = x_start;
            break;
        case '|':
            {
                if(pos + 6 <= text32.length())
                {
                    p++;

                    std::u32string hcol(p, p+6);
                    uint32_t hexValue;
                    try
                    {
                        hexValue = u32toi(hcol);
                    }
                    catch (...)
                    {
                        hexValue = 0xff0000;
                    }

                    float r = ((hexValue >> 16) & 0xff) / 255.f;
                    float g = ((hexValue >> 8 ) & 0xff) / 255.f;
                    float b = ((hexValue      ) & 0xff) / 255.f;
                    col_    = glm::vec4(r, g, b, col.a);
                    p += 5;
                }
            }
            break;

        default:
            if(!no_draw)
                drawQuadText(glm::vec2(x, y), cc, *font->font, col_);

            x += cc.advance.x;
            x_max = glm::max(static_cast<float>(x), x_max);
            break;
        }
    }
    y += font->spacing;
    return glm::vec2(x_max - x_start, y - y_start);
}

void SpriteBatch::drawRect(const glm::vec2 &loc, const glm::vec2 &size, const glm::vec4 &_col)
{
    if(cur >= BUFFER_SIZE - 1)
        render();
    if(current_program != color_program)
    {
        render();
        current_program = color_program;
        current_program->Use();
    }
    vertices[cur*4    ].pos = glm::vec3(loc.x,          loc.y,          0);
    vertices[cur*4 + 1].pos = glm::vec3(loc.x + size.x, loc.y,          0);
    vertices[cur*4 + 2].pos = glm::vec3(loc.x + size.x, loc.y + size.y, 0);
    vertices[cur*4 + 3].pos = glm::vec3(loc.x,          loc.y + size.y, 0);

    vertices[cur*4    ].col = _col;
    vertices[cur*4 + 1].col = _col;
    vertices[cur*4 + 2].col = _col;
    vertices[cur*4 + 3].col = _col;

    vertices[cur*4    ].uv = glm::vec2(0, 0);
    vertices[cur*4 + 1].uv = glm::vec2(1, 0);
    vertices[cur*4 + 2].uv = glm::vec2(1, 1);
    vertices[cur*4 + 3].uv = glm::vec2(0, 1);

    index[cur*6    ] = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 3;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 2;
    index[cur*6 + 5] = cur*4 + 3;

    cur++;
}

void SpriteBatch::drawQuadText(const glm::vec2 &loc, const Font::CharInfo &inf, const Texture &tex, const glm::vec4 &color)
{
    if(current_program != font_program)
    {
        render();
        current_program = font_program;
        current_program->Use();
    }
    if(tex.textureId != current)
    {
        render();
        current = tex.textureId;
    }

    if(cur >= BUFFER_SIZE - 1)
        render();

    vertices[cur*4    ].pos = glm::vec3(loc.x + inf.bearing.x,                   loc.y - inf.bearing.y, 0);
    vertices[cur*4 + 1].pos = glm::vec3(loc.x + inf.bearing.x + inf.size.x*FDIM, loc.y - inf.bearing.y, 0);
    vertices[cur*4 + 2].pos = glm::vec3(loc.x + inf.bearing.x + inf.size.x*FDIM, loc.y - inf.bearing.y + inf.size.y*FDIM, 0);
    vertices[cur*4 + 3].pos = glm::vec3(loc.x + inf.bearing.x,                   loc.y - inf.bearing.y + inf.size.y*FDIM, 0);

    vertices[cur*4    ].col = color;
    vertices[cur*4 + 1].col = color;
    vertices[cur*4 + 2].col = color;
    vertices[cur*4 + 3].col = color;

    vertices[cur*4 + 0].uv  = glm::vec2(inf.pos.x,              inf.pos.y);
    vertices[cur*4 + 1].uv  = glm::vec2(inf.pos.x + inf.size.x, inf.pos.y);
    vertices[cur*4 + 2].uv  = glm::vec2(inf.pos.x + inf.size.x, inf.pos.y + inf.size.y);
    vertices[cur*4 + 3].uv  = glm::vec2(inf.pos.x,              inf.pos.y + inf.size.y);

    index[cur*6    ] = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 3;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 2;
    index[cur*6 + 5] = cur*4 + 3;

    cur++;
}

void SpriteBatch::drawQuad(const glm::vec2 &loc, const glm::vec2 &size, GLuint tex_id, const glm::vec4 &col_, const glm::vec4 &double_uv)
{
    if(current_program != basic_program)
    {
        render();
        current_program = basic_program;
        current_program->Use();
    }
    if(tex_id != current)
    {
        render();
        current = tex_id;
    }
    if(cur >= BUFFER_SIZE - 1)
        render();

    vertices[cur*4    ].pos = glm::vec3(loc.x,          loc.y,          0);
    vertices[cur*4 + 1].pos = glm::vec3(loc.x + size.x, loc.y,          0);
    vertices[cur*4 + 2].pos = glm::vec3(loc.x + size.x, loc.y + size.y, 0);
    vertices[cur*4 + 3].pos = glm::vec3(loc.x,          loc.y + size.y, 0);

    vertices[cur*4    ].col = col_;
    vertices[cur*4 + 1].col = col_;
    vertices[cur*4 + 2].col = col_;
    vertices[cur*4 + 3].col = col_;

	vertices[cur * 4].uv = glm::vec2(double_uv.x, double_uv.y);
	vertices[cur * 4 + 1].uv = glm::vec2(double_uv.z, double_uv.y);
	vertices[cur * 4 + 2].uv = glm::vec2(double_uv.z, double_uv.w);
	vertices[cur * 4 + 3].uv = glm::vec2(double_uv.x, double_uv.w);

    index[cur*6    ] = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 3;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 2;
    index[cur*6 + 5] = cur*4 + 3;

    cur++;
}

void SpriteBatch::drawTriangle(const glm::vec2 &loc, const glm::vec2 &loc2, const glm::vec2 &loc3, const glm::vec4 &col_)
{
    if(current_program != color_program)
    {
        render();
        current_program = color_program;
        current_program->Use();
    }
    if(cur >= BUFFER_SIZE - 1)
        render();

    vertices[cur*4    ].pos = glm::vec3(loc.x,  loc.y, 0);
    vertices[cur*4 + 1].pos = glm::vec3(loc2.x, loc2.y, 0);
    vertices[cur*4 + 2].pos = glm::vec3(loc3.x, loc3.y, 0);

    vertices[cur*4    ].col = col_;
    vertices[cur*4 + 1].col = col_;
    vertices[cur*4 + 2].col = col_;

    index[cur*6    ] = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 2;
    index[cur*6 + 3] = cur*4 + 2;
    index[cur*6 + 4] = cur*4 + 2;
    index[cur*6 + 5] = cur*4 + 2;

    cur++;
}

void SpriteBatch::drawQuad(const glm::vec2 &loc, const glm::vec2 &size, const Texture &tex, const glm::vec4 &col_, const glm::vec4 &double_uv)
{
    drawQuad(loc, size, tex.textureId, col_, double_uv);
}

void SpriteBatch::drawQuadAtlas(const glm::vec2 &loc, const glm::vec2 &size, const AtlasPart &tex, int apos, const glm::vec4 &col_)
{
    if(current_program != basic_program)
    {
        render();
        current_program = basic_program;
        current_program->Use();
    }
    if(tex.tex->textureId != current)
    {
        render();
        current = tex.tex->textureId;
    }
    if(cur >= BUFFER_SIZE - 1)
        render();

    vertices[cur*4    ].pos = glm::vec3(loc.x,          loc.y,          0);
    vertices[cur*4 + 1].pos = glm::vec3(loc.x + size.x, loc.y,          0);
    vertices[cur*4 + 2].pos = glm::vec3(loc.x + size.x, loc.y + size.y, 0);
    vertices[cur*4 + 3].pos = glm::vec3(loc.x,          loc.y + size.y, 0);

    vertices[cur*4    ].col = col_;
    vertices[cur*4 + 1].col = col_;
    vertices[cur*4 + 2].col = col_;
    vertices[cur*4 + 3].col = col_;

    float sx = 32 / (float) tex.tex->width;
    float sy = 32 / (float) tex.tex->height;
    int inrow = tex.tex->width / 32;
    float x = (apos % inrow) * sx;
    float y = (apos / inrow) * sy;

    vertices[cur*4    ].uv = glm::vec2(x,      y);
    vertices[cur*4 + 1].uv  = glm::vec2(x + sx, y);
    vertices[cur*4 + 2].uv  = glm::vec2(x + sx, y + sy);
    vertices[cur*4 + 3].uv  = glm::vec2(x,      y + sy);

    index[cur*6    ] = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 3;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 2;
    index[cur*6 + 5] = cur*4 + 3;

    cur++;
}

void SpriteBatch::drawLine(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color)
{
    if(cur >= BUFFER_SIZE - 1)
        render();
    if(current_program != color_program)
    {
        render();
        current_program = color_program;
        current_program->Use();
    }

    glm::vec2 s = start;
    glm::vec2 e = end;

    float len = glm::length(s - e);
    float phi = atan2(s.y - e.y, s.x - e.x);

    glm::mat2 rot = glm::mat2();
    rot[0][0] = glm::cos(phi); rot[0][1] = -glm::sin(phi);
    rot[1][0] = glm::sin(phi); rot[1][1] = glm::cos(phi);
    // |\ |
    // | \|
    //  p
    glm::vec2 p[4] = {glm::vec2(0, 0), glm::vec2(-len, 0), glm::vec2(0, -width), glm::vec2(-len, -width)};
    for(int i = 0; i < 4; i++)
        p[i] = p[i] * rot + s;


    vertices[cur*4    ].pos = glm::vec3(p[0].x, p[0].y, 0);
    vertices[cur*4 + 1].pos = glm::vec3(p[1].x, p[1].y, 0);
    vertices[cur*4 + 2].pos = glm::vec3(p[2].x, p[2].y, 0);
    vertices[cur*4 + 3].pos = glm::vec3(p[3].x, p[3].y, 0);

    vertices[cur*4    ].col = color;
    vertices[cur*4 + 1].col = color;
    vertices[cur*4 + 2].col = color;
    vertices[cur*4 + 3].col = color;

    vertices[cur*4    ].uv  = glm::vec2(0, 0);
    vertices[cur*4 + 1].uv  = glm::vec2(1, 0);
    vertices[cur*4 + 2].uv  = glm::vec2(1, 1);
    vertices[cur*4 + 3].uv  = glm::vec2(0, 1);

    index[cur*6    ] = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 2;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 3;
    index[cur*6 + 5] = cur*4 + 2;

    cur++;
}

void SpriteBatch::drawAALine(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color)
{
    if(cur >= BUFFER_SIZE - 4)
        render();
    if(current_program != color_program)
    {
        render();
        current_program = color_program;
        current_program->Use();
    }

    glm::vec2 s = start;
    glm::vec2 e = end;

    float len = glm::length(s - e);
    float phi = atan2(s.y - e.y, s.x - e.x);

    glm::mat2 rot = glm::mat2();
    rot[0][0] = glm::cos(phi); rot[0][1] = -glm::sin(phi);
    rot[1][0] = glm::sin(phi); rot[1][1] = glm::cos(phi);

    float p = 4;

    // |\ |\ |\ |
    // | \| \| \|
    //  r  c  l
    glm::vec2 p_left[4]   = {glm::vec2(0, 0),          glm::vec2(-len, 0),          glm::vec2(0, -width/p),   glm::vec2(-len, -width/p)};
    glm::vec2 p_center[4] = {glm::vec2(0, -width/p),   glm::vec2(-len, -width/p),   glm::vec2(0, -width/p*3), glm::vec2(-len, -width/p*3)};
    glm::vec2 p_right[4]  = {glm::vec2(0, -width/p*3), glm::vec2(-len, -width/p*3), glm::vec2(0, -width),     glm::vec2(-len, -width)};
    for(int i = 0; i < 4; i++)
    {
        p_left[i]   = p_left[i]   * rot + s;
        p_center[i] = p_center[i] * rot + s;
        p_right[i]  = p_right[i]  * rot + s;
    }

    glm::vec4 tr = Color::Clear;


    vertices[cur*4     ].pos = glm::vec3(p_left[0].x, p_left[0].y, 0);
    vertices[cur*4 + 1 ].pos = glm::vec3(p_left[1].x, p_left[1].y, 0);
    vertices[cur*4 + 2 ].pos = glm::vec3(p_left[2].x, p_left[2].y, 0);
    vertices[cur*4 + 3 ].pos = glm::vec3(p_left[3].x, p_left[3].y, 0);

    vertices[cur*4 + 4 ].pos = glm::vec3(p_center[0].x, p_center[0].y, 0);
    vertices[cur*4 + 5 ].pos = glm::vec3(p_center[1].x, p_center[1].y, 0);
    vertices[cur*4 + 6 ].pos = glm::vec3(p_center[2].x, p_center[2].y, 0);
    vertices[cur*4 + 7 ].pos = glm::vec3(p_center[3].x, p_center[3].y, 0);

    vertices[cur*4 + 8 ].pos = glm::vec3(p_right[0].x, p_right[0].y, 0);
    vertices[cur*4 + 9 ].pos = glm::vec3(p_right[1].x, p_right[1].y, 0);
    vertices[cur*4 + 10].pos = glm::vec3(p_right[2].x, p_right[2].y, 0);
    vertices[cur*4 + 11].pos = glm::vec3(p_right[3].x, p_right[3].y, 0);

    vertices[cur*4 + 0 ].col = tr;
    vertices[cur*4 + 1 ].col = tr;
    vertices[cur*4 + 2 ].col = color;
    vertices[cur*4 + 3 ].col = color;

    vertices[cur*4 + 4 ].col = color;
    vertices[cur*4 + 5 ].col = color;
    vertices[cur*4 + 6 ].col = color;
    vertices[cur*4 + 7 ].col = color;

    vertices[cur*4 + 8 ].col = color;
    vertices[cur*4 + 9 ].col = color;
    vertices[cur*4 + 10].col = tr;
    vertices[cur*4 + 11].col = tr;

    vertices[cur*4     ].uv  = glm::vec2(0, 0);
    vertices[cur*4 + 1 ].uv  = glm::vec2(1, 0);
    vertices[cur*4 + 2 ].uv  = glm::vec2(1, 1);
    vertices[cur*4 + 3 ].uv  = glm::vec2(0, 1);
    vertices[cur*4 + 4 ].uv  = glm::vec2(0, 0);
    vertices[cur*4 + 5 ].uv  = glm::vec2(1, 0);
    vertices[cur*4 + 6 ].uv  = glm::vec2(1, 1);
    vertices[cur*4 + 7 ].uv  = glm::vec2(0, 1);
    vertices[cur*4 + 8 ].uv  = glm::vec2(0, 0);
    vertices[cur*4 + 9 ].uv  = glm::vec2(1, 0);
    vertices[cur*4 + 10].uv  = glm::vec2(1, 1);
    vertices[cur*4 + 11].uv  = glm::vec2(0, 1);

    index[cur*6    ] = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 2;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 3;
    index[cur*6 + 5] = cur*4 + 2;

    index[cur*6 + 6 ] = cur*4     + 4;
    index[cur*6 + 7 ] = cur*4 + 1 + 4;
    index[cur*6 + 8 ] = cur*4 + 2 + 4;
    index[cur*6 + 9 ] = cur*4 + 1 + 4;
    index[cur*6 + 10] = cur*4 + 3 + 4;
    index[cur*6 + 11] = cur*4 + 2 + 4;

    index[cur*6 + 12] = cur*4     + 8;
    index[cur*6 + 13] = cur*4 + 1 + 8;
    index[cur*6 + 14] = cur*4 + 2 + 8;
    index[cur*6 + 15] = cur*4 + 1 + 8;
    index[cur*6 + 16] = cur*4 + 3 + 8;
    index[cur*6 + 17] = cur*4 + 2 + 8;

    cur+=3;
}

void SpriteBatch::render()
{
    if(cur == 0)
        return;
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    current_program->Use();
    current_program->SetUniform("transform_VP", uniform);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, current);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vtpc)*cur*4, &vertices[0]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vtpc), (void*)(offsetof(Vtpc, pos)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vtpc), (void*)(offsetof(Vtpc, uv)));
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vtpc), (void*)(offsetof(Vtpc, col)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLshort)*cur*6, &index[0]);

    glDrawElements(GL_TRIANGLES, cur*6, GL_UNSIGNED_SHORT, NULL);
    glEnable(GL_CULL_FACE);

    cur = 0;
}

void SpriteBatch::reduceScissor(glm::vec2 loc, glm::vec2 size)
{
    loc = scis_min = glm::max(loc, scis_min);
    size = scis_size = glm::min(size, scis_size);

    render();
    glEnable(GL_SCISSOR_TEST);
    glScissor(loc.x, RESY - (loc.y + size.y), size.x, size.y);
}

void SpriteBatch::resetScissor()
{
    render();
    glDisable(GL_SCISSOR_TEST);
    scis_min = {0,0};
    scis_size = {RESX, RESY};
}
