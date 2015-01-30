#ifdef _FRAGMENT_

uniform sampler2D colorTexture;
varying vec2 fragTexcoord;
varying vec4 col;

void main(void)
{
    gl_FragColor = col * vec4(texture2D(colorTexture, fragTexcoord).rgba);
}

#endif

#ifdef _VERTEX_


attribute vec3 position;
attribute vec2 texcoord;
attribute vec3 normal;
attribute vec4 color;

uniform mat4 MVP;

varying vec2 fragTexcoord;
varying vec4 col;

void main(void)
{
    gl_Position = MVP * vec4(position, 1);
    fragTexcoord = texcoord;
    col = color;
}

#endif

