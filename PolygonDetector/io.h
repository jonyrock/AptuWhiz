#pragma once

#include "types.h"

std::istream & skip_char(std::istream & in, char ch) {
    char c;
    while ((in >> c) && (c != ch));
    return in;
}

std::istream & operator >>(std::istream & in, point_type & pt) {
    return skip_char(skip_char(skip_char(in, '(') >> pt.x, ',') >> pt.y, ')');
}

