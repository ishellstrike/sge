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

    Texture();
    Texture(GLuint id);
    ~Texture();

    void Load(const std::string &a, bool smooth = false, bool mip = false);
    void Load(const Pixmap &a, bool smooth = false, bool mip = false);
    //void Load(QImage *a, bool smooth = false, bool mip = false);
    void Empty(const glm::vec2 &size, GLuint dim = GL_TEXTURE_2D, GLuint format = GL_RGBA);
    void CreateDepth(glm::vec2 &size);
    void IdOnly();
};

#endif // JTEXTURE_H
