#ifndef GBUFFER_H
#define GBUFFER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GBuffer
{
public:

    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION = 0,
        GBUFFER_TEXTURE_TYPE_DIFFUSE = 1,
        GBUFFER_TEXTURE_TYPE_NORMAL = 2,
        GBUFFER_TEXTURE_TYPE_TEXCOORD = 3,
        GBUFFER_NUM_TEXTURES = 4
    };

    GBuffer();

    ~GBuffer();

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
    void Resize(unsigned int WindowWidth, unsigned int WindowHeight);

    void BindForWriting();

    void BindForReading();

    void BindForBlit();

    void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);
private:

    GLuint m_fbo = 0;
    GLuint m_textures[GBUFFER_NUM_TEXTURES] = {};
    GLuint m_depthTexture = 0;
};

#endif // GBUFFER_H
