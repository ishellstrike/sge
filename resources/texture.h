/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef JTEXTURE_H
#define JTEXTURE_H
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "pixmap.h"

struct Texture {
public:
    GLuint textureId = 0;
    std::string name = "empty";
    unsigned int height = 0;
    unsigned int width = 0;
    unsigned int zsize = 0;
    bool mip = false;

    Texture();
    Texture(glm::vec2 __size, bool smooth = false, bool _mip = false, GLuint dim = GL_TEXTURE_2D, GLuint internal_format = GL_RGBA, GLuint type = GL_UNSIGNED_BYTE, GLuint format = GL_RGBA);
    ~Texture();
    GLuint m_dim = -1;

    void Load(const std::string &a, bool smooth = false, bool mip = false);
    void Load(const Pixmap &a, bool smooth = false, bool mip = false);
    //void Load(QImage *a, bool smooth = false, bool mip = false);
    void Empty(const glm::vec2 &size, bool smooth = false, bool _mip = false, GLuint dim = GL_TEXTURE_2D, GLuint internal_format = GL_RGBA, GLuint type = GL_UNSIGNED_BYTE, GLuint format = GL_RGBA);
    void CreateDepth(glm::vec2 &size);
    void IdOnly();
    void GenMipmap();
    void Bind(GLuint slot);
};

#endif // JTEXTURE_H
