#ifndef JargShader_h__
#define JargShader_h__

#include <string>
#include <vector>
#include <GL/glew.h>

class JargShader{
public:
    JargShader();
    ~JargShader(void);
    std::string name;
    std::vector<int> vars;
    void Use() const;
<<<<<<< HEAD
    GLint locateVars(const std::string &s);
    void PushGlobalHeader(const std::string &s, const char *newParameter = "#version 120");
=======
    GLint locateVar(const std::string &s);
    void PushGlobalHeader(const std::string &s, const char *newParameter = "#version 210");
>>>>>>> 352992101835dc6a114c459c82272cd3ebc100b6
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
