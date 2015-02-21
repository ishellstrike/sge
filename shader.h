#ifndef JargShader_h__
#define JargShader_h__

#include <string>
#include <vector>
#include <GL/glew.h>
#define GLSLVER "#version 120"

/*!
 * \brief The JargShader class
 *
 * Abstract shader implementation
 * can load adn compile programs from source
 * automatically release owning ogl resources
 */
class JargShader{
public:
    JargShader();
    ~JargShader(void);
    std::string name;
    std::vector<int> vars; /*!< stored uniforms locations */
    void Use() const;
    GLint locateVar(const std::string &s);
    void PushGlobalHeader(const std::string &s, const char *version = GLSLVER);
    void loadShaderFromSource(GLenum type, const std::string &source);
    bool Link();
    GLint program;
    bool has_header;
    std::vector<GLint> shaders_;
    std::string global_header;

    GLint posAttrib, /*!< position attribute */
    colAttrib,       /*!< color attribute */
    uvAttrib,        /*!< texcoord attribute */
    normAttrib,      /*!< normal attribute */
    tangentAttrib,   /*!< tangent attribute */
    binormalAttrib;  /*!< binormal attribute */

    void Afterlink();
private:
    GLint locate(const std::string &s);
};
#endif // JargShader_h__
