#include "texture.h"
#include "logger.h"
#include "pixmap.h"

Texture::Texture()
{
}

Texture::Texture(GLuint id) :
    textureId(id),
    name("fromID " + std::to_string(id))
{
}

Texture::~Texture()
{
    if(textureId != -1){
        glDeleteTextures(1, &textureId);
        textureId = -1;
    }
}

/*!
 * \brief Texture::Load Загружает заданное изображение png как битовую карту, а затем загружает его в видеопамять
 * \param a Имя изображения *.png
 * \param smooth параметр интерполяции текселей (true - GL_LINEAR, false - GL_NEAREST)
 * \param mip параметр генерации мип-карт
 */
void Texture::Load(const std::string &a, bool smooth, bool mip)
{
    Pixmap p(a);
    auto slpos = a.find_last_of('/');
    name = a.substr(slpos, a.length() - slpos);
    Load(p, smooth, mip);
}

/*!
 * \brief Texture::Load Выполняет загрузку битовой карты в видеопамять
 * \param a Битовая карта
 * \param smooth параметр интерполяции текселей (true - GL_LINEAR, false - GL_NEAREST)
 * \param mip параметр генерации мип-карт
 */
void Texture::Load(const Pixmap &a, bool smooth, bool mip)
{
    width = a.width;
    height = a.height;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &a.data[0]);
    if(smooth)
    {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}

/*!
 * \brief Texture::Empty Создает в видеопамяти @dim мерную текстуру в цветовом пространстве @format
 * \param size размер текстуры
 * \param dim размерность (GL_TEXTURE_2D)
 * \param format цветовое пространство (GL_RGBA)
 */
void Texture::Empty(const glm::vec2 &size, GLuint dim /*= GL_TEXTURE_2D*/, GLuint format /*= GL_RGBA*/)
{
    width = (int) size.x;
    height = (int) size.y;
    name = "empty_texture";

    glGenTextures(1, &textureId);
    glBindTexture(dim, textureId);
    glTexImage2D(dim, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(dim, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(dim, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(dim, 0);
}

/*!
 * \brief Texture::IdOnly Генерирует новую текстуру не устанавливая никаких данных, только генерирует texture id
 */
void Texture::IdOnly()
{
    assert(!textureId);
    name = "id only";
    glGenTextures(1, &textureId);
}

void Texture::CreateDepth(glm::vec2 &size)
{
    width = (int) size.x;
    height = (int) size.y;
    name = "empty_depth";

    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glBindTexture(GL_TEXTURE_2D, 0);
}
