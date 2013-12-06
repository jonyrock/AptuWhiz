
#include "types.h"
#include "io.h"
#include <iostream>
#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

#define REP(i, N) for(int i = 0; i < (int)(N); i++)

void waitEnter() {
    while (waitKey() != 1048586);
}

DCEL* build_dcel() {
    
    int N;
    cin >> N;

    cout << "building DCEL" << endl;

    DCEL* dcel = new DCEL();

    REP(i, N) {
        int n;
        cin >> n;
        bool idlePolygon = n < 10;
        point_type pprev;
        cin >> pprev;

        REP(j, n - 1) {
            point_type p;
            cin >> p;
            if (idlePolygon)
                continue;
            dcel->add_segment(pprev, p);


            pprev = p;
        }
    }

    return dcel;

}

int main(int argc, char** argv) {
    
    auto f = freopen("contours.txt", "r", stdin);
    if (!f) {
        cerr << "can't open file" << endl;
        return 0;
    }
    
    int width, height;
    cin >> width >> height;
    
    Mat img(Size(width, height), CV_LOAD_IMAGE_GRAYSCALE, 255);

    DCEL* dcel = build_dcel();

    for(auto e : dcel->edges){
        line(img, e->from().point, e->to().point, 50);
    }

    imshow("Contours", img);
    waitEnter();

    cout << "end processing" << endl;


    return 0;
}

