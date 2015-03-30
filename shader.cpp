#include "shader.h"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include "logger.h"
#include "helper.h"

#define printLog(obj){int infologLength = 0; \
    char infoLog[1024]; \
    if (glIsShader(obj)) \
    glGetShaderInfoLog(obj, 1024, &infologLength, infoLog); \
else \
    glGetProgramInfoLog(obj, 1024, &infologLength, infoLog); \
    if (infologLength > 0) { \
    LOG(verbose) << infoLog; \
    } else { \
    LOG(verbose) << "     no errors"; \
    } }

JargShader::JargShader() :
    has_header(false)
{
    program = glCreateProgram();
}

JargShader::~JargShader(void)
{
    while(!shaders_.empty()) {
        glDeleteShader(shaders_.back());
        LOG(verbose) << "Deleting shader " << std::to_string(shaders_.back());
        shaders_.pop_back();
    }
    glDeleteProgram(program);
    LOG(verbose) << string_format("Deleting program %i", program);
}

/*!
 * \brief JargShader::Use
 */
void JargShader::Use() const
{
    glUseProgram(program);
}

/*!
 * \brief JargShader::locateVar
 * \param s
 * \return ogl id
 *
 * search uniform and store it location in #vars
 */
GLint JargShader::locateVar(const std::string &s)
{
    GLint a = glGetUniformLocation(program, s.c_str());
    if(a >= 0)
        LOG(verbose) << s << " located in " << a;
    else
        LOG(verbose) << s << " missed";
    vars.push_back(a);
    return a;
}

/*!
 * \brief JargShader::loadShaderFromSource
 * \param type shader type
 * \param source
 *
 * load typed shader from text file
 */
void JargShader::loadShaderFromSource(GLenum type,const std::string &source) {

    std::stringstream ss;
    name = source;
    std::string part_name;

    if(!has_header){
        ss << GLSLVER << std::endl;
    }

    switch(type)
    {
    case GL_FRAGMENT_SHADER:
        part_name = "#define _FRAGMENT_";
        break;
    case GL_VERTEX_SHADER:
        part_name = "#define _VERTEX_";
        break;
    case GL_GEOMETRY_SHADER:
        part_name = "#define _GEOMETRY_";
        break;
    case GL_TESS_EVALUATION_SHADER:
        part_name = "#define _TESSEVAL_";
        break;
    case GL_TESS_CONTROL_SHADER:
        part_name = "#define _TESSCONTROL_";
        break;
    default:
        part_name = "";
        break;
    }

    ss << part_name << std::endl;

    if(has_header) {
        ss << global_header;
    }
    std::ifstream file(source.c_str());
    std::string line;
    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            ss << line << std::endl;
        }
        file.close();
    } else {
        LOG(fatal) << string_format("%s %s", "Failed to open file ", source.c_str());
        return;
    }
    std::string str = ss.str();
    int length = str.length();
    const char *data = str.c_str();
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const char **)&data, &length);
    glCompileShader(id);
    LOG(verbose) << source << " file " << part_name;
    printLog(id);
    glAttachShader(program, id);
    shaders_.push_back(id);
}

/*!
 * \brief JargShader::Link
 * \return success
 *
 * link compiled shaders to program
 */
bool JargShader::Link() {
    glLinkProgram(program);
    LOG(verbose) << "Program " << std::to_string(program) << " linking";
    printLog(program);
    LOG(verbose) << "--------------------";
    return true;
}

/*!
 * \brief JargShader::Afterlink
 *
 * search basic uniforms
 * uniforms uses in #Mesh class
 */
void JargShader::Afterlink()
{
    Use();
    posAttrib = glGetAttribLocation(program, "position");
    uvAttrib = glGetAttribLocation(program, "texcoord");
    colAttrib = glGetAttribLocation(program, "color");
    normAttrib = glGetAttribLocation(program, "normal");
    tangentAttrib = glGetAttribLocation(program, "tangent");
    binormalAttrib = glGetAttribLocation(program, "binormal");

    LOG(verbose) << "pos = " << posAttrib <<
                 "; col = " << colAttrib <<
                 "; uv = " << uvAttrib <<
                 "; norm = " << normAttrib <<
                 "; tan = " << tangentAttrib <<
                 "; bi = " << binormalAttrib << ";";
}

/*!
 * \brief JargShader::PushGlobalHeader
 * \param source
 * \param version version string (e.g. "#version 330 core"
 *
 * rewrite standart version string and set shader header for each shader
 * can used for adding typical constants and functions to shaders
 */
void JargShader::PushGlobalHeader(const std::string &source, const char *version)
{
    std::stringstream ss;
    name = source;
    std::string part_name;
    if(!has_header) {
        ss << version << std::endl;
    } else {
        ss << global_header;
    }
    std::ifstream file(source.c_str());
    std::string line;
    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            ss << line << std::endl;
        }
        file.close();
    } else {
        LOG(fatal) << string_format("%s %s", "Failed to open file ", source.c_str());
        return;
    }
    global_header = ss.str();
    has_header = true;
}
