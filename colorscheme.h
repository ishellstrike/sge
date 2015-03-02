#ifndef COLORSCHEME_H
#define COLORSCHEME_H
#include <glm/glm.hpp>
#include "colorextender.h"

struct ColorScheme
{
    glm::vec4 border_up, border_down, hovered, basic, pressed, text, hovered_text;

    static ColorScheme Basic()
    {
        return ColorScheme::FromColor(Color::Gray);
    }

    static ColorScheme FromColor(const glm::vec4 &col)
    {
        ColorScheme scheme;

        scheme.basic = col * 1.f;
        scheme.hovered = col * 1.2f;
        scheme.pressed = col * 0.8f;
        scheme.border_up = col * 1.5f;
        scheme.border_down = col * 0.5f;

        if(glm::length(col) > 1) //bright color
        {
            scheme.text = Color::Black;
            scheme.hovered_text = Color::White;
        }
        else
        {
            scheme.text = Color::White;
            scheme.hovered_text = Color::Yellow;
        }

        return scheme;
    }
};

#endif // COLORSCHEME_H
