#ifndef BASICJARGSHADER_H
#define BASICJARGSHADER_H
#include "shader.h"


/*!
 * \brief The BasicJargShader class
 *
 * Shader for typical programs
 * automaticaly search basic uniforms
 */
class BasicJargShader : public Shader
{
public:
    BasicJargShader();
    ~BasicJargShader();

    GLuint ambient_location,
    diffuse_location,
    specular_location,
    emission_location,
    shininess_location,

    texture_location,
    normal_location,
    height_location,
    global_height_location,
    grad_location,

    detail_location,

    low_location,
    medium_location,
    high_location,
    side_location,

    mat_model_location,
    mat_viewProjection_location,
    viewPosition_location,
    mat_normal_location,
    lightPosition_location;

    void Afterlink();
private:
    GLint locate(const std::string &s);
};

#endif // BASICJARGSHADER_H
