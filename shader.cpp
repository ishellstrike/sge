#include "shader.h"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include "logger.h"
#include "helper.h"
#include <map>
#include <regex>
#include "sge_fielsystem.h"

bool printLog(GLuint id)
{
    char infoLog[1024];
    int infologLength = 0;
    if (glIsShader(id))
        glGetShaderInfoLog(id, 1024, &infologLength, infoLog);
    else
        glGetProgramInfoLog(id, 1024, &infologLength, infoLog);

    if (infologLength > 0)
    {
        LOG(error) << infoLog;
        return false;
    }
    else
    {
        LOG(verbose) << "     no errors";
        return true;
    }
}

JargShader::JargShader() :
    has_header(false)
{
    program = glCreateProgram();
}

JargShader::~JargShader(void)
{
    while(!shaders_.empty()) {
        glDeleteShader(shaders_.back());
        //LOG(verbose) << "Deleting shader " << std::to_string(shaders_.back());
        shaders_.pop_back();
    }
    glDeleteProgram(program);
    //LOG(verbose) << string_format("Deleting program %i", program);
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

std::map<int, std::string> shader_defines = {
         std::make_pair(GL_FRAGMENT_SHADER, "#define _FRAGMENT_"),
         std::make_pair(GL_VERTEX_SHADER, "#define _VERTEX_"),
         std::make_pair(GL_GEOMETRY_SHADER, "#define _GEOMETRY_"),
         std::make_pair(GL_TESS_EVALUATION_SHADER, "#define _TESSEVAL_"),
         std::make_pair(GL_TESS_CONTROL_SHADER, "#define _TESSCONTROL_")
        };

/*!
 * \brief JargShader::loadShaderFromSource
 * \param type shader type
 * \param source
 *
 * load typed shader from text file
 */
void JargShader::loadShaderFromSource(GLenum type, const std::string &source, const std::string &version/* = GLSLVER*/) {

    std::stringstream ss;
    name = source;

    ss << version << std::endl;

    ss << shader_defines[type] << std::endl;

    ss << preprocessIncludes(source);

    std::string str = ss.str();
    //LOG(verbose) << str;

    int length = str.length();
    const char *data = str.c_str();

    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const char **)&data, &length);
    glCompileShader(id);

    LOG(verbose) << source << " file " << shader_defines[type];
    bool has_error = !printLog(id);
    if(has_error)
    {
        int num = GetLastPatternedFilenameNubmer("shader_error_log", ".txt");
        std::string f_name = string_format("shader_error_log%d.txt", num+1);
        LOG(error) << "shader error detail saveid in " << f_name;
        std::stringstream out_file;
        out_file << str;
        char infoLog[1024];
        int infologLength = 0;
        glGetProgramInfoLog(id, 1024, &infologLength, infoLog);
        out_file << std::endl << "=======ERROR======" << std::endl << infoLog;
        SaveTextFile(f_name, out_file.str());
    }

    glAttachShader(program, id);
    shaders_.push_back(id);
}

std::string get_dir(std::string path)
{
    return path.substr(0, path.find_last_of('/') + 1);
}

std::string JargShader::preprocessIncludes(const std::string &filename, int level /*= 0 */ )
{
    //PrintIndent();
    if(level > 32)
        LOG(fatal) << "header inclusion depth limit reached, might be caused by cyclic header inclusion";

    static const std::regex re("^[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
    std::stringstream input;
    std::stringstream output;

    input << LoadTextFile(filename);

    size_t line_number = 1;
    std::smatch matches;

    std::string line;
    while(std::getline(input, line))
    {
        if (std::regex_search(line, matches, re))
        {
            std::string include_file = matches[1];

            output << preprocessIncludes(get_dir(filename) + include_file, level + 1) << std::endl;
        }
        else
        {
            output <<  line << std::endl;
        }
        ++line_number;
    }
    //PrintUnindent();
    return output.str();
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
    uv2Attrib = glGetAttribLocation(program, "texcoord2");
    colAttrib = glGetAttribLocation(program, "color");
    normAttrib = glGetAttribLocation(program, "normal");
    tangentAttrib = glGetAttribLocation(program, "tangent");
    binormalAttrib = glGetAttribLocation(program, "binormal");

    LOG(verbose) << "pos = " << posAttrib <<
                 "; col = " << colAttrib <<
                 "; uv = " << uvAttrib <<
                 "; uv2 = " << uv2Attrib <<
                 "; norm = " << normAttrib <<
                 "; tan = " << tangentAttrib <<
                 "; bi = " << binormalAttrib << ";";
}
