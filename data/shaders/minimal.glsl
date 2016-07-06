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
#include "scattering/scatter_params.h"
#include "scattering/common.glsl"

uniform sampler2D material_texture;
uniform sampler2D material_normal;
uniform sampler2D material_height;
uniform sampler2D material_global_height;
uniform sampler2D material_grad;

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
uniform float S = 100;
uniform float screenDistance = 20;

uniform vec2 WindowSize;

#define FIX

const float ISun = 100.0;

uniform vec3 c;
uniform vec3 s = vec3(0,1,0);
uniform mat4 projInverse;
uniform mat4 viewInverse;
uniform float exposure = 0.4f;

#ifdef _VERTEX_
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texcoord;
layout (location = 4) in vec2 texcoord2;


out VS_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
} vert;

void main(void)
{
    gl_Position = vec4(position, 1);
    vert.texcoordout = texcoord;
    vert.texcoordout2 = texcoord2;
}
#endif

#ifdef _TESSCONTROL_
layout ( vertices = 3 ) out;

in VS_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
} tcs_in[];

out TCS_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
} tcs_out[];

void main ()
{

    if (gl_InvocationID == 0)
    {
        vec2 screenPos[3];

        for (int i = 0; i < 3; i++)
        {
            float snoize = decodeFloat(texture(material_height, tcs_in[i].texcoordout));
            vec3 pos = gl_in[i].gl_Position.xyz;
            vec3 newPosition = (R + S * snoize) * pos;
            vec4 position = transform_VP * transform_M * vec4(newPosition, 1);

            if (position.w != 0.0)
            {
                position /= position.w;
            }

            screenPos[i].x = (position.x + 1.0) * 0.5 * WindowSize.x;
            screenPos[i].y = (position.y + 1.0) * 0.5 * WindowSize.y;
        }

        // Tessellate depending how big the distance is on the screen
        gl_TessLevelOuter[0] = max(distance(screenPos[1], screenPos[2]) / screenDistance, 1.0);
        gl_TessLevelOuter[1] = max(distance(screenPos[2], screenPos[0]) / screenDistance, 1.0);
        gl_TessLevelOuter[2] = max(distance(screenPos[0], screenPos[1]) / screenDistance, 1.0);

        gl_TessLevelInner[0] = (gl_TessLevelOuter[0] + gl_TessLevelOuter[1] + gl_TessLevelOuter[2]) / 3.0;
    }

    tcs_out[gl_InvocationID].texcoordout = tcs_in[gl_InvocationID].texcoordout;
    tcs_out[gl_InvocationID].texcoordout2 = tcs_in[gl_InvocationID].texcoordout2;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
#endif

#ifdef _TESSEVAL_
layout(triangles, fractional_odd_spacing, ccw) in;

in TCS_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
} tes_in[];

out TES_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
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
} geo_in[3];

out GEO_OUT {
    vec2 texcoordout;
    vec2 texcoordout2;
    vec3 positionout;
	vec2 coords;
    vec3 ray;
} geo_out;

void main(void)
{
    const vec3 arr[] = vec3[](vec3(1,0,0), vec3(0,1,0), vec3(0,0,1));
    for(int i = 0; i < 3; ++i)
    {
        geo_out.texcoordout = geo_in[i].texcoordout;
        geo_out.texcoordout2 = geo_in[i].texcoordout2;

        float snoize = decodeFloat(texture(material_global_height, geo_out.texcoordout2));
        vec3 pos = gl_in[i].gl_Position.xyz;
        vec3 newPosition = (R + S * snoize) * pos;
        vec4 mLocation = transform_M * vec4(newPosition, 1);
        vec4 mvpLocation = transform_VP * mLocation;

        geo_out.positionout = mLocation.xyz;
        gl_Position = mvpLocation;
		
		geo_out.coords = mvpLocation.xy * 0.5 + 0.5;
		geo_out.ray = newPosition;//(viewInverse * vec4((projInverse * vec4(normalize(newPosition), 1)).xyz, 0.0)).xyz;

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
	vec2 coords;
    vec3 ray;
} frag_in;

const vec4 fog = vec4(100/255.f, 149/255.f, 237/255.f, 1.f);
float density = 0.0003;
const float LOG2 = 1.442695;
const vec4 colc = vec4(0.5, 0.5, 0.5, 1);

layout (location = 0) out vec4 WorldPosOut;
layout (location = 1) out vec4 DiffuseOut;
layout (location = 2) out vec4 NormalOut;
layout (location = 3) out vec4 TexCoordOut;

//inscattered light along ray x+tv, when sun in direction s (=S[L]-T(x,x0)S[L]|x0)
vec3 inscatter(inout vec3 x, inout float t, vec3 v, vec3 s, out float r, out float mu, out vec3 attenuation) {
    vec3 result;
    r = length(x);
    mu = dot(x, v) / r;
    float d = -r * mu - sqrt(r * r * (mu * mu - 1.0) + Rt * Rt);
    if (d > 0.0) { // if x in space and ray intersects atmosphere
        // move x to nearest intersection of ray with top atmosphere boundary
        x += d * v;
        t -= d;
        mu = (r * mu + d) / Rt;
        r = Rt;
    }
    if (r <= Rt) { // if ray intersects atmosphere
        float nu = dot(v, s);
        float muS = dot(x, s) / r;
        float phaseR = phaseFunctionR(nu);
        float phaseM = phaseFunctionM(nu);
        vec4 inscatter = max(texture4D(inscatterSampler, r, mu, muS, nu), 0.0);
        if (t > 0.0) {
            vec3 x0 = x + t * v;
            float r0 = length(x0);
            float rMu0 = dot(x0, v);
            float mu0 = rMu0 / r0;
            float muS0 = dot(x0, s) / r0;
#ifdef FIX
            // avoids imprecision problems in transmittance computations based on textures
            attenuation = analyticTransmittance(r, mu, t);
#else
            attenuation = transmittance(r, mu, v, x0);
#endif
            if (r0 > Rg + 0.01) {
                // computes S[L]-T(x,x0)S[L]|x0
                inscatter = max(inscatter - attenuation.rgbr * texture4D(inscatterSampler, r0, mu0, muS0, nu), 0.0);
#ifdef FIX
                // avoids imprecision problems near horizon by interpolating between two points above and below horizon
                const float EPS = 0.004;
                float muHoriz = -sqrt(1.0 - (Rg / r) * (Rg / r));
                if (abs(mu - muHoriz) < EPS) {
                    float a = ((mu - muHoriz) + EPS) / (2.0 * EPS);

                    mu = muHoriz - EPS;
                    r0 = sqrt(r * r + t * t + 2.0 * r * t * mu);
                    mu0 = (r * mu + t) / r0;
                    vec4 inScatter0 = texture4D(inscatterSampler, r, mu, muS, nu);
                    vec4 inScatter1 = texture4D(inscatterSampler, r0, mu0, muS0, nu);
                    vec4 inScatterA = max(inScatter0 - attenuation.rgbr * inScatter1, 0.0);

                    mu = muHoriz + EPS;
                    r0 = sqrt(r * r + t * t + 2.0 * r * t * mu);
                    mu0 = (r * mu + t) / r0;
                    inScatter0 = texture4D(inscatterSampler, r, mu, muS, nu);
                    inScatter1 = texture4D(inscatterSampler, r0, mu0, muS0, nu);
                    vec4 inScatterB = max(inScatter0 - attenuation.rgbr * inScatter1, 0.0);

                    inscatter = mix(inScatterA, inScatterB, a);
                }
#endif
            }
        }
#ifdef FIX
        // avoids imprecision problems in Mie scattering when sun is below horizon
        inscatter.w *= smoothstep(0.00, 0.02, muS);
#endif
        result = max(inscatter.rgb * phaseR + getMie(inscatter) * phaseM, 0.0);
    } else { // x in space and ray looking in space
        result = vec3(0.0);
    }
    return result * ISun;
}

//ground radiance at end of ray x+tv, when sun in direction s
//attenuated bewteen ground and viewer (=R[L0]+R[L*])
vec3 groundColor(vec3 x, float t, vec3 v, vec3 s, float r, float mu, vec3 attenuation)
{
    vec3 result = vec3(0);
    if (t > 0.0) { // if ray hits ground surface
        // ground reflectance at end of ray, x0
        vec3 x0 = x + t * v;
        float r0 = length(x0);
        vec3 n = x0 / r0;
        vec2 coords = vec2(atan(n.y, n.x), acos(n.z)) * vec2(0.5, 1.0) / M_PI + vec2(0.5, 0.0);
        vec4 reflectance = texture(reflectanceSampler, coords) * vec4(0.2, 0.2, 0.2, 1.0);
        if (r0 > Rg + 0.01) {
            reflectance = vec4(0.4, 0.4, 0.4, 0.0);
        }

        // direct sun light (radiance) reaching x0
        float muS = dot(n, s);
        vec3 sunLight = transmittanceWithShadow(r0, muS);

        // precomputed sky light (irradiance) (=E[L*]) at x0
        vec3 groundSkyLight = irradiance(irradianceSampler, r0, muS);

        // light reflected at x0 (=(R[L0]+R[L*])/T(x,x0))
        vec3 groundColor = reflectance.rgb * (max(muS, 0.0) * sunLight + groundSkyLight) * ISun / M_PI;

        // water specular color due to sunLight
        if (reflectance.w > 0.0) {
            vec3 h = normalize(s - v);
            float fresnel = 0.02 + 0.98 * pow(1.0 - dot(-v, h), 5.0);
            float waterBrdf = fresnel * pow(max(dot(h, n), 0.0), 150.0);
            groundColor += reflectance.w * max(waterBrdf, 0.0) * sunLight * ISun;
        }

        result = attenuation * groundColor; //=R[L0]+R[L*]
    } else { // ray looking at the sky
        result = vec3(0.0);
    }
    return result;
}

// direct sun light for ray x+tv, when sun in direction s (=L0)
vec3 sunColor(vec3 x, float t, vec3 v, vec3 s, float r, float mu) {
    if (t > 0.0) {
        return vec3(0.0);
    } else {
        vec3 transmittance = r <= Rt ? transmittanceWithShadow(r, mu) : vec3(1.0); // T(x,xo)
        float isun = step(cos(M_PI / 180.0), dot(v, s)) * ISun; // Lsun
        return transmittance * isun; // Eq (9)
    }
}

vec3 HDR(vec3 L) {
    L = L * exposure;
    L.r = L.r < 1.413 ? pow(L.r * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.r);
    L.g = L.g < 1.413 ? pow(L.g * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.g);
    L.b = L.b < 1.413 ? pow(L.b * 0.38317, 1.0 / 2.2) : 1.0 - exp(-L.b);
    return L;
}

in vec2 coords;
in vec3 ray;

void main(void)
{
vec2 texcoordout = frag_in.texcoordout;
    vec2 texcoordout2 = frag_in.texcoordout2;
    vec3 positionout = frag_in.positionout;

    float snoize = decodeFloat(texture(material_height, texcoordout));
    vec3 grad = -decodeNormal(texture(material_grad, texcoordout));

    grad = grad / (R + S * snoize);
    vec3 plane = grad*10 - (grad*10 * positionout) * positionout;
	
	float snoize2 = decodeFloat(texture(material_height, texcoordout*100));
    vec3 grad2 = -decodeNormal(texture(material_grad, texcoordout*100));

    grad2 = grad2 / (R + S * snoize2);
    vec3 plane2 = grad2*9 - (grad2*9 * positionout) * positionout;
	
	
	
    vec3 normal = positionout - S * plane - (S * plane2);
    vec3 eye = normalize(normal);
    vec3 light = normalize(transform_lightPos);

    vec4 tex;
    vec4 tex2;

    tex = texture2D(material_low, texcoordout2*1000);
    tex2 = texture2D(material_medium, texcoordout2*1000);
    tex = mix(tex, tex2, snoize+0.4);
    if(snoize > 0.5)
    {
        tex = texture2D(material_high, texcoordout2*1000);
    }

    vec4 tex3 = texture2D(material_side, texcoordout2*1000);
    tex3 += texture2D(material_side, texcoordout2*100);
    tex3 /= 1.5;
    tex = mix(tex, tex3, clamp(abs(grad.y) + abs(grad.x), 0, 1));
	
	vec3 x = c;
    vec3 v = normalize(frag_in.ray);

    float r = length(x);
    float mu = dot(x, v) / r;
    float t = -r * mu - sqrt(r * r * (mu * mu - 1.0) + Rg * Rg);

    vec3 g = x - vec3(0.0, 0.0, Rg + 10.0);
    float a = v.x * v.x + v.y * v.y - v.z * v.z;
    float b = 2.0 * (g.x * v.x + g.y * v.y - g.z * v.z);
    float c = g.x * g.x + g.y * g.y - g.z * g.z;
    float d = -(b + sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
    bool cone = d > 0.0 && abs(x.z + d * v.z - Rg) <= 10.0;

    if (t > 0.0) {
        if (cone && d < t) {
            t = d;
        }
    } else if (cone) {
        t = d;
    }

    vec3 attenuation;
    vec3 inscatterColor = inscatter(x, t, v, s, r, mu, attenuation); //S[L]-T(x,xs)S[l]|xs
    vec3 groundColor = groundColor(x, t, v, s, r, mu, attenuation); //R[L0]+R[L*]
    vec3 sunColor = sunColor(x, t, v, s, r, mu); //L0
    vec4 outc = vec4(HDR(sunColor + inscatterColor), 1.0); // Eq (16)
    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
    float lum = dot(outc.xyz, W);
    outc = vec4(outc.xyz, lum);
    //gl_FragColor = outc;


    //DiffuseOut = texture(inscatterSampler,vec3(coords,(s.x+1.0)/2.0))*100;
	//DiffuseOut.a = 1;
    //gl_FragColor = vec4(texture(irradianceSampler,coords).rgb*5.0, 1.0);
    //gl_FragColor = texture(transmittanceSampler, coords);
	
	DiffuseOut = tex;
    TexCoordOut = vec4(texcoordout, 0, 1);
    NormalOut = vec4(eye, 1);
    WorldPosOut = vec4(positionout, 1);
}
#endif