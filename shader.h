/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef JargShader_h__
#define JargShader_h__
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>
#define GLSLVER "#version 330 core"

/*!
 * \brief The JargShader class
 *
 * Abstract shader implementation
 * can load adn compile programs from source
 * automatically release owning ogl resources
 */
class Shader{
public:
    Shader();
    virtual ~Shader(void);
    std::string shaderfile_name;
    void Use() const;
    void loadShaderFromSource(GLenum type, const std::string &filename, const std::string &version = GLSLVER);
    bool Link() const;
    void Clear();
    GLint program;
    std::vector<GLint> shaders_;
    std::vector<std::string> extensions;
    std::vector<std::string> defines;

    template<class T>
    void SetUniform(const std::string &name, const T &val) const
    {
      SetUniform_(val, name);
    }

    GLuint GetUniformLocation(const std::string &uni_name) const;

    void AddExtension(std::string s);
    void AddDefine(std::string s);

    void SaveBinary(const std::string &name);
    void LoadBinary(const std::string &name);
private:
    void LogDumpError(const std::string &filename, GLenum type, const std::string &str, GLuint shader = -1);
    GLint locate(const std::string &s);
    void preprocessIncludes(std::stringstream &ss, const std::string &filename, GLenum type, int level = 0);

    void SetUniform_(const glm::mat4 &val, const std::string &uni_name) const;
    void SetUniform_(const glm::mat3 &val, const std::string &uni_name) const;
    void SetUniform_(const glm::mat2 &val, const std::string &uni_name) const;

    void SetUniform_(int val,              const std::string &uni_name) const;
    void SetUniform_(unsigned int val,     const std::string &uni_name) const;

    void SetUniform_(const glm::vec4 &val, const std::string &uni_name) const;
    void SetUniform_(const glm::vec3 &val, const std::string &uni_name) const;
    void SetUniform_(const glm::vec2 &val, const std::string &uni_name) const;

    void SetUniform_(const float &val,     const std::string &uni_name) const;

    mutable std::unordered_map <std::string, GLuint> m_uniforms;
    bool source_loaded = false;
};
#endif // JargShader_h__
