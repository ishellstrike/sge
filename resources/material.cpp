#include "material.h"


std::vector<GLuint> Material::GetTextureLocations()
{
    return {texture ? texture->textureId : -1,
            normal ? normal->textureId : -1,
            height ? height->textureId : -1,
            grad ? grad->textureId : -1,
            global_height ? global_height->textureId : -1,

            detail ? detail->textureId : -1,

            low ? low->textureId : -1,
            medium ? medium->textureId : -1,
            high ? high->textureId : -1,
            side ? side->textureId : -1};
}
