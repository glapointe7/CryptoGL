#include "Graph.hpp"

#include "Heap.hpp"

#include <fstream>
#include <sstream>


using namespace DataStructure;

void Graph::load(const String &Filename)
{
    std::ifstream InputFile(Filename.c_str());
    String Line;
    while (std::getline(InputFile, Line)) 
    {
        std::istringstream InputString(Line); 
        uint32_t head, tail;
        InputString >> head;
        const Vertex Head(head);
        Vertices.push_back(Head);
        
        // Build the connected edges to the head vertex.
        while(InputString >> tail)
        {
            const Vertex Tail(tail);
            const Edge E(Head, Tail);
            Vertices[head - 1].appendNeighbor(E);
            Edges.push_back(E);
        } 
    }
}

void Graph::loadWithWeight(const String &filename)
{
    std::ifstream InputFile(filename.c_str());
    String Line;
    while (std::getline(InputFile, Line)) 
    {
        // Convert String line to a vector of integers.
        std::istringstream InputString(Line); 
        uint64_t head, tail, length;
        char comma;
        InputString >> head;
        const Vertex Head(head);
        Vertices.push_back(Head);
        
        while(InputString >> tail)
        {
            InputString >> comma;
            InputString >> length;
            const Vertex Tail(tail);
            const Edge E(Head, Tail, length);
            Vertices[head - 1].appendNeighbor(E);
            Edges.push_back(E);
        }
    }
}

uint64_t Graph::getShortestPath()
{
    // Initialize and build a binary heap with vertices.
    Vertices[0].setDistance(0);
    Heap<Vertex> Q(Vertices);
    Vector<uint64_t> Previous(4, std::numeric_limits<uint32_t>::max());

    // Apply main loop of Dijkstra algorithm.
    while(!Q.isEmpty())
    {
        const Vertex MinimumVertex = Q.removeMinimumValue();
        const Vector<Edge> Neighbors = MinimumVertex.getNeighbors();
        for(const Edge E : Neighbors)
        {
            const uint64_t distance = MinimumVertex.getDistance() + E.getWeight();
            Vertex Neighbor = E.getTail();
            if(distance < Neighbor.getDistance())
            {
                Neighbor.setDistance(distance);
                Previous[Neighbor.getID() - 1] = MinimumVertex.getDistance();
                Q.push(Neighbor);
            }
        }
    }
    
    uint64_t total_distance = 0;
    for(const uint64_t &distance : Previous)
    {
        if(distance < std::numeric_limits<uint32_t>::max())
            total_distance += distance;
    }
    
    return total_distance;
} 