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
    Texture(GLuint id);
    Texture(glm::vec2 __size, bool smooth = false, bool _mip = false, GLuint dim = GL_TEXTURE_2D, GLuint format = GL_RGBA, GLuint type =GL_UNSIGNED_BYTE);
    ~Texture();

    void Load(const std::string &a, bool smooth = false, bool mip = false);
    void Load(const Pixmap &a, bool smooth = false, bool mip = false);
    //void Load(QImage *a, bool smooth = false, bool mip = false);
    void Empty(const glm::vec2 &size, bool smooth = false, bool _mip = false, GLuint dim = GL_TEXTURE_2D, GLuint format = GL_RGBA, GLuint type = GL_UNSIGNED_BYTE);
    void CreateDepth(glm::vec2 &size);
    void IdOnly();
    void GenMipmap();
};

#endif // JTEXTURE_H
