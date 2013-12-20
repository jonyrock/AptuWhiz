#pragma once

#include "point_type.h"
#include "segment_type.h"
#include "polygon_type.h"

#include <vector>
#include <map>
#include <set>
#include <stddef.h>
#include "io.h"
#include "stdexcept"


using namespace std;

class DCEL {
public:

    struct Vertex {
        point_type point;

        Vertex(point_type point_) : point(point_) {
        }
    };

    class Edge {
        friend DCEL;
    private:
        Edge* next_;
        Edge* twin_;
        const Vertex to_;
        bool visited_;

        Edge(const Vertex& v) : to_(v), isDeleted(false) {
            next_ = NULL;
            visited_ = false;
        }
        bool isDeleted;
    public:

        inline Edge* next() const {
            return this->next_;
        }

        inline Edge* twin() const {
            return this->twin_;
        }

        inline Edge* right_next() const {
            return this->twin()->next();
        }

        inline Edge* right_next(Edge* e) const {
            return this->twin()->next_ = e;
        }

        inline Vertex to() const {
            return this->to_;
        }

        inline Vertex from() const {
            return this->twin()->to();
        }

        inline segment_type get_segment() const {
            return segment_type(from().point, to().point);
        }

        friend std::ostream & operator <<(std::ostream & out, DCEL::Edge const & e) {
            out << e.from().point << " -> " << e.to().point;
            return out;
        }

        bool is_leaf() {
            return next() == twin();
        }
    };

    typedef set<Edge*> edgeList;
    void add_segment(const point_type& u, const point_type& v);
    vector<DCEL::Edge*> get_all_edges(const Vertex v) const;
    vector<polygon_type> get_all_facets(const vector<DCEL::Edge*>&) const;
    void deleteEdge(Edge* it);
    void deleteEdgeWithTwin(Edge* e);

    void delete_leafs();
    
    void merge_near();

    polygon_type find_center_polygon();

    ~DCEL();

    edgeList edges;
    int width, height;

    DCEL(int step_, int width_, int height_) : step(step_),
    width(width_), height(height_) {
        if (step_ <= 0)
            throw invalid_argument("Step can't be negative or equal zero.");
    }

private:
    int step;
    // map from vertex to first edges
    map<const Vertex, Edge*> vertexEdge;
    void insert_new_edge(Edge*);
    polygon_type walk(Edge*) const;

};

inline bool operator<(DCEL::Vertex const & a, DCEL::Vertex const & b) {
    return a.point < b.point;
}

inline bool operator==(DCEL::Vertex const & a, DCEL::Vertex const & b) {
    return a.point < b.point;
}