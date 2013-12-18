#pragma once

#include "types.h"

inline std::istream & skip_char(std::istream & in, char ch) {
    char c;
    while ((in >> c) && (c != ch));
    return in;
}

inline std::istream & operator >>(std::istream & in, point_type & pt) {
    return skip_char(skip_char(skip_char(in, '(') >> pt.x, ',') >> pt.y, ')');
}

inline std::ostream & operator <<(std::ostream & out, point_type const & pt) {
    out << "(" << pt.x << ", " << pt.y << ")";
    return out;
}

inline std::ostream & operator <<(std::ostream & out, segment_type const & s) {
    out << "(" << s[0] << ", " << s[1] << ")";
    return out;
}

