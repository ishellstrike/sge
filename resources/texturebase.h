#ifndef TEXTUREBASE_H
#define TEXTUREBASE_H
#include "GL/glew.h"
#include <string>

class TextureBase
{
public:
   TextureBase();

   GLuint textureId = 0;
   std::string name = "empty";
   unsigned int height = 0;
   unsigned int width = 0;
   unsigned int zsize = 0;
   bool mip = false;
};

#endif // TEXTUREBASE_H
