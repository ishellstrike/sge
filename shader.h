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
    ~Shader(void);
    std::string shaderfile_name;
    void Use() const;
    void loadShaderFromSource(GLenum type, const std::string &filename, const std::string &version = GLSLVER);
    bool Link();
    GLint program;
    std::vector<GLint> shaders_;
    std::vector<std::string> extensions;

    template<class T>
    void SetUniform(const std::string &name, const T &val)
    {
      SetUniform_(val, name);
    }

    GLuint GetUniformLocation(const std::string &uni_name);

    void AddExtension(std::string s);
private:
    GLint locate(const std::string &s);
    std::string preprocessIncludes(const std::string &filename, int level = 0);

    void SetUniform_(const glm::mat4 &val, const std::string &uni_name);
    void SetUniform_(const glm::mat3 &val, const std::string &uni_name);
    void SetUniform_(const glm::mat2 &val, const std::string &uni_name);

    void SetUniform_(int val,              const std::string &uni_name);
    void SetUniform_(unsigned int val,     const std::string &uni_name);

    void SetUniform_(const glm::vec4 &val, const std::string &uni_name);
    void SetUniform_(const glm::vec3 &val, const std::string &uni_name);
    void SetUniform_(const glm::vec2 &val, const std::string &uni_name);

    void SetUniform_(const float &val,     const std::string &uni_name);

    std::unordered_map <std::string, GLuint> m_uniforms;
};
#endif // JargShader_h__
