#include "texture.h"
#include "logger.h"
#include "pixmap.h"

Texture::Texture()
{
}

Texture::Texture(glm::vec2 __size,
                 bool smooth /*=false*/,
                 bool _mip /*=false*/,
                 GLuint dim /*= GL_TEXTURE_2D*/,
                 GLuint internal_format /*= GL_RGBA*/,
                 GLuint type /*=GL_UNSIGNED_BYTE*/,
                 GLuint format /*= GL_RGBA*/)
{
    m_dim = dim;
    Empty(__size, smooth, _mip, dim, internal_format, type, format);
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
    m_dim = GL_TEXTURE_2D;
    Load(p, smooth, mip);
}

/*!
 * \brief Texture::Load Выполняет загрузку битовой карты в видеопамять
 * \param a Битовая карта
 * \param smooth параметр интерполяции текселей (true - GL_LINEAR, false - GL_NEAREST)
 * \param mip параметр генерации мип-карт
 */
void Texture::Load(const Pixmap &a, bool smooth /*=false*/, bool _mip /*=false*/)
{
    width = a.width;
    height = a.height;

    glGenTextures(1, &textureId);
    glBindTexture(m_dim, textureId);
    glTexImage2D(m_dim, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &a.data[0]);


    if(_mip)
    {
        glTexParameteri(m_dim, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(m_dim, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
    }
    else
    {
        glTexParameteri(m_dim, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(m_dim, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST);
    }

    mip = _mip;
    if(_mip)
    {
        GenMipmap();
    }
}

/*!
 * \brief Texture::Empty Создает в видеопамяти @dim мерную текстуру в цветовом пространстве @format
 * \param size размер текстуры
 * \param dim размерность (GL_TEXTURE_2D)
 * \param format цветовое пространство (GL_RGBA)
 */
void Texture::Empty(const glm::vec2 &size, bool smooth /*=false*/, bool _mip /*=false*/, GLuint dim /*= GL_TEXTURE_2D*/, GLuint internal_format /*= GL_RGBA*/, GLuint type /*=GL_UNSIGNED_BYTE*/, GLuint format /*= GL_RGBA*/)
{
    width = (int) size.x;
    height = (int) size.y;
    name = "empty_texture";

    glGenTextures(1, &textureId);
    glBindTexture(dim, textureId);
    glTexImage2D(dim, 0, internal_format, size.x, size.y, 0, format, type, NULL);

    if(_mip)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, smooth ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, smooth ? GL_LINEAR : GL_NEAREST);
    }
    mip = _mip;
}

void Texture::GenMipmap()
{
    if(mip)
        glGenerateMipmap(GL_TEXTURE_2D);
    else
        LOG(error) << "trying to generating mipmap for non mipmap texture. skipped.";
}

void Texture::Bind(GLuint slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureId);
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
