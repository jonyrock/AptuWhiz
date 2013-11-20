
#include "geom_algorithms.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <stack>
#include "io/point.h"

namespace geom {
    namespace algorithms {

        void triangulate(const polygon_type& polygon, vector<segment_type>& res) {
            vector<pair<size_t, size_t> > diagonals = get_tri_split(polygon);
        }

        int cross_prod(point_type a, point_type b, point_type c) {
            return a.x * (b.y - c.y) - a.y * (b.x - c.x) + (b.x * c.y - b.y * c.x);
        }

        int cross_prod(const segment_type& segment, point_type p) {
            return cross_prod(segment[0], segment[1], p);
        }

        double polygon_diameter(const polygon_type& pc) {
            double biggestDist = 0;
            for (int i = 0; i < pc.size(); i++) {
                for (int j = i + 1; j < pc.size(); j++) {
                    double myDist = segment_length(segment_type(pc[i], pc[j]));
                    biggestDist = std::max(biggestDist, myDist);
                }
            }
            return biggestDist;
        }

        int left_turn(point_type a, point_type b, point_type c) {
            int det = cross_prod(a, b, c);
            if (det > 0) return 1;
            if (det < 0) return -1;
            return 0;
        }

        int left_turn(const segment_type& segment, point_type p) {
            return left_turn(segment[0], segment[1], p);
        }

        bool point_on_line(const point_type& point, const segment_type& segment) {
            if (left_turn(segment, point) != 0) return false;
            if (point.x < std::min(segment[0].x, segment[1].x)) return false;
            if (point.x > std::max(segment[0].x, segment[1].x)) return false;
            if (point.y < std::min(segment[0].y, segment[1].y)) return false;
            if (point.y > std::max(segment[0].y, segment[1].y)) return false;
            return true;
        }

        bool segments_intersected(const segment_type& a, const segment_type& b) {

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

            return (res1 <= 0 && res2 <= 0);

        }

        double segment_length(const segment_type& s) {
            double xLen = s[0].x - s[1].x * 1.0;
            double yLen = s[0].y - s[1].y * 1.0;
            return sqrt(xLen * xLen + yLen * yLen);
        }

        bool orient_polygon_anticlockwise(vector<point_type>& pts_) {
            if (pts_.size() < 2) return false;
            auto minIt = min_element(pts_.begin(), pts_.end());
            size_t minI = minIt - pts_.begin();

            point_type left_most_v = pts_[minI];
            point_type next_v = pts_[(minI + 1) % pts_.size()];
            point_type prev_v = pts_[(minI - 1 + pts_.size()) % pts_.size()];

            bool isLeft = left_turn(prev_v, left_most_v, next_v) > 0;


            if (isLeft) return true;

            reverse(pts_.begin(), pts_.end());

            return false;
        }

        bool orient_polygon_clockwise(vector<point_type>& pts_) {
            bool res = orient_polygon_anticlockwise(pts_);
            reverse(pts_.begin(), pts_.end());
            return !res;
        }

        bool check_intersections(const vector<point_type>& pts_) {
            for (uint i = 0; i < pts_.size(); i++) {
                const segment_type segmentA(
                        pts_[i],
                        pts_[(i + 1) % pts_.size()]
                        );
                for (uint t = 2; t <= pts_.size() - 2; t++) {
                    const segment_type segmentB(
                            pts_[(i + t) % pts_.size()],
                            pts_[(i + t + 1) % pts_.size()]
                            );
                    if (segments_intersected(segmentA, segmentB)) {
                        cout << "Was intersection : "
                                << segmentA[0] << segmentA[1] << " - "
                                << segmentB[0] << segmentB[1] << endl;
                        return true;
                    }
                }
                const segment_type segmentNext(
                        pts_[(i + 1) % pts_.size()],
                        pts_[(i + 2) % pts_.size()]
                        );
                if (point_on_line(segmentNext[1], segmentA) ||
                        point_on_line(segmentA[0], segmentNext)) {
                    return true;
                }
            }
            return false;
        }

        void triangulate_monotonous(const vector<point_type>& pts_, vector<segment_type>& res) {

            vector<point_type> pts(pts_);
            auto minIt = min_element(pts.begin(), pts.end());

            rotate(pts.begin(), minIt, pts.end());

            size_t maxI = max_element(pts.begin(), pts.end()) - pts.begin();

            vector<size_t> orderByXY;
            size_t upI = pts.size() - 1;
            size_t lowI = 0;
            while (upI >= maxI || lowI < maxI) {
                if (upI < maxI || pts[lowI] < pts[upI]) {
                    orderByXY.push_back(lowI++);
                    continue;
                }
                orderByXY.push_back(upI--);
            }

            stack<size_t> st;
            st.push(orderByXY[0]);
            st.push(orderByXY[1]);


            for (size_t i = 2; i < pts.size() - 1; i++) {
                bool prevWasTop = st.top() >= maxI;
                bool meTop = orderByXY[i] >= maxI;
                auto curPoint = pts[orderByXY[i]];

                if (prevWasTop != meTop) {
                    auto newTop = st.top();
                    while (st.size() > 1) {
                        res.push_back(segment_type(pts[st.top()], curPoint));
                        st.pop();
                    }
                    st.pop();
                    st.push(newTop);
                } else {
                    int currentTurn = meTop ? 1 : -1;
                    while (st.size() > 1) {
                        auto bi = st.top();
                        st.pop();
                        auto ci = st.top();
                        bool isRightTurn = left_turn(curPoint, pts[bi], pts[ci]) == currentTurn;
                        if (isRightTurn) {
                            res.push_back(segment_type(curPoint, pts[ci]));
                        } else {
                            st.push(bi);
                            break;
                        }
                    }
                }

                st.push(orderByXY[i]);

            }

            st.pop();
            while (st.size() > 1) {
                res.push_back(segment_type(pts[st.top()], pts[maxI]));
                st.pop();
            }

        }

        bool is_point_inside(const vector<point_type>& polygon, const point_type& point) {
            int32 minx = min_element(polygon.begin(), polygon.end()) -> x;
            segment_type ray(point_type(minx - 1, point.y + 1), point);
            bool inside = false;
            for (size_t i = 0; i < polygon.size(); i++) {
                point_type from = polygon[i];
                point_type to = polygon[(i + 1) % polygon.size()];
                if (segments_intersected(segment_type(from, to), ray))
                    inside = !inside;
            }
            return inside;
        }

        bool is_polygon_inside(const vector<point_type>& polygonOuter, const vector<point_type>& polygonIn) {
            for (auto p : polygonIn) {
                if (!is_point_inside(polygonOuter, p)) return false;
            }
            return true;
        }

        bool is_polygons_intersected(const vector<point_type>& polygonA, const vector<point_type>& polygonB) {
            for (size_t i = 0; i < polygonA.size(); i++) {
                point_type froma = polygonA[i];
                point_type toa = polygonA[(i + 1) % polygonA.size()];
                segment_type sa(froma, toa);
                for (size_t j = 0; j < polygonB.size(); j++) {
                    point_type fromb = polygonB[i];
                    point_type tob = polygonB[(i + 1) % polygonB.size()];
                    segment_type sb(fromb, tob);
                    if (segments_intersected(sa, sb)) return true;
                }
            }
            return false;
        }

        bool is_polygons_intersected(const vector<vector<point_type> >& polygons,
                const vector<point_type>& polygon) {
            for (auto p : polygons) {
                if (is_polygons_intersected(p, polygon)) return true;
            }
            return false;
        }

        // it is not used in real triangulation method

        vector<pair<point_type, TRIP_TYPE> > get_points_types(
                const vector<point_type>& polygon,
                const vector<vector<point_type> >& holes) {

            vector<pair<point_type, TRIP_TYPE> > typesRes;

            for (size_t i = 0; i < polygon.size(); i++) {
                typesRes.push_back(
                        make_pair(polygon[i], get_trip_type(polygon, i, false)));
            }

            for (auto hole : holes)
                for (size_t i = 0; i < hole.size(); i++) {
                    typesRes.push_back(
                            make_pair(hole[i], get_trip_type(hole, i, false)));
                }

            return typesRes;

        }

        vector<pair<size_t, size_t> > get_tri_split(const vector<point_type>& polygon) {

            vector<pair<size_t, size_t > > res;

            vector<size_t> orderByXY;
            for (size_t i = 0; i < polygon.size(); i++) orderByXY.push_back(i);

            sort(orderByXY.begin(), orderByXY.end(),
                    [&polygon](size_t i, size_t j) {
                        return polygon[i] < polygon[j];
                    });


            Status status(polygon);
            for (size_t i : orderByXY) {

                auto type = get_trip_type(polygon, i, false);
                if (type == TRIP_START) {
                    status.add(i);
                    continue;
                }

                bool foundGoodHelper = false;
                size_t helper = status.helper(i);
                if (helper != (size_t) (-1) && helper != prev(polygon, i)) {
                    foundGoodHelper = true;
                }
                if (!foundGoodHelper) {
                    size_t lowerSegmentI = status.find_lower_segment(i);
                    if (lowerSegmentI != (size_t) (-1)) {
                        helper = status.helper(lowerSegmentI);
                        foundGoodHelper = true;
                    }
                }
                if (foundGoodHelper) {
                    TRIP_TYPE helperType = get_trip_type(polygon, helper, false);
                    cout << "(" << i << ", " << helper << ") " << endl;
                    if (helperType == TRIP_MERGE) {
                        res.push_back(make_pair(helper, i));
                    }
                    if (type == TRIP_SPLIT) {
                        res.push_back(make_pair(helper, i));
                    }
                }
                status.remove(i);
                status.update(i);
                status.add(i);
            }

            return res;

        }

        TRIP_TYPE get_trip_type(const vector<point_type>& polygon, size_t index,
                bool isInHole) {
            auto pbefore = polygon[(index > 0) ? index - 1 : polygon.size() - 1];
            auto pafter = polygon[(index < polygon.size() - 1) ? index + 1 : 0];

            auto pme = polygon[index];

            if (pbefore.x < pme.x && pme.x < pafter.x) {
                return TRIP_REGULAR;
            }

            if (pme.x <= pbefore.x && pme.x < pafter.x) {
                auto turn = left_turn(pbefore, pme, pafter);
                if (isInHole) turn *= -1;
                if (turn > 0) return TRIP_START;

            }
            if (pme.x < pbefore.x && pme.x < pafter.x) {
                return TRIP_SPLIT;
            }

            if (pme.x > pbefore.x && pme.x >= pafter.x) {
                auto turn = left_turn(pbefore, pme, pafter);
                if (isInHole) turn *= -1;
                if (turn > 0) return TRIP_END;
                else return TRIP_MERGE;
            }

            return TRIP_REGULAR;

        }

        double dot_product(const point_type& a, const point_type& b) {
            return a.x * b.x + a.y * b.y;
        }

        double dist(const segment_type& s, const point_type& p) {
            int cross = cross_prod(s, p);
            double len = segment_length(s);
            double lenLine = abs(cross / len);
            double lenLeft = segment_length(segment_type(p, s[0]));
            double lenRight = segment_length(segment_type(p, s[1]));

            point_type vl(s[1].x - s[0].x, s[1].y - s[0].y);
            point_type wl(p.x - s[0].x, p.y - s[0].y);
            double c1 = dot_product(wl, vl);
            if (c1 <= 0)
                return lenLeft;

            point_type vr(s[0].x - s[1].x, s[0].y - s[1].y);
            point_type wr(p.x - s[1].x, p.y - s[1].y);
            double c2 = dot_product(wr, vr);
            if (c2 <= 0)
                return lenRight;
            return lenLine;
        }

        double dist(const point_type& p, const segment_type& s) {
            return dist(s, p);
        }

        double dist(const polygon_type& pc, const point_type& p) {
            double minDist = INT_MAX * 1.0;
            for (size_t i = 1; i < pc.size(); i++) {
                point_type pa(pc[i].x, pc[i].y);
                point_type pb(pc[i - 1].x, pc[i - 1].y);
                double myDist = dist(segment_type(pa, pb), p);
                if (myDist < minDist)
                    minDist = myDist;
            }
            point_type pa(pc.front().x, pc.front().y);
            point_type pb(pc.back().x, pc.back().y);
            double myDist = dist(segment_type(pa, pb), p);
            if (myDist < minDist) minDist = myDist;
            return minDist;
        }
    }
}


