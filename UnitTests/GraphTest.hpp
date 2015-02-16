#ifndef GRAPHTEST_HPP
#define	GRAPHTEST_HPP

#include "Test.hpp"
#include "../src/Graph.hpp"

namespace UnitTests
{
    class GraphTest : public Test
    {
    protected:
       Graph *Sample200, *Sample40;

       void setUp() override
       {
          Sample200 = new Graph();
          Sample40 = new Graph();       
       }

       void tearDown() override
       {
          delete Sample200;
          delete Sample40;
       }
    };

        
    TEST(KargerMinimalCutsOnSample200, GraphTest) 
    {
        Sample200->load("/media/gabriel/Data/Algorithms Analysis/kargerMinCut.txt");
        compare(17, Sample200->getMinimumCut());
    }
}

#endif