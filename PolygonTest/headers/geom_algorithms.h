#pragma once

#include "stdafx.h"
#include <limits.h>
#include "geom/primitives/point.h"
#include "geom/primitives/vector.h"
#include "geom/primitives/segment.h"
#include <set>
#include <map>

using namespace std;

namespace geom {
    namespace algorithms {

        using namespace geom::structures;

        enum TRIP_TYPE {
            TRIP_SPLIT,
            TRIP_MERGE,
            TRIP_START,
            TRIP_END,
            TRIP_REGULAR
        };

        typedef std::vector<point_type> polygon_type;

        void triangulate(const polygon_type& pts_, vector<segment_type>& res);
        
        int cross_prod(point_type a, point_type b, point_type c);
        
        int cross_prod(const segment_type& segment, point_type p);
        
        double polygon_diameter(const polygon_type& pc);
        
        int left_turn(point_type a, point_type b, point_type c);

        int left_turn(const segment_type& segment, point_type p);

        bool point_on_line(const point_type& point, const segment_type& segment);

        bool segments_intersected(const segment_type& a, const segment_type& b);
        
        double segment_length(const segment_type& s);

        // return true if polygon was oriented clockwise
        bool orient_polygon_anticlockwise(vector<point_type>& pts_);

        bool orient_polygon_clockwise(polygon_type& pts_);

        bool check_intersections(const polygon_type& pts_);

        void triangulate_monotonous(const polygon_type& pts_,
                vector<segment_type>& res);

        bool is_polygon_inside(const polygon_type& polygonOuter,
                const polygon_type& polygonIn);
        bool is_polygons_intersected(const polygon_type& polygonA,
                const polygon_type& polygonB);
        bool is_polygons_intersected(const vector<polygon_type>& polygons,
                const polygon_type& polygon);
        vector<pair<size_t, size_t> > get_tri_split(const polygon_type& polygon);

        bool is_point_inside(const vector<point_type>& polygon, const point_type& point);

        vector<pair<point_type, TRIP_TYPE> > get_points_types(
                const polygon_type& polygon,
                const vector<polygon_type>& holes);

        TRIP_TYPE get_trip_type(const polygon_type& polygon, size_t index,
                bool isInHole);

        inline size_t next(const polygon_type& polygon, size_t i) {
            return (i + 1) % polygon.size();
        }

        inline point_type nextp(const polygon_type& polygon, size_t i) {
            return polygon[next(polygon, i)];
        }

        inline size_t prev(const polygon_type& polygon, size_t i) {
            return ((i + polygon.size()) - 1) % polygon.size();
        }

        inline point_type prevp(const polygon_type& polygon, size_t i) {
            return polygon[prev(polygon, i)];
        }

        inline bool is_lower_regular(const polygon_type& polygon, size_t i) {
            TRIP_TYPE type = get_trip_type(polygon, i, false);
            if (type != TRIP_REGULAR) return false;
            return nextp(polygon, i).x > polygon[i].x;
        }

        double dist(const segment_type& s, const point_type& p);

        double dist(const point_type& p, const segment_type& s);
        
        double dist(const polygon_type& pc, const point_type& p);
        
        class Status {
        public:

            Status(const polygon_type& polygon) : polygon(polygon) {
            }

            void add(size_t i) {
                TRIP_TYPE type = get_trip_type(polygon, i, false);
                if (type == TRIP_START || type == TRIP_SPLIT) {
                    insert(next(polygon, i));
                }
                if (type == TRIP_REGULAR && is_lower_regular(polygon, i)) {
                    insert(next(polygon, i));
                }
            }

            void update(size_t i) {
                TRIP_TYPE type = get_trip_type(polygon, i, false);
                if (type == TRIP_REGULAR && is_lower_regular(polygon, i))
                    return;
                if (type == TRIP_START)
                    return;
                if (type == TRIP_END)
                    return;

                auto left = find_lower_segment(i);
                if (left == (size_t) (-1)) return;
                cout << "helper[" << left << "] " << helper_[left] << "->" << i << endl;
                helper_[left] = i;
            }

            size_t find_lower_segment(size_t i) {
                cout << "search: " << i << ": ";
                auto lastGood = segments.end();
                for (auto it = segments.begin(); it != segments.end(); it++) {
                    cout << prev(polygon, *it) << " " << *it << " ";
                    point_type pa = prevp(polygon, *it);
                    point_type pb = polygon[*it];
                    int turn = left_turn(segment_type(pa, pb), polygon[i]);
                    cout << turn << "::";
                    if (turn > 0) lastGood = it;
                    else break;
                }
                cout << " -- res: " << *lastGood << endl;

                if (lastGood == segments.end()) {
                    cout << "NOT FOUND LOWER SEGMENT FOR " << i << "!" << endl;
                    return (size_t) - 1;
                }

                return *lastGood;
            }

            void remove(size_t i) {
                cout << "remove " << i << endl;

                TRIP_TYPE type = get_trip_type(polygon, i, false);
                if (type == TRIP_SPLIT || type == TRIP_START)
                    return;
                if (type == TRIP_REGULAR && !is_lower_regular(polygon, i))
                    return;

                auto it = find(segments.begin(), segments.end(), i);
                if (it == segments.end()) {
                    cout << "NOT FOUND SEGMENT END " << i << " !" << endl;
                    return;
                }
                segments.erase(it);
            }

            size_t helper(size_t i) {
                auto it = helper_.find(i);
                if (it != helper_.end()) {
                    return helper_[i];
                }
                return (size_t) (-1);
            }




        private:
            const polygon_type& polygon;
            map<size_t, size_t> helper_;
            vector<size_t> segments;

            void insert(size_t i) {
                cout << "insert " << i;
                point_type leftEnd = prevp(polygon, i);
                auto it = segments.begin();
                while (it != segments.end()) {
                    auto pa = prevp(polygon, *it);
                    auto pb = polygon[*it];
                    int turn = left_turn(segment_type(pa, pb), leftEnd);
                    if (turn < 0) break;
                    it++;
                }
                segments.insert(it, i);
                cout << " Now size " << segments.size() << endl;
                helper_[i] = prev(polygon, i);
            }

        };


    }
}
