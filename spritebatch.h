#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H
#include "jtexture.h"
#include <string>
#include <memory>
#include "jargshader.h"

#include <GL/glew.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <freetype2/config/ftheader.h>

#define SIZE 10000
class SpriteBatch
{
public:
    SpriteBatch();
    ~SpriteBatch();

    glm::vec3 *pos = nullptr;
    glm::vec2 *uv = nullptr;
    glm::vec4 *col = nullptr;

    GLuint *index = nullptr;
    unsigned int cur = 0;

    GLuint current = 0;
    GLuint m_vbo[4];

    std::shared_ptr<Texture> font;
    std::shared_ptr<Texture> fontatlas;

    glm::mat4 uniform;

    std::shared_ptr<JargShader> font_program = std::make_shared<JargShader>(),
                                basic_program = std::make_shared<JargShader>(),
                                color_program = std::make_shared<JargShader>(),
                                current_program;

    void initFreeType();
    void setUniform(const glm::mat4 &uni);

    glm::vec2 renderText(const char *text, float x, float y, float sx, float sy, const glm::vec4 &col_);

    void render();
    void drawRect(const glm::vec2 &loc, const glm::vec2 &size, const glm::vec4 &_col);
    glm::vec2 renderAtlas();
    void drawQuad(const glm::vec2 &loc, const glm::vec2 &size, const Texture &tex, const glm::vec4 &col_);
    void drawQuadAtlas(const glm::vec2 &loc, const glm::vec2 &size, const Texture &tex, int apos, const glm::vec4 &col_);
    void drawLine(const glm::vec2 &start, const glm::vec2 &end, float width, const glm::vec4 &color);
private:
    FT_Face m_ftFace;
    void drawQuadText(const glm::vec2 &loc, const glm::vec2 &size, const Texture &tex, const glm::vec4 &color);
    //glm::vec2 renderText(const char *text, float x, float y, float sx, float sy, glm::vec4 &color);
};
#endif // SPRITEBATCH_H
