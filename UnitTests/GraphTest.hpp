#ifndef GRAPHTEST_HPP
#define	GRAPHTEST_HPP

#include "Test.hpp"
#include "../src/Graph.hpp"

using namespace DataStructure;

namespace UnitTests
{
    class GraphTest : public Test
    {
    protected:
       Graph *Dijkstra7, *Dijkstra4, *Dijkstra15, *Dijkstra50;

       void setUp() override
       {
          Dijkstra4 = new Graph();
          Dijkstra7 = new Graph();
          Dijkstra15 = new Graph();
          Dijkstra50 = new Graph();
       }

       void tearDown() override
       {
          delete Dijkstra4;
          delete Dijkstra7;
          delete Dijkstra15;
          delete Dijkstra50;
       }
    };

        
//    TEST(KargerMinimalCutsOnSample200, GraphTest) 
//    {
//        Sample200->load("/media/gabriel/Data/Algorithms Analysis/kargerMinCut.txt");
//        compare(17, Sample200->getMinimumCut());
//    }
    
//    TEST(KargerMinimalCutsOnSample8, GraphTest) 
//    {
//        Karger8->load("/home/gabriel/Documents/CryptoGL_DEV/UnitTests/karger8.txt");
//        compare(2, Karger8->getMinimumCut());
//    }
    
    // Dijkstra4 => shortest path length = 2 starts = 1 ; end = 4
    TEST(DijkstraShortestPathOnSample4, GraphTest) 
    {
        Dijkstra4->loadWithWeight("/home/gabriel/Documents/CryptoGL_DEV/UnitTests/Dijkstra4Easy.txt");
        //const Vector<Vertex> ShortestPath = Dijkstra4->getShortestPath();
        compare(2, Dijkstra4->getShortestPath(1, 4));
    }
    
    // Dijkstra7 => shortest path length = 5 starts = 1 ; end = 7
    TEST(DijkstraShortestPathOnSample7, GraphTest) 
    {
        Dijkstra7->loadWithWeight("/home/gabriel/Documents/CryptoGL_DEV/UnitTests/Dijkstra7Normal.txt");
        //const Vector<Vertex> ShortestPath = Dijkstra4->getShortestPath();
        compare(5, Dijkstra7->getShortestPath(1, 7));
    }
    
    // Dijkstra15 => shortest path length = 26 starts = 13 ; end = 5
    TEST(DijkstraShortestPathOnSample15, GraphTest) 
    {
        Dijkstra15->loadWithWeight("/home/gabriel/Documents/CryptoGL_DEV/UnitTests/Dijkstra15Medium.txt");
        //const Vector<Vertex> ShortestPath = Dijkstra4->getShortestPath();
        compare(26, Dijkstra15->getShortestPath(14, 6));
    }
    
    // Dijkstra50 => shortest path length = 9 starts = 28 ; end = 6
    TEST(DijkstraShortestPathOnSample50, GraphTest) 
    {
        Dijkstra50->loadWithWeight("/home/gabriel/Documents/CryptoGL_DEV/UnitTests/Dijkstra50Large.txt");
        //const Vector<Vertex> ShortestPath = Dijkstra4->getShortestPath();
        compare(9, Dijkstra50->getShortestPath(29, 7));
    }
    
    // 2599,2610,2947,2052,2367,2399,2029,2442,2505,3068
    // Dijkstra length for vertices 7,37,59,82,99,115,133,165,188,197
    
    // 10,30,50,80,90,110,130,160,180,190
    // Give: 3205,2303,3152,982,2018,2317,1820,2403,3027,2596
}

#endif