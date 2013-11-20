#include "stdafx.h"

#include "app_viewer.h"
#include <iostream>
using namespace std;
int main(int argc, char ** argv) {
    cout << "hello" << endl;
    QApplication app(argc, argv);
    app_viewer viewer;
    visualization::run_viewer(&viewer, "Polygon matching test");
}

