/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifdef _FRAGMENT_

uniform sampler2D material_texture;
in vec2 fragTexcoord;
in vec4 col;

uniform float iGlobalTime;

layout (location = 0) out vec4 WorldPosOut;
layout (location = 1) out vec4 DiffuseOut;
layout (location = 2) out vec4 NormalOut;
layout (location = 3) out vec4 TexCoordOut;

void main(void)
{
    vec2 p = -1.0 + 2.0 * fragTexcoord;
    float r = sqrt(dot(p, p));
    if (r < 1.0)
    {
      vec2 uv;
      float f = (1.0 - sqrt(1.0 - r))/r;
      //vec3 n = (Rd * t) / r;
      uv.x = p.x * f + iGlobalTime;
      uv.y = p.y * f + iGlobalTime;
      DiffuseOut = texture(material_texture, uv);
      TexCoordOut = vec4(uv,0,1);
      NormalOut = mix(vec4(0,1,0,1), vec4(1,0,0,1), r*r);
    }
    else
    {
      discard;
    }

    //WorldPosOut = vec4(1);
    //DiffuseOut = vec4(1);
   // NormalOut = vec4(1);
   // TexCoordOut = vec4(1);
}

#endif

#ifdef _VERTEX_


layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec4 color;

uniform mat4 transform_VP;

out vec2 fragTexcoord;
out vec4 col;

void main(void)
{
    gl_Position = transform_VP * vec4(position, 1);
    fragTexcoord = texcoord;
    col = color;
}

#endif

