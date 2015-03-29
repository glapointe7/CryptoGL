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
    const uint8_t starting_id = InputFile.get() - 48;
    InputFile.seekg(std::ios::beg);
    String Line;
    while (std::getline(InputFile, Line)) 
    {
        std::istringstream InputString(Line); 
        uint64_t head, tail;
        InputString >> head;
        const Vertex Head(head - starting_id);
        Vertices.push_back(Head);
        const uint64_t line_length = Line.length() - 1;
        Vertices.back().reserveNeighbors(line_length);
        Edges.reserve(line_length);
        
        while(InputString >> tail)
        {
            char comma;
            uint64_t length;
            InputString >> comma;
            InputString >> length;
            const Vertex Tail(tail - starting_id);
            const Edge E(Head, Tail, length);
            Vertices.back().appendNeighbor(E);
            Edges.push_back(E);
        }
    }
}

uint64_t Graph::getShortestPath(const uint64_t &start, const uint64_t &end)
{
    // Initialize and build a binary heap with vertices.
    Vertices[start - 1].setDistance(0);
    Heap<Vertex> Q(Vertices);

    // Apply main loop of Dijkstra algorithm.
    while(!Q.isEmpty())
    {
        const uint64_t MinimumVertex = Q.removeMinimumValue().getID();
        const Vector<Edge> Neighbors = Vertices[MinimumVertex].getNeighbors();
        const uint64_t minimum_vertex_distance = Vertices[MinimumVertex].getDistance();
        const uint64_t neighbors_size = Neighbors.size();
 
        for(uint64_t i = 0; i < neighbors_size; ++i)
        {
            const uint64_t distance = minimum_vertex_distance + Neighbors[i].getWeight();
            const Vertex Neighbor = Neighbors[i].getTail();
            if(distance < Vertices[Neighbor.getID()].getDistance())
            {
                Vertices[Neighbor.getID()].setDistance(distance);
                Q.push(Neighbor);
            }
        }
    }
    
    return Vertices[end - 1].getDistance();
} 