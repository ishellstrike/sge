/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

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
        LOG(trace) << s << " located in " << a;
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

    detail_location = locate("material_detail");

    low_location = locate("material_low");
    medium_location = locate("material_medium");
    high_location = locate("material_high");
    side_location = locate("material_side");

    glUniform1i(texture_location, 0);
    glUniform1i(normal_location, 1);
    glUniform1i(height_location, 2);
    glUniform1i(grad_location, 3);
    glUniform1i(global_height_location, 4);

    glUniform1i(detail_location, 5);

    glUniform1i(low_location, 6);
    glUniform1i(medium_location, 7);
    glUniform1i(high_location, 8);
    glUniform1i(side_location, 9);

    mat_model_location = locate("transform_M");
    mat_viewProjection_location = locate("transform_VP");
    viewPosition_location = locate("transform_viewPos");
    mat_normal_location = locate("transform_N");
    lightPosition_location = locate("transform_lightPos");

    glUniformMatrix4fv(viewPosition_location,  1, GL_FALSE,  &glm::translate(glm::mat4(1), glm::vec3(2.f,2.f,2.f))[0][0]);
}
