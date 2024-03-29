
#include "types.h"
#include "io.h"
#include <iostream>
#include <stdio.h>
#include <random>

#include "geom_algorithms.h"
using namespace geom::algorithms;

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

#define REP(i, N) for(int i = 0; i < (int)(N); i++)


int WIDTH, HEIGHT;
vector<polygon_type> POLYGONS;

void read_data() {
    auto f = freopen("contours.txt", "r", stdin);
    if (!f) {
        cerr << "can't open file" << endl;
        return;
    }
    cin >> WIDTH >> HEIGHT;

    int N;
    cin >> N;

    REP(i, N) {
        int n;
        cin >> n;
        polygon_type ps;
        point_type pprev;
        cin >> pprev;
        ps.push_back(pprev);

        REP(j, n - 1) {
            point_type p;
            cin >> p;
            ps.push_back(p);
            pprev = p;
        }
        if (ps.size() >= 5)
            POLYGONS.push_back(ps);
    }
}

void waitEnter() {

    while (true) {
        int key = waitKey();
        if (key == 1048586)
            break;
        if (key == 10)
            break;
    }
}

DCEL* build_dcel(int step) {
    DCEL* dcel = new DCEL(step, WIDTH, HEIGHT);

    REP(i, POLYGONS.size()) {
        polygon_type& ps = POLYGONS[i];
        point_type pprev = ps[0];

        REP(j, (ps.size() - 1)) {
            point_type p = ps[j + 1];
            dcel->add_segment(pprev, p);
            pprev = p;
        }
    }
    return dcel;
}

void draw_dcel(Mat& img, const DCEL* dcel) {
    for (auto e : dcel->edges) {
        int c1 = 255 - (rand() % 200);
        int c2 = 255 - (rand() % 200);
        int c3 = 255 - (rand() % 200);
        line(img, e->from().point, e->to().point, Scalar(c1, c2, c3));
    }
}

void show_dcel(DCEL* dcel) {
    Mat img = Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
    draw_dcel(img, dcel);
    Mat imgResized = Mat::zeros(HEIGHT * 2, WIDTH * 2, CV_8UC3);
    resize(img, imgResized, Size(HEIGHT * 2, WIDTH * 2));
    imshow("Contours", imgResized);
}

void draw_polygon(Mat& img, const polygon_type& ps) {
    Point psp[1][ps.size()];
    for (size_t i = 0; i < ps.size(); i++) {
        psp[0][i] = Point(ps[i].x, ps[i].y);
    }
    const Point * ppt[1] = {psp[0]};
    int npt[] = {(int) ps.size()};
    fillPoly(img,
            ppt,
            npt,
            1,
            Scalar(255, 255, 255), 8);

}

void show_polygon(const polygon_type& ps) {
    Mat img = Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
    draw_polygon(img, ps);
    Mat imgResized = Mat::zeros(HEIGHT * 2, WIDTH * 2, CV_8UC3);
    resize(img, imgResized, Size(HEIGHT * 2, WIDTH * 2));
    imshow("Contours", imgResized);
}

int main(int argc, char** argv) {
    cout << "read data" << endl;
    read_data();

    REP(step, 20) {
        cout << "building DCEL " << step + 1 << endl;
        DCEL* dcel = build_dcel(step + 1);
        show_dcel(dcel);
        waitEnter();
        
        dcel->merge_near();
        show_dcel(dcel);
        waitEnter();
        
        dcel->delete_leafs();
        show_dcel(dcel);
        waitEnter();
        
        
        cout << "end processing" << endl;
        auto inPol = dcel->find_center_polygon();
//        cout << "---------" << endl;
//        for (auto p : inPol) {
//            cout << p << " ";
//        }
//        cout << endl << "xxxxxxx" << endl;
        //        drawContours()
        show_polygon(inPol);
        waitEnter();

        delete dcel;
    }

    return 0;
}

