#include "stdafx.h"

#include "app_viewer.h"

int main(int argc, char ** argv) {
    QApplication app(argc, argv);
    app_viewer viewer;
    visualization::run_viewer(&viewer, "Polygon matching test");
}

