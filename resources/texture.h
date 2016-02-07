/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef JTEXTURE_H
#define JTEXTURE_H
#include <string>

#include "texturebase.h"
#include "pixmap.h"

#include <glm/glm.hpp>

struct Texture : public TextureBase {
public:
    Texture();
    Texture(glm::vec2 __size, bool smooth, bool _mip, GLuint dim, GLuint internal_format, GLuint type, GLuint format);
    Texture(glm::vec2 __size, bool smooth, bool _mip, GLuint dim, GLuint internal_format, GLuint type);
    Texture(glm::vec2 __size, bool smooth, bool _mip, GLuint dim, GLuint internal_format);
    Texture(glm::vec2 __size, bool smooth, bool _mip, GLuint dim);
    Texture(glm::vec2 __size, bool smooth, bool _mip);
    Texture(glm::vec2 __size, bool smooth);
    Texture(glm::vec2 __size);
    ~Texture();
    GLuint m_dim = -1;

    void Load(const std::string &a, bool smooth = false, bool mip = false);
    void Load(const Pixmap &a, bool smooth = false, bool mip = false);
    void Empty(const glm::vec2 &size, bool smooth = false, bool _mip = false, GLuint dim = GL_TEXTURE_2D, GLuint internal_format = GL_RGBA, GLuint type = GL_UNSIGNED_BYTE, GLuint format = GL_RGBA);
    void CreateDepth(glm::vec2 &size);
    void IdOnly();
    void GenMipmap();
    void Bind(GLuint slot);
};

#endif // JTEXTURE_H
