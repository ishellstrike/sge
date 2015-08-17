#include "TextureGenerator.h"
#include <stdarg.h>
#include "helper.h"
#include "geometry/quad.h"
#include "geometry/mesh.h"
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

/*!
 * \brief TextureGenerator::RenderOnTempFbo
 * \param func
 */
void TextureGenerator::RenderOnTempFbo(std::function<void()> func) const
{
    FrameBuffer fbo;
    fbo.bindTexture(*target);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo.FBO); 
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glDisable(GL_BLEND);
    glViewport(0, 0, target->width, target->height);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,1);
    shader->Use();

    for (int i=0; i<texes.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, texes[i]->textureId);
        std::string str = "inputTex";
        str.append(std::to_string(i));
        auto uni = glGetUniformLocation(shader->program, str.c_str());
        if(uni == -1)
            LOG(error) << "Texture generator could't found " << str << " sampler in " << shader->name << ". skipped.";
        glUniform1i(uni, i);
    }

    for(int i=0; i < named_textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i + texes.size());
        glBindTexture(GL_TEXTURE_2D, named_textures[i].second->textureId);
        const char *s = named_textures[i].first.c_str();
        auto uni = glGetUniformLocation(shader->program, s);
        if(uni == -1)
            LOG(error) << "Texture generator could't found " << named_textures[i].first << " sampler in " << shader->name << ". skipped.";
        glUniform1i(uni, i + texes.size());
    }

    //TODO: определение параметра
    for (int i=0; i<params.size(); i++)
    {
        std::string str = "param";
        str.append(std::to_string(i));
        auto uni = glGetUniformLocation(shader->program, str.c_str());
        if(uni == -1)
            LOG(error) << "Texture generator could't found " << str << " uniform in " << shader->name << ". skipped.";
        glUniform1f(uni, params[i]);
    }
    
    func();

    const auto &quad_mesh = Quad::getMesh(2);
    quad_mesh->shader = shader;
    quad_mesh->Bind();
    Camera c;
    quad_mesh->Render(c);
    glViewport(0, 0, RESX, RESY);

//    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//    switch(status)
//    {
//    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
//        LOG(fatal) << "FATAL FRAMEBUFFER ERROR : GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
//        break;
//    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
//        LOG(fatal) << "FATAL FRAMEBUFFER ERROR : GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
//        break;
//    case GL_FRAMEBUFFER_UNSUPPORTED:
//        LOG(fatal) << "FATAL FRAMEBUFFER ERROR : GL_FRAMEBUFFER_UNSUPPORTED";
//        break;
//    default:
//        if (status != GL_FRAMEBUFFER_COMPLETE)
//            LOG(fatal) << "FATAL FRAMEBUFFER ERROR #" << status;
//        break;
//    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
