#include "spritebatch.h"
#include "logger.h"
#include <locale>
#include "utfcpp/utf8.h"
#include "prefecences.h"
#include <GL/glew.h>

typedef std::codecvt<wchar_t, char, mbstate_t> cvt;

//#include <unistd.h>

SpriteBatch::SpriteBatch()
{
    index = new GLuint[SIZE*6];
    uv = new glm::vec2[SIZE*4];
    pos = new glm::vec3[SIZE*4];
    col = new glm::vec4[SIZE*4];

    //char buf[255];
    //getcwd(buf, 255);

    //LOG(info) << buf;

    basic_program->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/basic.glsl");
    basic_program->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/basic.glsl");
    basic_program->Link();
    basic_program->Use();
    basic_program->locateVar("MVP");
    basic_program->locateVar("colorTexture");
    basic_program->Afterlink();
    glUniform1i(basic_program->vars[1], 0);

    font_program->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/font.glsl");
    font_program->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/font.glsl");
    font_program->Link();
    font_program->Use();
    font_program->locateVar("MVP");
    font_program->locateVar("colorTexture");
    font_program->Afterlink();
    glUniform1i(font_program->vars[1], 0);

    color_program->loadShaderFromSource(GL_VERTEX_SHADER, "data/shaders/color.glsl");
    color_program->loadShaderFromSource(GL_FRAGMENT_SHADER, "data/shaders/color.glsl");
    color_program->Link();
    color_program->Use();
    color_program->locateVar("MVP");
    color_program->Afterlink();

    current_program = basic_program;

    glUseProgram(0);

    glGenBuffers(4, m_vbo);
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
    drawText(text32, pos.x, pos.y + font->spacing, font, col_);
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
        drawText(text32, half.x, half.y + font->spacing, font, col_);
        break;
    case ALIGN_HOR_CENTER:
        half = pos + size / 2.f;
        half -= a / 2.f;
        drawText(text32, half.x, pos.y + font->spacing, font, col_);
        break;
    default: //LEFT
        drawText(text32, pos.x, pos.y + font->spacing, font, col_);
        break;
    }
}

glm::vec2 SpriteBatch::measureText(const std::string &text, Font *font)
{
    std::u32string text32;
    utf8::utf8to32(text.begin(), text.end(), std::back_inserter(text32));
    return drawText(text32, 0, 0, font, glm::vec4(0), true);
}

glm::vec2 SpriteBatch::drawText(const std::u32string &text32, int x, int y,
                                Font *font, const glm::vec4 &col_, bool no_draw)
{
    float x_start = x;
    float y_start = y;
    float x_max = 0;
    const char32_t *p;

    for(p = &text32[0]; *p; p++)
    {
        Font::CharInfo cc = font->chars[*p];
        if(*p == '\n')
        {
            y += font->spacing;
            x = x_start;
            continue;
        }

        if(!no_draw)
            drawQuadText(glm::vec2(x, y), cc, *font->font, col_);

        x += cc.advance.x;
        x_max = glm::max(static_cast<float>(x), x_max);
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

void SpriteBatch::drawQuad(const glm::vec2 &loc, const glm::vec2 &size, const Texture &tex, const glm::vec4 &col_)
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

    uv[cur*4]      = glm::vec2(0, 0);
    uv[cur*4 + 1]  = glm::vec2(1, 0);
    uv[cur*4 + 2]  = glm::vec2(1, 1);
    uv[cur*4 + 3]  = glm::vec2(0, 1);

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


    pos[cur*4]     = glm::vec3(s.x,         s.y + 2,     0);
    pos[cur*4 + 1] = glm::vec3(s.x + width, s.y,     0);
    pos[cur*4 + 2] = glm::vec3(e.x + width, e.y + 2, 0);
    pos[cur*4 + 3] = glm::vec3(e.x,         e.y,     0);

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
    index[cur*6 + 2] = cur*4 + 3;
    index[cur*6 + 3] = cur*4 + 1;
    index[cur*6 + 4] = cur*4 + 2;
    index[cur*6 + 5] = cur*4 + 3;

    cur++;
}

void SpriteBatch::render()
{
    if(cur == 0)
        return;
    current_program->Use();
    glUniformMatrix4fv(current_program->vars[0], 1, GL_FALSE, &uniform[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, current);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*cur*4, &pos[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(current_program->posAttrib);
    glVertexAttribPointer(current_program->posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*cur*4, &uv[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(current_program->uvAttrib);
    glVertexAttribPointer(current_program->uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*cur*4, &col[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(current_program->colAttrib);
    glVertexAttribPointer(current_program->colAttrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*cur*6, &index[0], GL_STREAM_DRAW);

    glDrawElements(GL_TRIANGLES, cur*6, GL_UNSIGNED_INT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

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
