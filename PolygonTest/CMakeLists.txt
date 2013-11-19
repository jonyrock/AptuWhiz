cmake_minimum_required(VERSION 2.8)

SET(PROJECTNAME PolygonMather)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


project(PolygonMather)


find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core)


include_directories( headers )
include_directories( src )
include_directories( visualization/headers )
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable( ${PROJECTNAME} src/main.cpp src/algorithms.cpp src/app_viewer.cpp)
qt5_use_modules( ${PROJECTNAME} Widgets)
qt5_use_modules( ${PROJECTNAME} OpenGL)

target_link_libraries( ${PROJECTNAME} ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${QT_LIBRARIES} )
target_link_libraries( ${PROJECTNAME} ${CMAKE_CURRENT_SOURCE_DIR}/visualization/libvisualization.a)


