#pragma once

struct point_type {
    int x, y;

    point_type(int x, int y)
    : x(x)
    , y(y) {
    }

    point_type()
    : x(0)
    , y(0) {
    }

};

inline bool operator<(point_type const & a, point_type const & b) {
    if (a.x == b.x)
        return a.y < b.y;
    else
        return a.x < b.x;
}

inline bool operator>(point_type const & a, point_type const & b) {
    return b < a;
}

inline bool operator ==(point_type const & a, point_type const & b) {
    return (a.x == b.x) && (a.y == b.y);
}

inline bool operator !=(point_type const & a, point_type const & b) {
    return !(a == b);
}



