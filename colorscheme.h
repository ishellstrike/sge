#ifndef COLORSCHEME_H
#define COLORSCHEME_H
#include <glm/glm.hpp>
#include "colorextender.h"

struct ColorScheme
{
    glm::vec4 border_up, border_down, hovered, basic, pressed, text, hovered_text;

    static ColorScheme Basic();

    static ColorScheme FromColor(const glm::vec4 &col);
};

#endif // COLORSCHEME_H
