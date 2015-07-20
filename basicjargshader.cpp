#include "basicjargshader.h"
#include "logger.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

BasicJargShader::BasicJargShader()
{

}

BasicJargShader::~BasicJargShader()
{

}

GLint BasicJargShader::locate(const std::string &s)
{
    GLint a = glGetUniformLocation(program, s.c_str());
    if(a >= 0)
        LOG(verbose) << s << " located in " << a;
    else
        LOG(verbose) << s << " missed";
    return a;
}

void BasicJargShader::Afterlink()
{
    Use();
    ambient_location = locate("material_ambient");
    diffuse_location = locate("material_diffuse");
    specular_location = locate("material_specular");
    emission_location = locate("material_emission");
    shininess_location = locate("material_shininess");

    texture_location = locate("material_texture");
    normal_location = locate("material_normal");
    height_location = locate("material_height");
    global_height_location = locate("material_global_height");
    grad_location = locate("material_grad");

    glUniform1i(texture_location, 0);
    glUniform1i(normal_location, 1);
    glUniform1i(height_location, 2);
    glUniform1i(grad_location, 3);

    mat_model_location = locate("transform_M");
    mat_viewProjection_location = locate("transform_VP");
    viewPosition_location = locate("transform_viewPos");
    mat_normal_location = locate("transform_N");
    lightPosition_location = locate("transform_lightPos");

    glUniformMatrix4fv(viewPosition_location,  1, GL_FALSE,  &glm::translate(glm::mat4(1), glm::vec3(2.f,2.f,2.f))[0][0]);

    JargShader::Afterlink();
}
