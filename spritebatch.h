/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>
#include <memory>

#include "basicjargshader.h"
#include "unordered_map"
#include "font.h"
#include "json/json.h"
#include "textureatlas.h"

#define BUFFER_SIZE 32000
struct Vtpc
{
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec4 col;
};

class SpriteBatch
{
public:

    enum ALIGN{
        ALIGN_LEFT,
        ALIGN_RIGHT,
        ALIGN_CENTER,
        ALIGN_HOR_CENTER
    };

    SpriteBatch();
    ~SpriteBatch();

    void render();
    void drawRect(const glm::vec2 &loc, const glm::vec2 &size, const glm::vec4 &_col);
    void drawQuad(const glm::vec2 &loc, const glm::vec2 &size, const Texture &tex, const glm::vec4 &col_, const glm::vec4 &double_uv = glm::vec4(0,0,1,1));
    void drawQuadAtlas(const glm::vec2 &loc, const glm::vec2 &size, const AtlasPart &tex, int apos, const glm::vec4 &col_);
    void drawQuad(const glm::vec2 &loc, const glm::vec2 &size, GLuint tex_id, const glm::vec4 &col_, const glm::vec4 &double_uv = glm::vec4(0,0,1,1));
    void drawLine(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color);
    void drawText(const std::string &text, const glm::vec2 &pos, Font *font, const glm::vec4 &col_);
    void drawText(const std::string &text, const glm::vec2 &pos, const glm::vec2 &size, Font *font, const glm::vec4 &col_, ALIGN align = ALIGN_LEFT);
    void reduceScissor(glm::vec2 loc, glm::vec2 size);
    void resetScissor();
    glm::vec2 measureText(const std::string &text, Font *font);
    void drawAALine(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color);
    glm::vec2 drawFormatted(const std::string &, glm::vec2, Font *);

    void drawTriangle(const glm::vec2 &loc, const glm::vec2 &loc2, const glm::vec2 &loc3, const glm::vec4 &col_);

	glm::mat4 uniform;

private:
    glm::vec2 drawText(const std::u32string &text32, float x, float y,
                       Font *font, const glm::vec4 &col_, bool no_draw);
    void drawQuadText(const glm::vec2 &loc, const Font::CharInfo &inf, const Texture &tex, const glm::vec4 &color);

    std::shared_ptr<BasicJargShader> font_program,
                                     basic_program,
                                     color_program,
                                     current_program,
                                     defered_jarg_program;
    glm::vec2 scis_min, scis_size;
    std::vector<Vtpc> vertices;

    std::vector<GLushort> index;
    unsigned int cur = 0;

    GLuint current = 0;
    GLuint normals = 0, outlines = 0;
	GLuint m_vbo[1];
    GLuint m_vao = 0;
};
#endif // SPRITEBATCH_H
