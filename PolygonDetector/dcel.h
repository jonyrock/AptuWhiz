#pragma once

#include "point_type.h"
#include "segment_type.h"
#include "polygon_type.h"

#include <vector>
#include <map>
#include <set>
#include <stddef.h>
#include "io.h"


using namespace std;

class DCEL {
public:
        
    struct Vertex {
        point_type point;
        Vertex(point_type point_): point(point_){}
    };
    
    class Edge {
        friend DCEL;
    private:
        Edge* next_;
        Edge* twin_;
        const Vertex to_;
        bool visited_;

        Edge(const Vertex& v) : to_(v) {
            next_ = NULL;
            visited_ = false;
        }
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
    };
    
    typedef set<Edge*> edgeList;
    void add_segment(const point_type& u, const point_type& v);
    vector<DCEL::Edge*> get_all_edges(const Vertex v) const;
    vector<polygon_type> get_all_facets(const vector<DCEL::Edge*>&) const;
    void deleteEdge(Edge* it);
    void deleteEdgeWithTwin(Edge* e);
    
    
    ~DCEL();
    
    edgeList edges;
    
    DCEL(int step_ = 5):step(step_){}
    
private:
    int step;
    // map from vertex to first edge
    map<const Vertex, Edge*> vertexEdge;
    void insert_new_edge(Edge*); // return conflict edge or NULL
    polygon_type walk(Edge*) const;

};

inline bool operator<(DCEL::Vertex const & a, DCEL::Vertex const & b) {
    return a.point < b.point;
}

inline bool operator==(DCEL::Vertex const & a, DCEL::Vertex const & b) {
    return a.point < b.point;
}