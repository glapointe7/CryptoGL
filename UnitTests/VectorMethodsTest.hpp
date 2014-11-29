
#ifndef VECTORMETHODSTEST_HPP
#define	VECTORMETHODSTEST_HPP

#include "Test.hpp"
#include "../src/Vector.hpp"

namespace UnitTests
{
    class VectorMethodsTest : public Test
    {
    protected:
       UInt16Vector *V;
       BytesVector *U;

       void setUp() override
       {
          V = new UInt16Vector({1, 2, 44, 7465, 0, 888, 12345});
          U = new BytesVector({1, 2, 3, 4, 5, 6, 23});
       }

       void tearDown() override
       {
          delete V;
          delete U;
       }
    };

    TEST(VectorJoin, VectorMethodsTest) 
    {
        compare("1-2-44-7465-0-888-12345", V->join('-'));
    }
    
    TEST(VectorRotateLeft8, VectorMethodsTest) 
    {
        compare({2, 3, 4, 5, 6, 23, 1}, U->rotateLeft(8));
    }
}

#endif