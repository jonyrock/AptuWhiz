
#include "types.h"
#include "io.h"
#include <iostream>
#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

#define REP(i, N) for(int i = 0; i < (int)(N); i++)

void waitEnter(){
    while(waitKey() != 1048586);
}

int main(int argc, char** argv) {

    freopen("contours.txt", "w", stdin);
    
    int N;
    cin >> N;
    
    cout << "building DCEL" << endl;
    
    DCEL dcel;
    
    Mat img(Size(300, 300), CV_LOAD_IMAGE_GRAYSCALE);
    imshow("Contours", img);
    waitEnter();
    
    
    REP(i, N) {
        int n;
        cin >> n;
        bool idlePolygon = n < 10;
            
        point_type pprev;
        cin >> pprev;
        REP(j, n - 1) {
            point_type p;
            cin >> p;
            if(idlePolygon)
                continue;
            dcel.add_segment(p, pprev);
            
            pprev = p;
        }
    }
    
    
    
    cout << "end processing" << endl;


    return 0;
}

