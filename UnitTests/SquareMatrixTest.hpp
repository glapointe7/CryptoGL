#ifndef SQUAREMATRIXTEST_HPP
#define	SQUAREMATRIXTEST_HPP

#include "Test.hpp"
#include "../src/SquareMatrix.hpp"

namespace UnitTests
{
    class SquareMatrixTest : public Test
    {
    protected:
       SquareMatrix *A, *B, *C;

       void setUp() override
       {
          A = new SquareMatrix({{1, 3, 1}, {1, 1, 0}, {2, 9, 3}}, 26);
          B = new SquareMatrix({{3, 4}, {5, 9}}, 26);
          C = new SquareMatrix({{9, 4}, {5, 7}}, 26);
       }

       void tearDown() override
       {
          delete A;
          delete B;
          delete C;
       }
    };

    TEST(Determinant3X3Mod26, SquareMatrixTest) 
    {
        compare(1, A->det());
    }

    TEST(Determinant2X2Mod26, SquareMatrixTest) 
    {
        compare(7, B->det());
    }
    
    TEST(InverseSimple2X2Mod26, SquareMatrixTest) 
    {
        C->inverse();
        compare({{5, 12}, {15, 25}}, C->getMatrix());
    }
    
    TEST(InverseSimple3X3Mod26, SquareMatrixTest) 
    {
        A->inverse();
        compare({{3, 0, 25}, {23, 1, 1}, {7, 23, 24}}, A->getMatrix());
    }
}

#endif