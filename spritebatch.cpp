#include "spritebatch.h"
#include "logger.h"
#include "prefecences.h"

//#include <unistd.h>

SpriteBatch::SpriteBatch() :
    font(std::make_shared<Texture>()),
    fontatlas(std::make_shared<Texture>())
{
    index = new GLuint[SIZE*6];
    uv = new glm::vec2[SIZE*4];
    pos = new glm::vec3[SIZE*4];
    col = new glm::vec4[SIZE*4];

    basic_program->loadShaderFromSource(GL_VERTEX_SHADER, Prefecences::getShadersDir() + "basic.glsl");
    basic_program->loadShaderFromSource(GL_FRAGMENT_SHADER, Prefecences::getShadersDir() + "basic.glsl");
    basic_program->Link();
    basic_program->Use();
    basic_program->locateVars("MVP");
    basic_program->locateVars("colorTexture");
    basic_program->getAttrib();
    glUniform1i(basic_program->vars[1], 0);

    font_program->loadShaderFromSource(GL_VERTEX_SHADER, Prefecences::getShadersDir() + "font.glsl");
    font_program->loadShaderFromSource(GL_FRAGMENT_SHADER, Prefecences::getShadersDir() + "font.glsl");
    font_program->Link();
    font_program->Use();
    font_program->locateVars("MVP");
    font_program->locateVars("colorTexture");
    font_program->getAttrib();
    glUniform1i(font_program->vars[1], 0);

    color_program->loadShaderFromSource(GL_VERTEX_SHADER, Prefecences::getShadersDir() + "color.glsl");
    color_program->loadShaderFromSource(GL_FRAGMENT_SHADER, Prefecences::getShadersDir() + "color.glsl");
    color_program->Link();
    color_program->Use();
    color_program->locateVars("MVP");
    color_program->getAttrib();

    current_program = basic_program;

    glUseProgram(0);

    glGenBuffers(4, m_vbo);


    font->IdOnly();

    fontatlas->Empty(glm::vec2(512, 512));
}

SpriteBatch::~SpriteBatch()
{
    delete[] index;
    delete[] uv;
    delete[] pos;
    delete[] col;

    glDeleteBuffers(4, m_vbo);
}

void SpriteBatch::initFreeType()
{
    FT_Library ft;
    if(FT_Init_FreeType(&ft))
    {
        LOG(fatal) << "could not init free type library.";
        return;
    }
    else
        LOG(fatal) << "FT init OK";
    char fontPath[] = Prefecences::getFontsDir() + "Inconsolata.otf";
    if(FT_New_Face(ft, fontPath, 0, &m_ftFace))
    {
        LOG(fatal) << "Could not open font " << fontPath;
        return;
    }
    else
        LOG(info) << "FT face OK";

    FT_Set_Pixel_Sizes(m_ftFace, 0, 48);
}

void SpriteBatch::setUniform(const glm::mat4 &uni)
{
    uniform = uni;
}


//in experiments mode
glm::vec2 SpriteBatch::renderText(const char *text, float x, float y, float sx, float sy, const glm::vec4 &col_)
{
    float x_start = x;
    float y_start = y;
    float x_max = 0;
    const char *p;
    FT_GlyphSlot ftGlyph = m_ftFace->glyph;

    for(p = text; *p; p++)
    {
        glBindTexture(GL_TEXTURE_2D, font->textureId);
        if(*p == '\n')
        {
            y+=ftGlyph->bitmap.rows;
            x=x_start;
            continue;
        }
        if(FT_Load_Char(m_ftFace, *p, FT_LOAD_RENDER))
        {
            LOG(error) << "Could not load character " << *p;
            continue;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ftGlyph->bitmap.width,
                     ftGlyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
                     ftGlyph->bitmap.buffer);

        float w = ftGlyph->bitmap.width * sx;
        float h = ftGlyph->bitmap.rows * sy;
        float x2 = x + ftGlyph->bitmap_left * sx;
        float y2 = y + 20 - ftGlyph->bitmap_top * sy;

        drawQuadText(glm::vec2(x2, y2), glm::vec2(w, h), *font, col_);
        render();

        x += (ftGlyph->advance.x >> 6) * sx;
        y += (ftGlyph->advance.y >> 6) * sy;
        x_max = glm::max(x, x_max);
    }
    return glm::vec2(x_max - x_start, y - y_start);
}

glm::vec2 SpriteBatch::renderAtlas()
{
    float x = 0, sx = 1;
    float y = 0, sy = 1;
    float x_start = x;
    float y_start = y;
    float x_max = 0;
    const char *p;
    FT_GlyphSlot ftGlyph = m_ftFace->glyph;
    char text[] = "`1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm,./~!@#$%^&*()_+QWERTYUIOP{}ASDFGHJKL:\"ZXCVBNM<>?\\|";

    for(p = text; *p; p++)
    {
        glBindTexture(GL_TEXTURE_2D, font->textureId);
        if(*p == '\n')
        {
            y+=ftGlyph->bitmap.rows;
            x=x_start;
            continue;
        }
        if(FT_Load_Char(m_ftFace, *p, FT_LOAD_RENDER))
        {
            LOG(error) << "Could not load character" << *p;
            continue;
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ftGlyph->bitmap.width,
                     ftGlyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
                     ftGlyph->bitmap.buffer);

        float w = ftGlyph->bitmap.width * sx;
        float h = ftGlyph->bitmap.rows * sy;
        float x2 = x + ftGlyph->bitmap_left * sx;
        float y2 = y + 20 - ftGlyph->bitmap_top * sy;

        drawQuadText(glm::vec2(x2, y2), glm::vec2(w, h), *font, glm::vec4(1,1,1,1));
        render();

        x += (ftGlyph->advance.x >> 6) * sx;
        y += (ftGlyph->advance.y >> 6) * sy;
        x_max = glm::max(x, x_max);
    }
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

void SpriteBatch::drawQuadText(const glm::vec2 &loc, const glm::vec2 &size, const Texture &tex, const glm::vec4 &color)
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

    pos[cur*4]     = glm::vec3(loc.x,          loc.y,          0);
    pos[cur*4 + 1] = glm::vec3(loc.x + size.x, loc.y,          0);
    pos[cur*4 + 2] = glm::vec3(loc.x + size.x, loc.y + size.y, 0);
    pos[cur*4 + 3] = glm::vec3(loc.x,          loc.y + size.y, 0);

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


    pos[cur*4]     = glm::vec3(s.x,         s.y,     0);
    pos[cur*4 + 1] = glm::vec3(s.x + width, s.y,     0);
    pos[cur*4 + 2] = glm::vec3(e.x + width, e.y + 1, 0);
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
