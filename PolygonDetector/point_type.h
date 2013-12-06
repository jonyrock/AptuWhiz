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


