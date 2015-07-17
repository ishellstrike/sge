#include "TextureGenerator.h"
#include <stdarg.h>
#include "helper.h"
#include "geometry/quad.h"
#include "geometry/mesh.h"
#include "BasicJargShader.h"
#include <functional>
#include "prefecences.h"

// TODO: наследуемые классы текстурного генератора, с перегрузкой функции OtherUniforms, котора€ вызываетс€ перед рендером
TextureGenerator::TextureGenerator(void)
{
}


TextureGenerator::~TextureGenerator(void)
{
}

void TextureGenerator::SetTextures(std::shared_ptr<Texture> tex)
{
    texes.push_back(tex);
}

void TextureGenerator::Reset(){
    texes.clear();
    params.clear();
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
 * ¬ходные текстуры должны именоватьс€  inputTex0 inputTex1 ...
 * ¬ходные юниформы param0 param1 ...
 * \param func
 */
void TextureGenerator::RenderOnTempFbo(std::function<void()> func) const
{
    FrameBuffer fbo;
    fbo.bindTexture(*target);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo.FBO); 
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
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
        glUniform1i(uni, 0);
    }

    //TODO: определение параметра
    for (int i=0; i<params.size(); i++)
    {
        std::string str = "param";
        str.append(std::to_string(i));
        auto uni = glGetUniformLocation(shader->program, str.c_str());
        glUniform1f(uni, params[i]);
    }
    
    func();

    auto quad_mesh = Quad::getMesh(2);
    quad_mesh->shader = shader;
    quad_mesh->Bind();
    Camera c;
    quad_mesh->Render(c);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, RESX, RESY);
}
