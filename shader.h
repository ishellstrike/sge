#ifndef JargShader_h__
#define JargShader_h__

#include <string>
#include <vector>
#include <GL/glew.h>
#define GLSLVER "#version 120"

class JargShader{
public:
    JargShader();
    ~JargShader(void);
    std::string name;
    std::vector<int> vars;
    void Use() const;
    GLint locateVar(const std::string &s);
    void PushGlobalHeader(const std::string &s, const char *newParameter = GLSLVER);
    void loadShaderFromSource(GLenum type, const std::string &source);
    bool Link();
    GLint program;
    bool has_header;
    std::vector<GLint> shaders_;
    std::string global_header;

    GLint posAttrib, colAttrib, uvAttrib, normAttrib, tangentAttrib, binormalAttrib;

    void Afterlink();
private:
    GLint locate(const std::string &s);
};
#endif // JargShader_h__
