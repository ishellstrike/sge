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

Shader::Shader()
{
    program = glCreateProgram();
}

Shader::~Shader(void)
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
void Shader::Use() const
{
    glUseProgram(program);
}

std::map<int, std::string> shader_defines = {
         std::make_pair(GL_FRAGMENT_SHADER, "_FRAGMENT_"),
         std::make_pair(GL_VERTEX_SHADER, "_VERTEX_"),
         std::make_pair(GL_GEOMETRY_SHADER, "_GEOMETRY_"),
         std::make_pair(GL_TESS_EVALUATION_SHADER, "_TESSEVAL_"),
         std::make_pair(GL_TESS_CONTROL_SHADER, "_TESSCONTROL_")
        };

std::string get_dir(std::string path)
{
    return path.substr(0, path.find_last_of('/') + 1);
}

std::string get_name(std::string path)
{
    return path.substr(path.find_last_of('/') + 1);
}

std::string get_filename_headername(std::string path)
{
    std::replace(path.begin(), path.end(), '.', '_');
    std::transform(path.begin(), path.end(), path.begin(), toupper);
    return path;
}

/*!
 * \brief JargShader::loadShaderFromSource
 * \param type shader type
 * \param source
 *
 * load typed shader from text file
 */
void Shader::loadShaderFromSource(GLenum type, const std::string &filename, const std::string &version/* = GLSLVER*/) {

    std::stringstream ss;
    shaderfile_name = filename;

    ss << version << std::endl;
    for(const auto &ext : extensions)
    {
        if(ext == std::string("GL_ARB_tessellation_shader"))
            if(type != GL_TESS_CONTROL_SHADER && type != GL_TESS_EVALUATION_SHADER)
                continue;
        ss << "#extension " << ext << " : enable" << std::endl;
    }

    ss << "#define " << shader_defines[type] << std::endl;

    ss << preprocessIncludes(filename);

    std::string str = ss.str();
    //LOG(verbose) << str;

    int length = str.length();
    const char *data = str.c_str();

    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const char **)&data, &length);
    glCompileShader(id);

    LOG(verbose) << filename << " file " << shader_defines[type];
    bool has_error = !printLog(id);
    if(has_error)
    {
        LOG(error) << "in file " << filename;
        std::string f_name = string_format("shader_error_log_%s_%s.txt",
                                           get_filename_headername(get_name(filename)).c_str(),
                                           shader_defines[type].c_str());
        LOG(error) << "shader error detail saveid in " << f_name;
        std::stringstream out_file;
        out_file << str;
        char infoLog[1024];
        int infologLength = 0;
        glGetShaderInfoLog(id, 1024, &infologLength, infoLog);
        out_file << std::endl << "=======ERROR======" << std::endl << infoLog << std::endl;
        SaveTextFile(f_name, out_file.str());
    }

    glAttachShader(program, id);
    shaders_.push_back(id);
}


std::string Shader::preprocessIncludes(const std::string &filename, int level /*= 0 */ )
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

            output << "#ifndef " << get_filename_headername(include_file) << std::endl;
            output << "#define " << get_filename_headername(include_file) << std::endl;
            output << preprocessIncludes(get_dir(filename) + include_file, level + 1) << std::endl;
            output << "#endif //" << get_filename_headername(include_file) << std::endl;
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
bool Shader::Link() {
    glLinkProgram(program);
    LOG(verbose) << "Program " << std::to_string(program) << " linking";
    if(!printLog(program))
        throw;
    LOG(verbose) << "--------------------";
    return true;
}

GLuint Shader::GetUniformLocation(const std::string &uni_name)
{
    const auto &search = m_uniforms.find(uni_name);
    if(search == m_uniforms.end())
    {
        GLuint loc = glGetUniformLocation(program, uni_name.c_str());
        if(loc == -1)
        {
            LOG(error) << uni_name << " missed in " << shaderfile_name;
        }
        m_uniforms[uni_name] = loc;
        return loc;
    }
    else
    {
        return m_uniforms[uni_name];
    }
}

void Shader::AddExtension(std::string s)
{
    extensions.push_back(std::move(s));
}

void Shader::SetUniform_(const glm::mat4 &val, const std::string &uni_name)
{
    glUniformMatrix4fv(GetUniformLocation(uni_name), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniform_(const glm::mat3 &val, const std::string &uni_name)
{
    glUniformMatrix3fv(GetUniformLocation(uni_name), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniform_(const glm::mat2 &val, const std::string &uni_name)
{
    glUniformMatrix2fv(GetUniformLocation(uni_name), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniform_(int val, const std::string &uni_name)
{
    glUniform1i(GetUniformLocation(uni_name), val);
}

void Shader::SetUniform_(unsigned int val, const std::string &uni_name)
{
    glUniform1ui(GetUniformLocation(uni_name), val);
}

void Shader::SetUniform_(const glm::vec4 &val, const std::string &uni_name)
{
    glUniform4fv(GetUniformLocation(uni_name), 1, &val[0]);
}

void Shader::SetUniform_(const glm::vec3 &val, const std::string &uni_name)
{
    glUniform3fv(GetUniformLocation(uni_name), 1, &val[0]);
}

void Shader::SetUniform_(const glm::vec2 &val, const std::string &uni_name)
{
    glUniform2fv(GetUniformLocation(uni_name), 1, &val[0]);
}

void Shader::SetUniform_(const float &val, const std::string &uni_name)
{
    glUniform1f(GetUniformLocation(uni_name), val);
}
