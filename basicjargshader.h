#ifndef BASICJARGSHADER_H
#define BASICJARGSHADER_H
#include "shader.h"

class BasicJargShader : public JargShader
{
public:
    BasicJargShader();
    ~BasicJargShader();

    GLuint ambient_location;
    GLuint diffuse_location;

    GLuint specular_location;

    GLuint emission_location;

    GLuint shininess_location;


    GLuint texture_location;

    GLuint normal_location;
};

#endif // BASICJARGSHADER_H
