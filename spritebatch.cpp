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

typedef std::codecvt<wchar_t, char, mbstate_t> cvt;

//#include <unistd.h>

SpriteBatch::SpriteBatch()
{
    index = new GLushort[SIZE*6];
    uv = new glm::vec2[SIZE*4];
    pos = new glm::vec3[SIZE*4];
    col = new glm::vec4[SIZE*4];

    basic_program = Resources::instance()->Get<BasicJargShader>("basic");
    color_program = Resources::instance()->Get<BasicJargShader>("color");
    font_program = Resources::instance()->Get<BasicJargShader>("font");

    current_program = basic_program;

    glUseProgram(0);

    glGenBuffers(4, m_vbo);

    p_loc = 0;

    uv_loc = 1;

    c_loc = 3;

    LOG(info) << "spritebatch ready";
}

SpriteBatch::~SpriteBatch()
{
    delete[] index;
    delete[] uv;
    delete[] pos;
    delete[] col;

    glDeleteBuffers(4, m_vbo);
}

void SpriteBatch::setUniform(const glm::mat4 &uni)
{
    uniform = uni;
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

    glm::vec2 a = drawText(text32, pos.x, pos.y, font, col_, true);
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

void SpriteBatch::resetDc()
{
    last_dc = dc;
    dc = 0;
}

int SpriteBatch::getDc()
{
    return last_dc;
}

glm::vec2 SpriteBatch::drawText(const std::u32string &text32, float x, float y,
                                Font *font, const glm::vec4 &col_, bool no_draw)
{
    float x_start = x;
    float y_start = y;
    float x_max = 0;
    const char32_t *p;
    bool color_code = false;

    for(p = &text32[0]; *p; p++)
    {
        Font::CharInfo cc = font->chars[*p];
        switch(*p)
        {
        case '\n':
            y += font->spacing;
            x = x_start;
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
    if(cur >= SIZE - 1)
        render();
    if(current_program != color_program)
    {
        render();
        current_program = color_program;
        current_program->Use();
    }
    pos[cur*4]     = glm::vec3(loc.x,          loc.y,          0);
    pos[cur*4 + 1] = glm::vec3(loc.x + size.x, loc.y,          0);
    pos[cur*4 + 2] = glm::vec3(loc.x + size.x, loc.y + size.y, 0);
    pos[cur*4 + 3] = glm::vec3(loc.x,          loc.y + size.y, 0);

    col[cur*4]     = _col;
    col[cur*4 + 1] = _col;
    col[cur*4 + 2] = _col;
    col[cur*4 + 3] = _col;

    uv[cur*4]     = glm::vec2(0, 0);
    uv[cur*4 + 1] = glm::vec2(1, 0);
    uv[cur*4 + 2] = glm::vec2(1, 1);
    uv[cur*4 + 3] = glm::vec2(0, 1);

    index[cur*6]     = cur*4;
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

    if(cur >= SIZE - 1)
        render();

    pos[cur*4]     = glm::vec3(loc.x + inf.bearing.x,                   loc.y - inf.bearing.y, 0);
    pos[cur*4 + 1] = glm::vec3(loc.x + inf.bearing.x + inf.size.x*FDIM, loc.y - inf.bearing.y, 0);
    pos[cur*4 + 2] = glm::vec3(loc.x + inf.bearing.x + inf.size.x*FDIM, loc.y - inf.bearing.y + inf.size.y*FDIM, 0);
    pos[cur*4 + 3] = glm::vec3(loc.x + inf.bearing.x,                   loc.y - inf.bearing.y + inf.size.y*FDIM, 0);

    col[cur*4]     = color;
    col[cur*4 + 1] = color;
    col[cur*4 + 2] = color;
    col[cur*4 + 3] = color;

    uv[cur*4 + 0]  = glm::vec2(inf.pos.x,              inf.pos.y);
    uv[cur*4 + 1]  = glm::vec2(inf.pos.x + inf.size.x, inf.pos.y);
    uv[cur*4 + 2]  = glm::vec2(inf.pos.x + inf.size.x, inf.pos.y + inf.size.y);
    uv[cur*4 + 3]  = glm::vec2(inf.pos.x,              inf.pos.y + inf.size.y);

    index[cur*6]     = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 3;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 2;
    index[cur*6 + 5] = cur*4 + 3;

    cur++;
}

void SpriteBatch::drawQuad(const glm::vec2 &loc, const glm::vec2 &size, const Texture &tex, const glm::vec4 &col_, const glm::vec4 &double_uv)
{
    if(current_program != basic_program)
    {
        render();
        current_program = basic_program;
        current_program->Use();
    }
    if(tex.textureId != current)
    {
        render();
        current = tex.textureId;
    }
    if(cur >= SIZE - 1)
        render();

    pos[cur*4]     = glm::vec3(loc.x,          loc.y,          0);
    pos[cur*4 + 1] = glm::vec3(loc.x + size.x, loc.y,          0);
    pos[cur*4 + 2] = glm::vec3(loc.x + size.x, loc.y + size.y, 0);
    pos[cur*4 + 3] = glm::vec3(loc.x,          loc.y + size.y, 0);

    col[cur*4]     = col_;
    col[cur*4 + 1] = col_;
    col[cur*4 + 2] = col_;
    col[cur*4 + 3] = col_;

    uv[cur*4]      = double_uv.xy();
    uv[cur*4 + 1]  = double_uv.zy();
    uv[cur*4 + 2]  = double_uv.zw();
    uv[cur*4 + 3]  = double_uv.xw();

    index[cur*6]     = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 3;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 2;
    index[cur*6 + 5] = cur*4 + 3;

    cur++;
}

void SpriteBatch::drawQuadAtlas(const glm::vec2 &loc, const glm::vec2 &size, const Texture &tex, int apos, const glm::vec4 &col_)
{
    if(current_program != basic_program)
    {
        render();
        current_program = basic_program;
        current_program->Use();
    }
    if(tex.textureId != current)
    {
        render();
        current = tex.textureId;
    }
    if(cur >= SIZE - 1)
        render();

    pos[cur*4]     = glm::vec3(loc.x,          loc.y,          0);
    pos[cur*4 + 1] = glm::vec3(loc.x + size.x, loc.y,          0);
    pos[cur*4 + 2] = glm::vec3(loc.x + size.x, loc.y + size.y, 0);
    pos[cur*4 + 3] = glm::vec3(loc.x,          loc.y + size.y, 0);

    col[cur*4]     = col_;
    col[cur*4 + 1] = col_;
    col[cur*4 + 2] = col_;
    col[cur*4 + 3] = col_;

    float sx = 32 / (float) tex.width;
    float sy = 32 / (float) tex.height;
    int inrow = tex.width / 32;
    float x = (apos % inrow) * sx;
    float y = (apos / inrow) * sy;

    uv[cur*4]      = glm::vec2(x,      y);
    uv[cur*4 + 1]  = glm::vec2(x + sx, y);
    uv[cur*4 + 2]  = glm::vec2(x + sx, y + sy);
    uv[cur*4 + 3]  = glm::vec2(x,      y + sy);

    index[cur*6]     = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 3;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 2;
    index[cur*6 + 5] = cur*4 + 3;

    cur++;
}

void SpriteBatch::drawLine(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color)
{
    if(cur >= SIZE - 1)
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


    pos[cur*4]     = glm::vec3(p[0].x, p[0].y, 0);
    pos[cur*4 + 1] = glm::vec3(p[1].x, p[1].y, 0);
    pos[cur*4 + 2] = glm::vec3(p[2].x, p[2].y, 0);
    pos[cur*4 + 3] = glm::vec3(p[3].x, p[3].y, 0);

    col[cur*4]     = color;
    col[cur*4 + 1] = color;
    col[cur*4 + 2] = color;
    col[cur*4 + 3] = color;

    uv[cur*4]      = glm::vec2(0, 0);
    uv[cur*4 + 1]  = glm::vec2(1, 0);
    uv[cur*4 + 2]  = glm::vec2(1, 1);
    uv[cur*4 + 3]  = glm::vec2(0, 1);

    index[cur*6]     = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 2;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 3;
    index[cur*6 + 5] = cur*4 + 2;

    cur++;
}

void SpriteBatch::drawAALine(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color)
{
    if(cur >= SIZE - 4)
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


    pos[cur*4]     = glm::vec3(p_left[0].x, p_left[0].y, 0);
    pos[cur*4 + 1] = glm::vec3(p_left[1].x, p_left[1].y, 0);
    pos[cur*4 + 2] = glm::vec3(p_left[2].x, p_left[2].y, 0);
    pos[cur*4 + 3] = glm::vec3(p_left[3].x, p_left[3].y, 0);

    pos[cur*4 + 4] = glm::vec3(p_center[0].x, p_center[0].y, 0);
    pos[cur*4 + 5] = glm::vec3(p_center[1].x, p_center[1].y, 0);
    pos[cur*4 + 6] = glm::vec3(p_center[2].x, p_center[2].y, 0);
    pos[cur*4 + 7] = glm::vec3(p_center[3].x, p_center[3].y, 0);

    pos[cur*4 + 8]  = glm::vec3(p_right[0].x, p_right[0].y, 0);
    pos[cur*4 + 9]  = glm::vec3(p_right[1].x, p_right[1].y, 0);
    pos[cur*4 + 10] = glm::vec3(p_right[2].x, p_right[2].y, 0);
    pos[cur*4 + 11] = glm::vec3(p_right[3].x, p_right[3].y, 0);

    col[cur*4 + 0] = tr;
    col[cur*4 + 1] = tr;
    col[cur*4 + 2] = color;
    col[cur*4 + 3] = color;

    col[cur*4 + 4] = color;
    col[cur*4 + 5] = color;
    col[cur*4 + 6] = color;
    col[cur*4 + 7] = color;

    col[cur*4 + 8]  = color;
    col[cur*4 + 9]  = color;
    col[cur*4 + 10] = tr;
    col[cur*4 + 11] = tr;

    uv[cur*4]      = glm::vec2(0, 0);
    uv[cur*4 + 1]  = glm::vec2(1, 0);
    uv[cur*4 + 2]  = glm::vec2(1, 1);
    uv[cur*4 + 3]  = glm::vec2(0, 1);
    uv[cur*4 + 4]  = glm::vec2(0, 0);
    uv[cur*4 + 5]  = glm::vec2(1, 0);
    uv[cur*4 + 6]  = glm::vec2(1, 1);
    uv[cur*4 + 7]  = glm::vec2(0, 1);
    uv[cur*4 + 8]  = glm::vec2(0, 0);
    uv[cur*4 + 9]  = glm::vec2(1, 0);
    uv[cur*4 + 10]  = glm::vec2(1, 1);
    uv[cur*4 + 11]  = glm::vec2(0, 1);

    index[cur*6]     = cur*4;
    index[cur*6 + 1] = cur*4 + 1;
    index[cur*6 + 2] = cur*4 + 2;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 3;
    index[cur*6 + 5] = cur*4 + 2;

    index[cur*6 + 6] = cur*4     + 4;
    index[cur*6 + 7] = cur*4 + 1 + 4;
    index[cur*6 + 8] = cur*4 + 2 + 4;
    index[cur*6 + 9] = cur*4 + 1 + 4;
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*cur*4, &pos[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(p_loc);
    glVertexAttribPointer(p_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*cur*4, &uv[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(uv_loc);
    glVertexAttribPointer(uv_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*cur*4, &col[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(c_loc);
    glVertexAttribPointer(c_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*cur*6, &index[0], GL_STREAM_DRAW);

    glDrawElements(GL_TRIANGLES, cur*6, GL_UNSIGNED_SHORT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_CULL_FACE);

    dc++;
    cur = 0;
}

void SpriteBatch::reduceScissor(glm::vec2 loc, glm::vec2 size)
{
    loc = scis_min = glm::max(loc, scis_min);
    size = scis_size = glm::min(size, scis_size);

    render();
    glEnable(GL_SCISSOR_TEST);
    glScissor(loc.x, RESY - (loc.y + size.y), GLsizei(size.x), GLsizei(size.y));
}

void SpriteBatch::resetScissor()
{
    render();
    glDisable(GL_SCISSOR_TEST);
    scis_min = {0,0};
    scis_size = {RESX, RESY};
}
