#include "gbuffer.h"
#include "logger.h"

GBuffer::GBuffer()
{

}

GBuffer::~GBuffer()
{

}

bool GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

    glGenTextures(GBUFFER_NUM_TEXTURES, m_textures);
    glGenTextures(1, &m_depthTexture);

    Resize(WindowWidth, WindowHeight);

    GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

    glDrawBuffers(GBUFFER_NUM_TEXTURES, DrawBuffers);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
            LOG(fatal) << "Framebuffer error, status: 0x" << Status;
            return false;
    }
    LOG(info) << "Framebuffer OK";

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    return true;
}

void GBuffer::Resize(unsigned int WindowWidth, unsigned int WindowHeight)
{
    for (unsigned int i = 0 ; i < GBUFFER_NUM_TEXTURES ; i++) {
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
        GL_TEXTURE_2D, m_textures[i], 0);
    }

    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                                            NULL);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
}

void GBuffer::BindForWriting()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void GBuffer::BindForReading()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    for (unsigned int i = 0 ; i < GBUFFER_NUM_TEXTURES; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
    }
}

void GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}

