#ifndef EDGEVERTEX_HPP
#define	EDGEVERTEX_HPP

#include "Types.hpp"
#include <limits>

using namespace CryptoGL;

namespace DataStructure
{   
    class Edge;
    
    class Vertex
    {
    private:
        uint64_t ID;
        uint64_t degree_in;
        uint64_t degree_out;
        double value;
        bool is_explored;
        uint64_t distance;
        Vector<Edge> Neighbors;
        uint64_t leader;
        bool is_used;
                
    public:
        explicit Vertex(const uint64_t &ID) 
            : ID(ID), is_explored(false), distance(std::numeric_limits<uint32_t>::max()),
              degree_in(0), degree_out(0), leader(0), is_used(false) {}
        Vertex() : Vertex(0) {}
        ~Vertex() {}
        
        void appendNeighbor(const Edge &E);
        
        Vector<Edge> getNeighbors() const { return Neighbors; }
        
        bool isExplored() const { return is_explored; }
        void setExplored(const bool is_explored) { this->is_explored = is_explored; }
        
        // true if used by a stack or a queue, false otherwise.
        bool isUsed() const { return is_used; }
        void setUsed(const bool is_used) { this->is_used = is_used; }
        
        double getDistance() const { return distance; }
        void setDistance(const double &distance) { this->distance = distance; }
        
        // Smallest value named 'Leader' of a vertex for a cycle.
        uint64_t getLeader() const { return leader; }
        void setLeader(const uint64_t &leader) { this->leader = leader; }
        
        uint64_t getID() const { return ID; }
        void setID(const uint64_t &ID) { this->ID = ID; }
        
        friend bool operator == (const Vertex &V, const Vertex &W)
        {
            return V.getDistance() == W.getDistance();
        }
        
        friend bool operator != (const Vertex &V, const Vertex &W)
        {
            return V.getDistance() != W.getDistance();
        }
        
        friend bool operator >= (const Vertex &V, const Vertex &W)
        {
            return V.getDistance() >= W.getDistance();
        }

        friend bool operator <= (const Vertex &V, const Vertex &W)
        {
            return V.getDistance() <= W.getDistance();
        }

        friend bool operator > (const Vertex &V, const Vertex &W)
        {
            return V.getDistance() > W.getDistance();
        }

        friend bool operator < (const Vertex &V, const Vertex &W)
        {
            return V.getDistance() < W.getDistance();
        }
    };
    
    
    class Edge
    {
    private:
        Vertex Head;
        Vertex Tail;
        uint64_t weight;
        
    public:
        Edge(const Vertex &Head, const Vertex &Tail, const uint64_t &weight)
            : Head(Head), Tail(Tail), weight(weight) {}
        Edge(const Vertex &Head, const Vertex &Tail) : Edge(Head, Tail, 0) {}
        Edge() {}
        ~Edge() {}
        
        bool isLoop() const;
        
        Vertex getTail() const { return Tail; }
        Vertex getHead() const { return Head; }
        uint64_t getWeight() const { return weight; }
    };
}

#endif