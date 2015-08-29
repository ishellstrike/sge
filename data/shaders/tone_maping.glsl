/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
uniform sampler2D mainMap;
uniform sampler2D blurMap;

uniform float exposure = 2;

#ifdef _VERTEX_
layout (location = 0) in vec3 position;

out vec2 fragCoord;

void main(void)
{
    gl_Position = vec4(position, 1);
    fragCoord = position.xy * 0.5 + 0.5;
}
#endif

#ifdef _FRAGMENT_

layout (location = 0) out vec4 fragColor;

in vec2 fragCoord;

void main( void )
{
    vec4 base = texture(mainMap, fragCoord);
    vec4 blur = texture(blurMap, fragCoord)/3;
    vec4 color = base + 5.0 * blur;

    fragColor.rgb = vec3 ( 1.0 ) - exp ( textureLod(mainMap, vec2(0.5, 0.5), 99).xyz * color.rgb );
    fragColor.a   = 1.0;
}
#endif
