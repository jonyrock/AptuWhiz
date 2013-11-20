#pragma once

#include "stdafx.h"
#include "math.h"
#include "geom/primitives/point.h"
#include "geom_algorithms.h"
#include <limits.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace std;
using namespace cv;

typedef std::vector<point_type> polygon_type;

#define BITMAP_RESOLUTION 100

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

Rect get_diameter_bounding_box(const polygon_type& polygon) {
    int minx = INT_MAX, maxx = INT_MIN,
            miny = INT_MAX, maxy = INT_MIN;
    for (auto p : polygon) {
        minx = min(minx, p.x);
        miny = min(miny, p.y);
    }
    int diameter = (int)geom::algorithms::polygon_diameter(polygon);
    return Rect(minx, miny, diameter, diameter);

}

polygon_type get_scaled_polygon(int boxSize, const polygon_type& polygon) {

    polygon_type pc(polygon);

    Rect box = get_diameter_bounding_box(pc);
    Rect bbox = get_bounding_box(pc);
    
    int offsetX = (box.width - bbox.width) / 2;
    int offsetY = (box.height - bbox.height) / 2;

    for (auto& p : pc) {

        int x = p.x - box.x + offsetX;
        int y = p.y - box.y + offsetY;

        x = (x * boxSize) / box.width;
        y = (y * boxSize) / box.height;
        y = boxSize - y;
        p.x = x;
        p.y = y;
    }

    return pc;

}

void draw_polygon(Mat& img, const polygon_type& pc) {
    for (size_t i = 1; i < pc.size(); i++) {
        Point pa(pc[i].x, pc[i].y);
        Point pb(pc[i - 1].x, pc[i - 1].y);
        line(img, pa, pb, Scalar(0));
    }
    Point pa(pc.front().x, pc.front().y);
    Point pb(pc.back().x, pc.back().y);
    line(img, pa, pb, Scalar(0));
}

void cut_outer_points(Mat& img, const polygon_type& polygon){
    //    for (int i = 0; i < BITMAP_RESOLUTION; i++) {
    //        for (int j = 0; j < BITMAP_RESOLUTION; j++) {
    //            // i - row, j - column
    //            bool inside = geom::algorithms::is_point_inside(pc, point_type(j, i));
    //            if (!inside) {
    //                img.at<uchar>(i, j) = (uchar) 255;
    //            }
    //        }
    //    }
}

void draw_gradient_polygon_blur(Mat& img, const polygon_type& polygon) {

    polygon_type pc = get_scaled_polygon(BITMAP_RESOLUTION, polygon);

    for (int i = 0; i < 100; i++) {
        draw_polygon(img, pc);
        GaussianBlur(img, img, Size(25, 25), 20.0);
    }

}

void draw_gradient_polygon_dist(Mat& img, const polygon_type& polygon) {

    int offsetX = (img.cols - BITMAP_RESOLUTION) / 2;
    int offsetY = (img.rows - BITMAP_RESOLUTION) / 2;

    polygon_type pc = get_scaled_polygon(BITMAP_RESOLUTION, polygon);

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            double d = geom::algorithms::dist(pc, point_type(j - offsetX, i - offsetY));
            d *= 3;
            d = min(d, 255.0);
            uchar color = (uchar) d;
            img.at<uchar>(i, j) = color;
        }
    }
}

void show_polygon(const polygon_type& polygon) {
    Mat img(Size(BITMAP_RESOLUTION * 2, BITMAP_RESOLUTION * 2),
            CV_LOAD_IMAGE_GRAYSCALE, 255);
    draw_gradient_polygon_dist(img, polygon);
    imshow("some win", img);
}

long long cost(const Mat& source, const Mat& dist,
        int offsetX, int offsetY,
        double angle, float scale = 1.0) {

    Point center = Point(dist.cols / 2, dist.rows / 2);
    Mat rot_mat = getRotationMatrix2D(center, angle, scale);
    Mat warp_dst = Mat::zeros(dist.rows, dist.cols, dist.type());
    Mat dist_tr;
    warpAffine(dist, dist_tr, rot_mat, warp_dst.size(),
            INTER_LINEAR, BORDER_CONSTANT, 255);

    long long cost = 0;
    for (int i = 0; i < source.rows; i++) {
        for (int j = 0; j < source.cols; j++) {
            int di = i + offsetY;
            int dj = j + offsetX;
            if (di < 0 || di >= source.rows) continue;
            if (dj < 0 || dj >= source.cols) continue;
            uchar a = source.at<uchar>(i + offsetY, j + offsetX);
            uchar b = dist_tr.at<uchar>(i, j);
            long long myCost = max(a, b) - min(a, b);
            cost += myCost;
        }
    }
    return cost;
}

int kkk = 0;
int match(const polygon_type& pca, const polygon_type& pcb) {

    Mat imga(Size(BITMAP_RESOLUTION * 2, BITMAP_RESOLUTION * 2),
            CV_LOAD_IMAGE_GRAYSCALE, 255);

    draw_gradient_polygon_dist(imga, pca);
    
//    imshow("some win", imga);

    Mat imgb(Size(BITMAP_RESOLUTION * 2, BITMAP_RESOLUTION * 2),
            CV_LOAD_IMAGE_GRAYSCALE, 255);

    draw_gradient_polygon_dist(imgb, pcb);
//    imshow("some win" + kkk, imgb);
    kkk++;

    int offsetX = 0;
    int offsetY = 0;
    double angle = 0;
    double scale = 1.0;
    long long bestCost = 0;

    while (true) {
        long long myCost = cost(imga, imgb, offsetX, offsetY, angle, scale);
        long long myCost_xm = cost(imga, imgb, offsetX - 1, offsetY, angle, scale);
        long long myCost_xp = cost(imga, imgb, offsetX + 1, offsetY, angle, scale);
        long long myCost_ym = cost(imga, imgb, offsetX, offsetY - 1, angle, scale);
        long long myCost_yp = cost(imga, imgb, offsetX, offsetY + 1, angle, scale);
        long long myCost_am = cost(imga, imgb, offsetX, offsetY, angle - 1, scale);
        long long myCost_ap = cost(imga, imgb, offsetX, offsetY, angle + 1, scale);

        vector<long long> results;
        results.push_back(myCost);
        results.push_back(myCost_xm);
        results.push_back(myCost_xp);
        results.push_back(myCost_ym);
        results.push_back(myCost_yp);
        results.push_back(myCost_am);
        results.push_back(myCost_ap);
        //        results.push_back(myCost_sm);
        //        results.push_back(myCost_sp);

        size_t mei = min_element(results.begin(), results.end()) - results.begin();

        if (mei == 0) {
            bestCost = myCost;
            break;
        }

        if (mei == 1) {
            offsetX--;
            continue;
        }

        if (mei == 2) {
            offsetX++;
            continue;
        }

        if (mei == 3) {
            offsetY--;
            continue;
        }

        if (mei == 4) {
            offsetY++;
            continue;
        }

        if (mei == 5) {
            angle -= 1;
            continue;
        }

        if (mei == 6) {
            angle += 1;
            continue;
        }

        if (mei == 7) {
            scale -= 0.1;
            continue;
        }

        if (mei == 8) {
            angle += 0.1;
            continue;
        }

    }

    cout << offsetX << " " << offsetY << " " << angle << "->";
    return bestCost;

}

size_t find_best_match_i(const polygon_type& p, vector<polygon_type>& others) {

    long long best = LONG_MAX;
    size_t bestI = 0;
    for (size_t i = 0; i < others.size(); i++) {
        long long myCostGrad = match(p, others[i]);
//        long long myCostBruut = matchBruut(p, others[i]);
        cout << " cost " << i << ": " << myCostGrad << endl;
        if (myCostGrad < best) {
            bestI = i;
            best = myCostGrad;
        }
    }

    return bestI;
}


