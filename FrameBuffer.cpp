/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "FrameBuffer.h"
#include "logger.h"

FrameBuffer::FrameBuffer(bool _depth)
{
    depth = _depth;
    FBO = 0;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


FrameBuffer::~FrameBuffer(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &FBO);
}

void FrameBuffer::bindTexture(const Texture& tex, GLuint attach/*GL_COLOR_ATTACHMENT0*/)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, attach, tex.textureId, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void FrameBuffer::CheckErrors()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        LOG(error) << "FATAL FRAMEBUFFER ERROR : GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        LOG(error) << "FATAL FRAMEBUFFER ERROR : GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        LOG(error) << "FATAL FRAMEBUFFER ERROR : GL_FRAMEBUFFER_UNSUPPORTED";
        break;
    default:
        if (status != GL_FRAMEBUFFER_COMPLETE)
            LOG(error) << "FATAL FRAMEBUFFER ERROR #" << status;
        break;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
