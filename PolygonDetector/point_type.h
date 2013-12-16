#pragma once

#include "opencv2/core/core.hpp"

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

    operator cv::Point() {
        return cv::Point(x, y);
    }

    friend std::ostream & operator <<(std::ostream & out, point_type const & pt) {
        out << "(" << pt.x << ", " << pt.y << ")";
        return out;
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


#define DEF_POINT_BIOPERATION(op) \
        inline point_type operator op(point_type const & a, point_type const & b) { \
                point_type p(a); \
                p.x = p.x op b.x; \
                p.y = p.x op b.y; \
        }

DEF_POINT_BIOPERATION(+)
DEF_POINT_BIOPERATION(-)
DEF_POINT_BIOPERATION(*)
DEF_POINT_BIOPERATION(/)

#undef DEF_POINT_BIOPERATION

inline point_type operator *(point_type const & p, int a) {
    return p * point_type(a, a);
}

inline point_type operator *(int a, point_type const & p) {
    return p * a;
}
