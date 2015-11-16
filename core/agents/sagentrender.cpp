#include "sagentrender.h"
#include "resources/resourcecontroller.h"

std::shared_ptr<SRenderSettings> SRenderSettings::basic = std::make_shared<SRenderSettings>();
std::shared_ptr<SRenderSettings> SRenderSettings::current = std::make_shared<SRenderSettings>();

template<typename _Ty>
void CheckSet(const _Ty &nval, _Ty &val, std::function<void()> func)
{
    if(val != nval)
    {
        func();
        val = nval;
    }
}

void SRenderSettings::Init(){
    std::shared_ptr<Material> m = std::make_shared<Material>();
    m->texture = Resources::instance()->Get<Texture>("error");
    SRenderSettings::basic->material = m;
    SRenderSettings::basic->shader = Resources::instance()->Get<BasicJargShader>("defered_basic_render");
}

void SRenderSettings::Set()
{

}

void SRenderSettings::Reset()
{
    current = std::make_shared<SRenderSettings>();
}

void SRenderAgent::Render(const Camera &cam)
{
    if(!settings->shader || !settings->material || !mesh)
        return;

    if(!mesh->mesh->vbo)
    {
        mesh->mesh->shader = settings->shader;
        mesh->mesh->Bind();
    }

    CheckSet(settings->shader, SRenderSettings::current->shader, [&](){
        settings->shader->Use();
        SRenderSettings::current->material = nullptr;
    });

    CheckSet(settings->material, SRenderSettings::current->material, [&](){

        settings->shader->Use();
        if(settings->material == nullptr)
            return;

        auto &shader = settings->shader;
        auto &mat = settings->material;

        shader->SetUniform("material_ambient",   mat->ambient);
        shader->SetUniform("material_diffuse",   mat->diffuse);
        shader->SetUniform("material_specular",  mat->specular);
        shader->SetUniform("material_emission",  mat->emission);
        shader->SetUniform("material_shininess", mat->shininess);

        if(GLEW_ARB_multi_bind)
        {
            auto texes = mat->GetTextureLocations();
            glBindTextures(0, texes.size(), &texes[0]);
        }
        else
        {
            if(mat->texture != nullptr) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, mat->texture->textureId);
            }
            if(mat->normal != nullptr) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, mat->normal->textureId);
            }
            if(mat->height != nullptr) {
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, mat->height->textureId);
            }
            if(mat->grad != nullptr) {
                glActiveTexture(GL_TEXTURE3);
                glBindTexture(GL_TEXTURE_2D, mat->grad->textureId);
            }
            if(mat->global_height != nullptr) {
                glActiveTexture(GL_TEXTURE4);
                glBindTexture(GL_TEXTURE_2D, mat->global_height->textureId);
            }


            if(mat->detail != nullptr) {
                glActiveTexture(GL_TEXTURE5);
                glBindTexture(GL_TEXTURE_2D, mat->detail->textureId);
            }
            if(mat->low != nullptr) {
                glActiveTexture(GL_TEXTURE6);
                glBindTexture(GL_TEXTURE_2D, mat->low->textureId);
            }
            if(mat->medium != nullptr) {
                glActiveTexture(GL_TEXTURE7);
                glBindTexture(GL_TEXTURE_2D, mat->medium->textureId);
            }
            if(mat->high != nullptr) {
                glActiveTexture(GL_TEXTURE8);
                glBindTexture(GL_TEXTURE_2D, mat->high->textureId);
            }
            if(mat->side != nullptr) {
                glActiveTexture(GL_TEXTURE9);
                glBindTexture(GL_TEXTURE_2D, mat->side->textureId);
            }

            OPENGL_CHECK_ERRORS();
        }
    });

    CheckSet(settings->wireframe, SRenderSettings::current->wireframe, [&](){
        if(settings->wireframe)
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        else
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    });

    mesh->mesh->Render();
}
