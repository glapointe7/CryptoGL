#include "EdgeVertex.hpp"

using namespace DataStructure;

void Vertex::appendNeighbor(const Edge &E)
{
    Neighbors.push_back(E);
}

void Vertex::reserveNeighbors(const uint64_t &n)
{
    Neighbors.reserve(n);
}


bool Edge::isLoop() const
{
    return Head.getID() == Tail.getID();
}