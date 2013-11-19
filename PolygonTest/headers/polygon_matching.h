#ifndef POLYGON_MATCHING_H
#define	POLYGON_MATCHING_H
#include "stdafx.h"
#include "math.h"
#include "geom/primitives/point.h"
//#include "algorithms.h"
#include <limits.h>"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "algorithms.h"

using namespace std;
using namespace cv;

typedef std::vector<point_type> polygon_type;

#define BITMAP_RESOLUTION 200

// be careful, because it return not in screen coordinates

Rect get_bounding_box(const polygon_type& polygon) {
    int minx = INT_MAX, maxx = INT_MIN,
            miny = INT_MAX, maxy = INT_MIN;
    for (auto p : polygon) {
        minx = min(minx, p.x);
        maxx = max(maxx, p.x);
        miny = min(miny, p.y);
        maxy = max(maxy, p.y);
    }
    return Rect(minx, miny, maxx - minx, maxy - miny);

}

void rotate(polygon_type& polygon, double angle) {

    int minx = INT_MAX, maxx = INT_MIN,
            miny = INT_MAX, maxy = INT_MIN;

    // TODO: use get_bounding_box()
    double a = (angle / 360.0) * M_PI;
    for (auto p : polygon) {
        minx = min(minx, p.x);
        maxx = max(maxx, p.x);
        miny = min(miny, p.y);
        maxy = max(maxy, p.y);
    }

    int centerx = (maxx + minx) / 2;
    int centery = (maxy + miny) / 2;

    for (size_t i = 0; i < polygon.size(); i++) {
        int x = polygon[i].x - centerx;
        int y = polygon[i].y - centery;
        int newx = x * cos(a) - y * sin(a);
        int newy = x * sin(a) + y * cos(a);
        polygon[i].x = newx + centerx;
        polygon[i].y = newy + centery;
    }

}

polygon_type get_scaled_polygon(int boxSize, const polygon_type& polygon) {

    polygon_type pc(polygon);

    Rect box = get_bounding_box(pc);

    for (auto& p : pc) {
        int x = p.x - box.x;
        int y = p.y - box.y;
        x = (x * boxSize) / box.width;
        y = (y * boxSize) / box.height;
        y = boxSize - y;
        p.x = x;
        p.y = y;
    }

    return pc;

}

void draw_polygon(Mat& img, const polygon_type& pc) {
    for (size_t i = 0; i < pc.size(); i++) {
        Point pa(pc[i].x, pc[i].y);
        Point pb(pc[i - 1].x, pc[i - 1].y);
        line(img, pa, pb, Scalar(0));
    }
    Point pa(pc.front().x, pc.front().y);
    Point pb(pc.back().x, pc.back().y);
    line(img, pa, pb, Scalar(0));
}

void draw_gradient_polygon(Mat& img, const polygon_type& polygon){
    
    polygon_type pc = get_scaled_polygon(BITMAP_RESOLUTION, polygon);

    for (int i = 0; i < 100; i++) {
        draw_polygon(img, pc);
        GaussianBlur(img, img, Size(25, 25), 20.0);
    }

    for (int i = 0; i < BITMAP_RESOLUTION; i++) {
        for (int j = 0; j < BITMAP_RESOLUTION; j++) {
            // i - row, j - column
            bool inside = geom::algorithms::is_point_inside(pc, point_type(j, i));
            if (!inside) {
                img.at<uchar>(i, j) = (uchar) 255;
            }
        }
    }
    
}

void show_polygon(const polygon_type& polygon) {

    Mat gray_buf(Size(BITMAP_RESOLUTION, BITMAP_RESOLUTION),
            CV_LOAD_IMAGE_GRAYSCALE, 255);
    
    draw_gradient_polygon(gray_buf, polygon);
    

    imshow("some win", gray_buf);

}

int match(const polygon_type& a, const polygon_type& b) {

    return 10;
}

size_t find_best_match_i(const polygon_type& p, vector<polygon_type>& others) {
    int best = 0;
    size_t bestI = 0;
    for (size_t i = 0; i < others.size(); i++) {
        int myCost = match(p, others[i]);
        if (myCost > best) {
            bestI = i;
        }
    }
    show_polygon(others[bestI]);
    return bestI;
}

#endif

