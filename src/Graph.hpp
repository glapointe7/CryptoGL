#ifndef GRAPH_HPP
#define	GRAPH_HPP

#include "Types.hpp"
#include "EdgeVertex.hpp"


using namespace CryptoGL;

namespace DataStructure
{          
    class Graph
    {
    private:
        Vector<Vertex> Vertices;
        Vector<Edge> Edges;
        Vertex Source;
        //Vector<Vector<Vertex>> Cycles;
        
        void detectCycle(Vertex Parents);
        
    public:
        explicit Graph(const Vertex &Source) : Source(Source) {}
        Graph() {}
        virtual ~Graph() {}
        
        /* Load and build a graph from an adjacency list given in the file. */
        void load(const String &Filename);
        void loadWithWeight(const String &filename);
        
        /* Get the shortest path distance of the current graph. 
           Dijkstra algorithm using binary Heap. */
        uint64_t getShortestPath();
        
        uint64_t getMinimalCuts() const;
        
        bool isConnexe();
    };
}

#endif