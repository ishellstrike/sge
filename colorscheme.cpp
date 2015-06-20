#include "colorscheme.h"

ColorScheme ColorScheme::Basic()
{
    static ColorScheme basic = ColorScheme::FromColor(glm::vec4(Color::Gray.xyz(), 0.8f));
    return basic;
}

ColorScheme ColorScheme::FromColor(const glm::vec4 &col)
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
