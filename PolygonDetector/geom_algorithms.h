#ifndef GEOM_ALGORITHMS_H
#define	GEOM_ALGORITHMS_H

#include "point_type.h"
#include "segment_type.h"

namespace geom {
    namespace algorithms {

        inline int cross_prod(point_type a, point_type b, point_type c) {
            return a.x * (b.y - c.y) - a.y * (b.x - c.x) + (b.x * c.y - b.y * c.x);
        }

        inline int cross_prod(const segment_type& segment, point_type p) {
            return cross_prod(segment[0], segment[1], p);
        }

        inline int cross_prod(point_type v, point_type w) {
            return v.x * w.y - v.y * w.x;
        }


        int left_turn(point_type a, point_type b, point_type c);

        int left_turn(const segment_type& segment, point_type p);

        bool point_on_line(const point_type& point, const segment_type& segment);

        bool segments_intersected(const segment_type& a, const segment_type& b);

        bool segments_inner_intersected(const segment_type& a, const segment_type& b);

        point_type segments_intesection(const segment_type& a, const segment_type& b);

    }
}


#endif

