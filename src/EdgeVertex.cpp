#include "EdgeVertex.hpp"

using namespace DataStructure;

void Vertex::appendNeighbor(const Edge &E)
{
    Neighbors.push_back(E);
}


bool Edge::isLoop() const
{
    return Head.getID() == Tail.getID();
}