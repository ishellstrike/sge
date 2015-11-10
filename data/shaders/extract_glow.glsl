/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/
uniform sampler2D mainMap;

uniform float lWhite = 1;

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
    vec2 dx   = vec2 ( 1.0 / 800.0, 0.0 );
    vec2 dy   = vec2 ( 0.0, 1.0 / 800.0 );
    vec4 base = texture(mainMap, fragCoord.xy ) +
                texture(mainMap, fragCoord.xy + dx ) +
                texture(mainMap, fragCoord.xy + dy ) +
                texture(mainMap, fragCoord.xy + dx + dy );

    base *= 0.25;

    if (length (base.rgb) < 1 )
        base.rgb = vec3(0);

    fragColor = base;
}
#endif
