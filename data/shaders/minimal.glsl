/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT License.
        See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/
#define VERT_POSITION 0
#define VERT_TEXCOORD 1
#define VERT_NORMAL 2
#define VERT_COLOR 3
#define FRAG_OUTPUT0 0

#include "float.lib.glsl"

uniform sampler2D material_texture;
uniform sampler2D material_normal;
uniform sampler2D material_height;
uniform sampler2D material_grad;
uniform sampler2D material_global_height;

uniform sampler2D material_detail;
uniform sampler2D material_low;
uniform sampler2D material_medium;
uniform sampler2D material_high;
uniform sampler2D material_side;

uniform vec4  material_ambient;
uniform vec4  material_diffuse;
uniform vec4  material_specular;
uniform vec4  material_emission;
uniform float material_shininess;

uniform mat4 transform_M; // model matrix
uniform mat4 transform_VP; // view * projection matrix
uniform mat3 transform_N; // normal matrix
uniform vec3 transform_viewPos;
uniform vec3 transform_lightPos;

uniform float R = 1000;
uniform float s = 100;
uniform float screenDistance = 100;

#ifdef _VERTEX_
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texcoord;
layout (location = 4) in vec2 texcoord2;


out Vertex {
    vec2 texcoordout;
    vec2 texcoordout2;
    vec3 positionout;
} Vert;

void main(void)
{
    //float snoize = decodeFloat(textureLod(material_global_height, texcoord2, 0));

    //vec3 newPosition = (R + s * snoize) * position;
    //vec4 mvpLocation = vec4(newPosition, 1);

    gl_Position = vec4(position, 1);
    Vert.positionout = position;
    Vert.texcoordout = texcoord;
    Vert.texcoordout2 = texcoord2;
}
#endif

#ifdef _TESSCONTROL_
layout ( vertices = 3 ) out;

in VS_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
    vec3 positionout;
} tcs_in[];

out TCS_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
    vec3 positionout;
} tcs_out[];

void main ()
{

    if (gl_InvocationID == 0)
    {
        vec2 screenPos[3];

        for (int i = 0; i < 3; i++)
        {
            vec4 position = transform_VP * transform_M * gl_in[i].gl_Position;

            if (position.w != 0.0)
            {
                position /= position.w;
            }

            screenPos[i].x = (position.x + 1.0) * 0.5 * 800;
            screenPos[i].y = (position.y + 1.0) * 0.5 * 800;
        }

        // Tessellate depending how big the distance is on the screen
        gl_TessLevelOuter[0] = max(distance(screenPos[1], screenPos[2]) / screenDistance, 1.0);
        gl_TessLevelOuter[1] = max(distance(screenPos[2], screenPos[0]) / screenDistance, 1.0);
        gl_TessLevelOuter[2] = max(distance(screenPos[0], screenPos[1]) / screenDistance, 1.0);

        gl_TessLevelInner[0] = (gl_TessLevelOuter[0] + gl_TessLevelOuter[1] + gl_TessLevelOuter[2]) / 3.0;
    }

    tcs_out[gl_InvocationID].texcoordout = tcs_in[gl_InvocationID].texcoordout;
    tcs_out[gl_InvocationID].texcoordout2 = tcs_in[gl_InvocationID].texcoordout2;
    tcs_out[gl_InvocationID].positionout = tcs_in[gl_InvocationID].positionout;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
#endif

#ifdef _TESSEVAL_
layout(triangles, fractional_odd_spacing, ccw) in;

in TCS_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
    vec3 positionout;
} tes_in[];

out TES_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
    vec3 positionout;
} tes_out;

vec2 interpolateVec2(in vec2 v0, in vec2 v1, in vec2 v2)
{
    return v0 * gl_TessCoord.x + v1 * gl_TessCoord.y + v2 * gl_TessCoord.z;
}

vec3 interpolateVec3(in vec3 v0, in vec3 v1, in vec3 v2)
{
    return v0 * gl_TessCoord.x + v1 * gl_TessCoord.y + v2 * gl_TessCoord.z;
}

vec4 interpolateVec4(in vec4 v0, in vec4 v1, in vec4 v2)
{
    return v0 * gl_TessCoord.x + v1 * gl_TessCoord.y + v2 * gl_TessCoord.z;
}

void main(void)
{
    tes_out.texcoordout = interpolateVec2(tes_in[0].texcoordout,
                                          tes_in[1].texcoordout,
                                          tes_in[2].texcoordout);

    tes_out.texcoordout2 = interpolateVec2(tes_in[0].texcoordout2,
                                           tes_in[1].texcoordout2,
                                           tes_in[2].texcoordout2);


    tes_out.positionout = normalize(interpolateVec3(tes_in[0].positionout,
                                                    tes_in[1].positionout,
                                                    tes_in[2].positionout));

    gl_Position = interpolateVec4(gl_in[0].gl_Position,
                                  gl_in[1].gl_Position,
                                  gl_in[2].gl_Position);
}
#endif

#ifdef _GEOMETRY_
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in TES_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
    vec3 positionout;
} tes_out[3];

out GEO_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
    vec3 positionout;
} geo_out;

void main(void)
{
    for(int i = 0; i < gl_in.length(); ++i)
    {
        geo_out.texcoordout = tes_out[i].texcoordout;
        geo_out.texcoordout2 = tes_out[i].texcoordout2;
        geo_out.positionout = tes_out[i].positionout;

        //float displacementOffset = texture(u_normalTexture, v_texCoord).a * u_displacementScale;
        //vec4 displacement = vec4(normalize(v_g_normal[i]) * displacementOffset, 0.0);

        float snoize = decodeFloat(textureLod(material_global_height, tes_out[i].texcoordout2, 0));

        vec3 newPosition = (R + s * snoize) * normalize(gl_in[i].gl_Position.xyz);
        vec4 mvpLocation =  vec4(newPosition, 0);

        gl_Position = mvpLocation;

        EmitVertex();
    }

    EndPrimitive();
}
#endif

#ifdef _FRAGMENT_
const float cutoff = 0.9f;

in GEO_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
    vec3 positionout;
} geo_in;

const vec4 fog = vec4(100/255.f, 149/255.f, 237/255.f, 1.f);
float density = 0.0003;
const float LOG2 = 1.442695;
const vec4 colc = vec4(0.5, 0.5, 0.5, 1);

layout(location = 0) out vec4 out_color;

void main(void)
{
    vec2 texcoordout = geo_in.texcoordout;
    vec2 texcoordout2 = geo_in.texcoordout2;
    vec3 positionout = geo_in.positionout;

    float snoize = decodeFloat(texture2D(material_height, texcoordout));
    vec3 grad = decodeNormal(texture2D(material_grad, texcoordout));

    grad = grad / (R + s * snoize);
    //grad = transform_N * grad;
    vec3 plane = grad - (grad * positionout) * positionout;
    vec3 normal = positionout - s * plane;
    vec3 eye = normalize(transform_N * normal);
    vec3 light = normalize(transform_lightPos);

    vec4 tex;
    vec4 tex2;

    tex = texture2D(material_low, texcoordout2*10);
    tex += texture2D(material_low, texcoordout2*100);
    tex /= 2;
    tex2 = texture2D(material_medium, texcoordout2*10);
    tex2 += texture2D(material_medium, texcoordout2*100);
    tex2 /= 2;
    tex = mix(tex, tex2, snoize+0.4);
    if(snoize > 0.5)
    {
        tex = texture2D(material_high, texcoordout2*10);
        tex += texture2D(material_high, texcoordout2*100);
    }

    vec4 tex3 = texture2D(material_side, texcoordout2*10);
    tex3 += texture2D(material_side, texcoordout2*100);
    tex = mix(tex, tex3, clamp(abs(grad.y) + abs(grad.x) * 5, 0, 1));

    vec4 col = material_ambient;

    float diffuse_rate = clamp(dot(light, eye), 0, 1);

    col += material_diffuse * diffuse_rate;

    //float z = gl_FragCoord.z / gl_FragCoord.w;
    //float fogFactor = exp2( -density * density * z * z * LOG2 );
    //fogFactor = clamp(fogFactor, 0.0, 1.0);
    //col = mix(fog, col, fogFactor);
    out_color = col * tex + material_emission;
    out_color.a = 1;
}
#endif
