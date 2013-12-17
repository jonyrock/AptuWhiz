#include "dcel.h"
#include "geom_algorithms.h"
#include <vector>
#include <stdexcept>
#include <math.h>
#include <iostream>

using namespace geom::algorithms;
using namespace std;

double angle2pi(const DCEL::Edge* e) {
    double x = e->to().point.x - e->from().point.x;
    double y = e->to().point.y - e->from().point.y;
    double piAngle = atan2(y, x);
    if (piAngle < 0) {
        piAngle += 2 * M_PI;
    }
    return piAngle;
}

bool between(const DCEL::Edge* e, const DCEL::Edge* ec) {
    double aa = angle2pi(e);
    double ab = angle2pi(e->right_next());
    double a = angle2pi(ec);
    return ab < a && a < aa;
}

void DCEL::deleteEdge(Edge* edge) {
    
    if (edge->get_segment() == segment_type(point_type(287, 196), point_type(252, 196))) {
        int k = 10;
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
    //    assert(prevEdge != NULL);
    if (prevEdge == NULL) {
        cout << "prev edge null: " << *edge << endl;
        cout << "vvvv EDGES vvvv" << endl;
        Edge* prevEdge = NULL;
        for (auto e : edges) {
            cout << "   ->" << *e << endl;
        }
        cout << "---------------" << endl;
        int kkk = 10;
    }
    prevEdge->right_next(edge->right_next());
    if (edges.size() == 1) {
        vertexEdge[from] = NULL;
        return;
    }
    if (vertexEdge[from] == edge) {
        vertexEdge[from] = edge->right_next();
    }
    this->edges.erase(edge);
    delete edge;
}

void DCEL::deleteEdgeWithTwin(Edge* it) {
    auto twin = it->twin();
    deleteEdge(it);
    deleteEdge(twin);
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
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        auto se = (**it).get_segment();
        //        if(false){
        if (segments_inner_intersected(se, newSegment)) {
            auto ip = segments_intesection(se, newSegment);

            if ((**it).get_segment() == segment_type(point_type(91, 210), point_type(91, 182))) {
                int k = 10;
            }
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
    
    

    // check only right 
    bool insertSucc = insert_new_edge(ev);
    if (!insertSucc) {
        delete ev;
        delete eu;
        return;
    }
    insert_new_edge(eu);

    
    if ((*eu).get_segment() == segment_type(point_type(91, 210), point_type(91, 182))) {
        int k = 10;
    }

    edges.insert(ev);
    edges.insert(eu);

}

bool DCEL::insert_new_edge(Edge* edge) {
    const Vertex& from = edge->from();
    if (from.point == point_type(91, 210)) {
        int kkkkk = 10;
    }
    if (vertexEdge.find(from) == vertexEdge.end() || vertexEdge[from] == NULL) {
        vertexEdge[from] = edge;
        edge->right_next(edge);
        return true;
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
            auto angleAfter = angle2pi(e->right_next());
            if (angleBefore == eangle) {
                auto len1 = segment_length(edge->get_segment());
                auto len2 = segment_length(e->get_segment());
                if (edge->to().point == e->to().point) {
                    return false;
                }
                //                cout << "add split segment from" << edge->from().point << " to split "  << edge->to().point << e->to().point << endl;
                if (len2 > len1) {
                    deleteEdge(e);
                    add_segment(from.point, edge->to().point);
                }
                add_segment(edge->to().point, e->to().point);
                return false;
            }
            if (angleBefore > eangle && angleAfter < eangle) {
                prevEdge = e;
                break;
            }
        }
    }

    assert(prevEdge != NULL);
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
    }

    edge->right_next(prevEdge->right_next());
    prevEdge->right_next(edge);
    return true;

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

DCEL::~DCEL() {
    for (auto ep : edges) {
        delete ep;
    }
}
