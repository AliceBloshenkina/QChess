#ifndef COLOR_OPPOSITE_H
#define COLOR_OPPOSITE_H

#include "color.h"

class Color_opposite{
public:
    static Color opposite(Color c)
    {
        return (c == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }
};
#endif
