﻿/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "TextureGenerator.h"
#include <stdarg.h>
#include "helper.h"
#include "BasicJargShader.h"
#include <functional>
#include "prefecences.h"
#include "logger.h"

// TODO: наследуемые классы текстурного генератора, с перегрузкой функции OtherUniforms, которая вызывается перед рендером
TextureGenerator::TextureGenerator(void)
{
}


TextureGenerator::~TextureGenerator(void)
{
}

void TextureGenerator::PushBackTexture(std::shared_ptr<Texture> tex)
{
    texes.push_back(tex);
}

void TextureGenerator::AddTexture(std::string __name, std::shared_ptr<Texture> &a)
{
    named_textures.push_back(std::make_pair(__name, a));
}

void TextureGenerator::Reset(){
    texes.clear();
    params.clear();
    named_textures.clear();
}

void TextureGenerator::SetParams(float tex)
{
    params.push_back(tex);
}

void TextureGenerator::SetResultTexture(std::shared_ptr<Texture> _tex)
{
    target = _tex;
}

void TextureGenerator::SetShader(std::shared_ptr<BasicJargShader> _shader)
{
    shader = _shader;
}

void TextureGenerator::drawQuad(const glm::vec4 &view ) const
{
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0,0);
        glVertex2f(view.x, view.y);

        glTexCoord2f(1,0);
        glVertex2f(view.z, view.y);

        glTexCoord2f(0,1);
        glVertex2f(view.x, view.w);

        glTexCoord2f(1,1);
        glVertex2f(view.z, view.w);
    glEnd();
}

/*!
 * \brief TextureGenerator::RenderOnTempFbo
 * \param func
 */
void TextureGenerator::RenderOnTempFbo(std::function<void()> func) const
{
    FrameBuffer fbo;
    fbo.bindTexture(*target);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo.FBO);
    glDisable(GL_BLEND);
    glViewport(0, 0, target->width, target->height);
    shader->Use();

    for (size_t i=0; i<texes.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, texes[i]->textureId);
        std::string str = "inputTex";
        str.append(std::to_string(i));
        auto uni = glGetUniformLocation(shader->program, str.c_str());
        if(uni == -1)
            LOG(error) << "Texture generator could't found " << str << " sampler in " << shader->shaderfile_name << ". skipped.";
        glUniform1i(uni, i);
    }

    for(size_t i=0; i < named_textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i + texes.size());
        glBindTexture(GL_TEXTURE_2D, named_textures[i].second->textureId);
        const char *s = named_textures[i].first.c_str();
        auto uni = glGetUniformLocation(shader->program, s);
        if(uni == -1)
            LOG(error) << "Texture generator could't found " << named_textures[i].first << " sampler in " << shader->shaderfile_name << ". skipped.";
        glUniform1i(uni, i + texes.size());
    }

    //TODO: определение параметра
    for (size_t i=0; i<params.size(); i++)
    {
        std::string str = "param";
        str.append(std::to_string(i));
        auto uni = glGetUniformLocation(shader->program, str.c_str());
        if(uni == -1)
            LOG(error) << "Texture generator could't found " << str << " uniform in " << shader->shaderfile_name << ". skipped.";
        glUniform1f(uni, params[i]);
    }
    
    func();

    drawScreenQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, RESX, RESY);
}
