#include "dcel.h"
#include "geom_algorithms.h"
#include "io.h"
#include <vector>
#include <stack>
#include <stdexcept>
#include <math.h>
#include <iostream>


//#define DEBUG

using namespace geom::algorithms;
using namespace std;

double angle2pi(segment_type seg) {
    double x = seg[1].x - seg[0].x;
    double y = seg[1].y - seg[0].y;
    double piAngle = atan2(y, x);
    if (piAngle < 0) {
        piAngle += 2 * M_PI;
    }
    return piAngle;
}

double angle2pi_back(segment_type seg) {
    return angle2pi(segment_type(seg[1], seg[0]));
}

double angle2pi(const DCEL::Edge* e) {
    return angle2pi(e->get_segment());
}

bool between(const DCEL::Edge* e, const DCEL::Edge* ec) {
    double aa = angle2pi(e);
    double ab = angle2pi(e->right_next());
    double a = angle2pi(ec);
    return ab < a && a < aa;
}

void DCEL::deleteEdge(Edge* edge) {



    if (edge->get_segment() == segment_type(point_type(0, 203), point_type(42, 175))) {
        int hh = 10;
    }

    Vertex from = edge->from();
    auto edges = get_all_edges(from);

    Edge* prevEdge = NULL;
    for (auto e : edges) {
        if (e->right_next() == edge) {
            prevEdge = e;
            break;
        }
    }
#ifndef DEBUG
    assert(prevEdge != NULL);
#else
    if (prevEdge == NULL) {
        cout << "prev edge null: " << *edge << endl;
        cout << "vvvv EDGES vvvv" << endl;
        Edge* prevEdge = NULL;
        //        for (auto e : edges) {
        //            cout << "   ->" << *e << endl;
        //        }
        //        cout << "---------------" << endl;
        int kkk = 10;
        assert(false);
    }
#endif
    prevEdge->right_next(edge->right_next());
    if (edges.size() == 1) {
        vertexEdge[from] = NULL;
        return;
    }
    if (vertexEdge[from] == edge) {
        vertexEdge[from] = edge->right_next();
    }

}

void DCEL::deleteEdgeWithTwin(Edge* e) {
#ifdef DEBUG
    cout << "delete edge " << *e << endl;
#endif

    auto twin = e->twin();
    deleteEdge(e);
    deleteEdge(twin);
    size_t sss = edges.size();
    edges.erase(e);
    size_t sss1 = edges.size();
    edges.erase(twin);
    size_t sss2 = edges.size();
    //    for(auto e: edges){
    //        cerr  << e << " ";
    //    }
    //    cerr << endl;
#ifdef DEBUG
    cout << "clr edges" << endl;
#endif
}

void DCEL::add_segment(const point_type& u_, const point_type& v_) {


    point_type u(u_);
    point_type v(v_);

    u.x -= u.x % step;
    u.y -= u.y % step;

    v.x -= v.x % step;
    v.y -= v.y % step;

    if (u == v)
        return;

    //    cout << "add segment " << u << " " << v << endl;
    segment_type newSegment(u, v);



#ifdef DEBUG
    cout << "add new segment " << newSegment << endl;

    if (newSegment == segment_type(point_type(7, 35), point_type(7, 70))) {
        int kk = 10;
    }
#endif

#ifdef DEBUG
    //    cout << "------------" << newSegment << endl;
    //    for (auto e : edges) {
    //        cout << *e << endl;
    //    }
    //    cout << "xxxxxxxx" << endl;
#endif
    auto newSegAngle = angle2pi(newSegment);
    auto newSegAngleb = angle2pi_back(newSegment);
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        auto se = (**it).get_segment();
        if (se == newSegment) {
            return;
        }
        bool forwardCollision = se[0] == newSegment[0] && newSegAngle == angle2pi(se);
        bool backwardCollision = se[0] == newSegment[1] && newSegAngleb == angle2pi(se);
        if (forwardCollision || backwardCollision) {
            double lenNew = segment_length(newSegment);
            double lenOld = segment_length(se);

            if (lenOld > lenNew) {
#ifdef DEBUG
                cout << "del call " << newSegment << endl;
#endif
                deleteEdgeWithTwin(*it);
#ifdef DEBUG
                cout << "inner add 1 " << newSegment << endl;
#endif
                if (forwardCollision)
                    add_segment(se[1], newSegment[1]);
                if (backwardCollision)
                    add_segment(se[1], newSegment[0]);
#ifdef DEBUG
                cout << "inner add 2 " << newSegment << endl;
#endif
                add_segment(u_, v_);
                return;
            } else {
#ifdef DEBUG
                cout << "inner add 3 " << newSegment << endl;
#endif
                if (forwardCollision)
                    add_segment(se[1], newSegment[1]);
                else
                    add_segment(se[1], newSegment[0]);
                return;
            }
        }
        if (segments_inner_intersected(se, newSegment)) {
            auto ip = segments_intesection(se, newSegment);
            deleteEdgeWithTwin(*it);
            add_segment(u, ip);
            add_segment(ip, v);
            add_segment(se[0], ip);
            add_segment(se[1], ip);
            return;
        }


    }


    Edge* ev = new Edge(Vertex(v));
    Edge* eu = new Edge(Vertex(u));

    ev->twin_ = eu;
    eu->twin_ = ev;

    insert_new_edge(ev);
    insert_new_edge(eu);
#ifdef DEBUG
    cout << "OK" << newSegment << endl;
#endif

    edges.insert(ev);
    edges.insert(eu);

}

void DCEL::insert_new_edge(Edge* edge) {

    const Vertex& from = edge->from();

#ifdef DEBUG
    cout << "insert" << *edge << endl;
    if (from.point == point_type(12, 72)) {
        int kkkk = 10;
    }
#endif

    if (vertexEdge.find(from) == vertexEdge.end() || vertexEdge[from] == NULL) {
        vertexEdge[from] = edge;
        edge->right_next(edge);
        return;
    }

    auto edges = get_all_edges(from);
    double minAngle = 2 * M_PI;
    double maxAngle = 0;
    Edge* minEdge = NULL;
    for (auto e : edges) {
        double myAngle = angle2pi(e);
        if (myAngle < minAngle) {
            minAngle = myAngle;
            minEdge = e;
        }
    }
    maxAngle = angle2pi(minEdge->right_next());

    double eangle = angle2pi(edge);
    Edge* prevEdge = NULL;
    if (eangle > maxAngle || eangle < minAngle) {
        prevEdge = minEdge;
    } else {
        for (auto e : edges) {
            auto angleBefore = angle2pi(e);

            assert(angleBefore != eangle);
            //            if(angleBefore == eangle){
            //                
            //            }

            auto angleAfter = angle2pi(e->right_next());
            if (angleBefore == eangle) {
                auto len1 = segment_length(edge->get_segment());
                auto len2 = segment_length(e->get_segment());
                if (edge->to().point == e->to().point)
                    return;
                //                cout << "add split segment from" << edge->from().point << " to split "  << edge->to().point << e->to().point << endl;
                if (len2 > len1) {
                    deleteEdge(e);
                    add_segment(from.point, edge->to().point);
                }
                add_segment(edge->to().point, e->to().point);
                return;
            }
            if (angleBefore > eangle && angleAfter < eangle) {
                prevEdge = e;
                break;
            }
        }
    }

#ifndef DEBUG
    assert(prevEdge != NULL);
#else
    if (prevEdge == NULL) {
        cout << "-----------" << endl;
        for (auto e : edges) {
            cout << *e << ", ";
            cout << angle2pi(e) << " " << endl;
        }
        cout << "vvvvvvvvvvvvv" << endl;
        cout << *edge << ", ";
        cout << eangle << endl;
        cout << "***********" << endl;
        assert(false);
    }
#endif
    edge->right_next(prevEdge->right_next());
    prevEdge->right_next(edge);
    return;

}

vector<DCEL::Edge*> DCEL::get_all_edges(const Vertex v) const {
    vector<DCEL::Edge*> res;
    Edge* firstEdge = vertexEdge.find(v)->second;
    if (firstEdge == NULL)
        return res;
    Edge* curEdge = firstEdge;
    while (true) {
        res.push_back(curEdge);
        curEdge = curEdge->right_next();
        if (curEdge == firstEdge)
            break;
    }
    return res;
}

polygon_type DCEL::walk(Edge* e) const {
    polygon_type res;
    while (!e->visited_) {
        e->visited_ = true;
        res.push_back(e->to().point);
        e = e->next();
    }
    return res;
}

vector<polygon_type> DCEL::get_all_facets(
        const vector<DCEL::Edge*>& knownBorderEdges) const {
    vector<polygon_type> res;

    for (auto e : edges) {
        e->visited_ = false;
    }

    for (auto e : knownBorderEdges) {
        if (e->visited_)
            continue;
        res.push_back(walk(e));
    }

    return res;
}

void DCEL::delete_leafs() {

//    stack<Edge*> leafs;
//
//    for (auto e : edges) {
//        if (e->is_leaf()) {
//            leafs.push(e);
//        }
//    }
//    
//    cout << "leafs size: " << leafs.size() << endl;
//
//    while (!leafs.empty()) {
//        Edge* e = leafs.top();
//        leafs.pop();
//        while(true) {
//            break;
//        }
//    }
    
    
    while(true){
        bool foundLeaf = false;
        for(auto e: edges){
            if(e->is_leaf()){
                foundLeaf = true;
                deleteEdgeWithTwin(e);
                break;
            }
        }
        if(!foundLeaf)
            break;
    }



}

DCEL::~DCEL() {
    for (auto ep : edges) {
        delete ep;
    }
}