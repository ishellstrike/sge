﻿/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "shader.h"
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include "logger.h"
#include "helper.h"
#include <map>
#include <regex>

bool printLog(GLuint id, bool compile = true)
{
    char infoLog[1024];
    int infologLength = 0;
    if (glIsShader(id))
        glGetShaderInfoLog(id, 1024, &infologLength, infoLog);
    else
        glGetProgramInfoLog(id, 1024, &infologLength, infoLog);

	GLint good = 0;
	if (compile)
		glGetProgramiv(id, GL_COMPILE_STATUS, &good);
	else
		glGetProgramiv(id, GL_LINK_STATUS, &good);

    if (infologLength > 0)
        LOG(error) << infoLog;
    else
        LOG(trace) << "     no errors";

	return !good;
}

Shader::Shader()
{
    program = glCreateProgram();
}

Shader::~Shader(void)
{
    Clear();
    glDeleteProgram(program);
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

std::string LoadTextFile(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::stringstream ss;
    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            ss << line << std::endl;
        }
        file.close();
    } else {
        LOG(fatal) << sge::string_format("Failed to open file %s", filename.c_str());
    }

    return ss.str();
}

void SaveTextFile(const std::string &filename, const std::string &content)
{
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
    } else {
        LOG(fatal) << sge::string_format("Failed to save file %s", filename.c_str());
    }
}

int GetLastPatternedFilenameNubmer(const std::string &filename, const std::string &ext)
{
    bool cont = true;
    int i = 0;

    std::ifstream file(sge::string_format("%s%d%s", filename.c_str(), 0, ext.c_str()));
    if(!file.is_open()) {file.close(); return -1;}
    file.close();

    while(cont)
    {
        std::ifstream file(sge::string_format("%s%d%s", filename.c_str(), i, ext.c_str()));
        if(!file.is_open()) cont = false;
        else i++;
        file.close();
    }
    return i;
}

void Shader::LogDumpError(const std::string &filename, GLenum type, const std::string &str, GLuint shader)
{
    std::string f_name;
    if(shader != -1)
    {
        f_name = sge::string_format("shader_error_log_%s_%s.txt",
                                    get_filename_headername(get_name(filename)).c_str(),
                                    shader_defines[type].c_str());
        LOG(error) << "in file " << filename;
        LOG(error) << "shader error detail saveid in " << f_name;
    }
    else
    {
        f_name = sge::string_format("shader_force_dump_%s_%s.txt",
                                    get_filename_headername(get_name(filename)).c_str(),
                                    shader_defines[type].c_str());
        LOG(trace) << filename << " force dump to " << f_name;
    }

    std::stringstream out_file;
    out_file << str;
    if(shader != -1)
    {
        char infoLog[1024];
        int infologLength = 0;
        glGetShaderInfoLog(shader, 1024, &infologLength, infoLog);
        out_file << std::endl << "=======ERROR======" << std::endl << infoLog << std::endl;
    }
    SaveTextFile(f_name, out_file.str());
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
    for(const auto &def : defines)
    {
        ss << "#define " << def << std::endl;
    }

    for(const auto &ext : extensions)
    {
        if(ext == std::string("GL_ARB_tessellation_shader"))
            if(type != GL_TESS_CONTROL_SHADER && type != GL_TESS_EVALUATION_SHADER)
                continue;
        ss << "#extension " << ext << " : enable" << std::endl;
    }

    ss << "#define " << shader_defines[type] << std::endl;
    ss << "//end of runtime header" << std::endl;

    preprocessIncludes(ss, filename, type);

    std::string str = ss.str();
    //LOG(verbose) << str;

    int length = str.length();
    const char *data = str.c_str();

    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, (const char **)&data, &length);
    glCompileShader(id);

    LOG(trace) << filename << " file " << shader_defines[type];
    bool has_error = !printLog(id);
    if(has_error)
    {
        LogDumpError(filename, type, str, id);
    }

    glAttachShader(program, id);
    shaders_.push_back(id);
    source_loaded = true;
}


void Shader::preprocessIncludes(std::stringstream &ss, const std::string &filename, GLenum type , int level /*= 0 */)
{
    if(level > 32)
        LOG(fatal) << "header inclusion depth limit reached, might be caused by cyclic header inclusion";

    static const std::regex re("^[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
    static const std::regex rdump("^[ ]*//DUMP_SOURCE.*");
    std::stringstream input;

    input << LoadTextFile(filename);

    size_t line_number = 1;
    std::smatch matches;

    std::string line;
    while(std::getline(input, line))
    {
        if (std::regex_search(line, matches, re))
        {
            std::string include_file = matches[1];

            ss << "#ifndef " << get_filename_headername(include_file) << std::endl;
            ss << "#define " << get_filename_headername(include_file) << std::endl;
            preprocessIncludes(ss, get_dir(filename) + include_file, type, level + 1);
            ss << "#endif //" << get_filename_headername(include_file) << std::endl;
        }
        else
        {
            ss <<  line << std::endl;
        }

        if(std::regex_search(line, matches, rdump))
        {
            LogDumpError(filename, type, ss.str());
        }
        ++line_number;
    }
}

/*!
 * \brief JargShader::Link
 * \return success
 *
 * link compiled shaders to program
 */
bool Shader::Link() const {
    glLinkProgram(program);
    LOG(trace) << "Program " << std::to_string(program) << " linking";
    if(printLog(program, false))
        throw;
    LOG(trace) << "--------------------";
    return true;
}

void Shader::Clear()
{
    while(!shaders_.empty()) {
        glDeleteShader(shaders_.back());
        //LOG(verbose) << "Deleting shader " << std::to_string(shaders_.back());
        shaders_.pop_back();
    }
    glDeleteProgram(program);
    //LOG(verbose) << string_format("Deleting program %i", program);
    program = glCreateProgram();
    extensions.clear();
    defines.clear();
    shaderfile_name = "";
    source_loaded = false;
}

GLuint Shader::GetUniformLocation(const std::string &uni_name) const
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
    if(source_loaded)
        LOG(fatal) << "extensions must be added before source loading";
    extensions.push_back(std::move(s));
}

void Shader::AddDefine(std::string s)
{
    if(source_loaded)
        LOG(fatal) << "defines must be added before source loading";
    defines.push_back(std::move(s));
}

void Shader::SaveBinary(const std::string &)
{

}

void Shader::LoadBinary(const std::string &)
{

}

void Shader::SetUniform_(const glm::mat4 &val, const GLuint pos) const
{
    glUniformMatrix4fv(pos, 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniform_(const glm::mat3 &val, const GLuint pos) const
{
    glUniformMatrix3fv(pos, 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniform_(const glm::mat2 &val, const GLuint pos) const
{
    glUniformMatrix2fv(pos, 1, GL_FALSE, &val[0][0]);
}

void Shader::SetUniform_(int val, const GLuint pos) const
{
    glUniform1i(pos, val);
}

void Shader::SetUniform_(unsigned int val, const GLuint pos) const
{
    glUniform1ui(pos, val);
}

void Shader::SetUniform_(const glm::vec4 &val, const GLuint pos) const
{
    glUniform4fv(pos, 1, &val[0]);
}

void Shader::SetUniform_(const glm::vec3 &val, const GLuint pos) const
{
    glUniform3fv(pos, 1, &val[0]);
}

void Shader::SetUniform_(const glm::vec2 &val, const GLuint pos) const
{
    glUniform2fv(pos, 1, &val[0]);
}

void Shader::SetUniform_(const float &val, const GLuint pos) const
{
    glUniform1f(pos, val);
}
