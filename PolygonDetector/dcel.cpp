#include "dcel.h"
#include <vector>
#include <stdexcept>
#include <math.h>
#include <iostream>

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

DCEL::Edge* DCEL::add_segment(const point_type& u, const point_type& v) {
    Edge* ev = new Edge(Vertex(v));
    Edge* eu = new Edge(Vertex(u));

    ev->twin_ = eu;
    eu->twin_ = ev;
    edges.push_back(ev);
    edges.push_back(eu);
    insert_new_edge(ev);
    insert_new_edge(eu);
    return ev;
}

void DCEL::insert_new_edge(Edge* edge) {
    const Vertex& from = edge->from();
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
            auto angleAfter = angle2pi(e->right_next());
            if (angleBefore > eangle && angleAfter < eangle) {
                prevEdge = e;
                break;
            }
        }
    }

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
        throw logic_error("can't find prev edge!");
    }

    edge->right_next(prevEdge->right_next());
    prevEdge->right_next(edge);

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
