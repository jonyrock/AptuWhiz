
#include "geom_algorithms.h"
#include <cassert>
#include <algorithm>


namespace geom {
    namespace algorithms {

        int left_turn(point_type a, point_type b, point_type c) {
            int det = cross_prod(a, b, c);
            if (det > 0) return 1;
            if (det < 0) return -1;
            return 0;
        }

        int left_turn(const segment_type& segment, point_type p) {
            return left_turn(segment[0], segment[1], p);
        }

        bool segments_inner_intersected(const segment_type& a,
                const segment_type& b) {

            if (std::max(a[0].x, a[1].x) < std::min(b[0].x, b[1].x))
                return false;
            if (std::max(b[0].x, b[1].x) < std::min(a[0].x, a[1].x))
                return false;
            if (std::max(a[0].y, a[1].y) < std::min(b[0].y, b[1].y))
                return false;
            if (std::max(b[0].y, b[1].y) < std::min(a[0].y, a[1].y))
                return false;

            int res1 = left_turn(a[0], a[1], b[0]);
            res1 *= left_turn(a[0], a[1], b[1]);

            int res2 = left_turn(b[0], b[1], a[0]);
            res2 *= left_turn(b[0], b[1], a[1]);

            return (res1 == -1 && res2 == -1);

        }

        bool segments_intersected(const segment_type& a,
                const segment_type& b) {
            if (segments_inner_intersected(a, b))
                return true;
            int res1 = left_turn(a[0], a[1], b[0]);
            res1 *= left_turn(a[0], a[1], b[1]);

            int res2 = left_turn(b[0], b[1], a[0]);
            res2 *= left_turn(b[0], b[1], a[1]);

            return (res1 * res2) == 0;
        }

        // check before that segments_inner_intersected !
        point_type segments_intesection(const segment_type& a, const segment_type& b) {
            assert(segments_inner_intersected(a, b));
            // a -> p + tr  // a[0] = p
            // b -> q + us  // b[0] = q
            // u = (q − p) × r / (r × s)
            point_type r = a[1] - a[0];
            point_type s = b[1] - b[0];
            int dc = cross_prod(r, s);
            int tc = cross_prod(b[0] - a[0], r);
            double u = (tc * 1.0) / dc;
            point_type us = u * s;
            return b[0] + us;
        }
        
        
        double segment_length(const segment_type& s) {
            double xLen = s[0].x - s[1].x * 1.0;
            double yLen = s[0].y - s[1].y * 1.0;
            return sqrt(xLen * xLen + yLen * yLen);
        }
        
    }

}