/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef COLORSCHEME_H
#define COLORSCHEME_H
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include "colorextender.h"

struct ColorScheme
{
    glm::vec4 border_up, border_down, hovered, basic, pressed, text, hovered_text;

    static ColorScheme Basic();

    static ColorScheme FromColor(const glm::vec4 &col);
};

#endif // COLORSCHEME_H
