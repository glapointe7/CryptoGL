#ifndef GRAPH_HPP
#define	GRAPH_HPP

#include "Types.hpp"

namespace CryptoGL
{
    class Vertex
    {
    private:
        uint32_t ID;

    public:
        Vertex() : ID(0) {}
        explicit Vertex(const uint32_t ID) : ID(ID) {}
        ~Vertex() {}

        uint32_t getID() const { return ID; }
        void setID(const uint32_t ID) { this->ID = ID; }
    };
    
    
    class Edge
    {
    private:
        Vertex U, V;
        double weight;

        /* 0 = right ; 1 = left */
        bool direction;

    public:
        Edge(const Vertex &U, const Vertex &V, const double &weight)
            : U(U), V(V), weight(weight) {}
        Edge(const Vertex &U, const Vertex &V)
            : Edge(U, V, 0) {}
        Edge() {}
        ~Edge() {}

        bool isLoop() const;
        
        Vertex getLeft() const { return U; }
        Vertex getRight() const { return V; }
        
        double getWeight() const { return weight; }
        void setWeight(const double &new_weight) { this->weight = new_weight; }
    };
    
    
    class Graph 
    {
    private:
        Vector<Vertex> Vertices;
        Vector<Edge> Edges;
        
    public:
        Graph() {}
        virtual ~Graph() {}

        /* Each line of the file must be an adjacency list. */
        void load(const String &filename);

        /* Add a vertex in the current graph. */
        void addVertex(const uint32_t ID);

        /* Add an edge in the current graph. */
        void addEdge(const uint32_t ID_left, const uint32_t ID_right);

        Edge pickRandomEdge() const;
        
        void randomContraction();

        Vector<Vertex> getShortestPath();
        
        // randomContraction has >= P(1/n^2) chance of finding the min cut. Run it a sufficient
        // number of times so that we have only a 1/n chance of failure.
        uint32_t getMinimumCut() const;
        
        Vector<Vertex> getVertices() const { return Vertices; }
        Vector<Edge> getEdges() const { return Edges; }
    };
}

#endif