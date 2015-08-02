#ifndef JargShader_h__
#define JargShader_h__
#include <string>
#include <vector>
#include <GL/glew.h>
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
    std::string name;
    std::vector<int> vars; /*!< stored uniforms locations */
    void Use() const;
    GLint locateVar(const std::string &s);
    void loadShaderFromSource(GLenum type, const std::string &filename, const std::string &version = GLSLVER);
    bool Link();
    GLint program;
    bool has_header;
    std::vector<GLint> shaders_;
    std::string global_header;

    GLint posAttrib, /*!< position attribute */
    colAttrib,       /*!< color attribute */
    uvAttrib,        /*!< texcoord attribute */
    uv2Attrib,       /*!< texcoord2 attribute */
    normAttrib,      /*!< normal attribute */
    tangentAttrib,   /*!< tangent attribute */
    binormalAttrib;  /*!< binormal attribute */

    void Afterlink();
    std::string preprocessIncludes(const std::string &filename, int level = 0);
private:
    GLint locate(const std::string &s);
};
#endif // JargShader_h__
