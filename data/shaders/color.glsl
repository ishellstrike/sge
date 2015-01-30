#ifdef _FRAGMENT_

varying vec4 fragColor;
varying vec2 uv;

void main(void)
{
    gl_FragColor = fragColor;
}

#endif

#ifdef _VERTEX_

attribute vec3 position;
attribute vec2 texcoord;
attribute vec3 normal;
attribute vec4 color;

uniform mat4 MVP;

varying vec4 fragColor;
varying vec2 uv;


void main(void)
{
    gl_Position = MVP * vec4(position, 1);
    fragColor = color;
    uv = texcoord;
}

#endif

