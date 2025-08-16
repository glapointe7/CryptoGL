#pragma once

#include "Test.hpp"
#include "../../src/core/types/Vector.hpp"

namespace UnitTests
{
    class VectorMethodsTest : public Test
    {
    protected:
       UInt16Vector *V;
       BytesVector *U;
       //UInt32Vector *W, *X, *Y;

       void setUp() override
       {
            V = new UInt16Vector({1, 2, 44, 7465, 0, 888, 12345});
            U = new BytesVector({1, 2, 3, 4, 5, 6, 23});
            // W = new UInt32Vector({
            //     0x00000001, 0x00000002, 0x00000004, 0x00000008, 
            //     0x00000010, 0x00000020, 0x00000040, 0x00000080
            // });

            // X = new UInt32Vector({
            //     0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
            //     0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
            // });

            // Y = new UInt32Vector({
            //     0x01234567, 0x89ABCDEF, 0x0F1F2F3F, 0x4F5F6F7F,
            //     0x80808080, 0x7F7F7F7F, 0xAAAAAAAA, 0x55555555
            // });
       }

       void tearDown() override
       {
          delete V;
          delete U;
          //delete W;
          //delete X;
          //delete Y;
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

    /*TEST(VectorTranspose8X32Bits, VectorMethodsTest) 
    {
        compare({0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080}, W->transposeBits());
    }

    TEST(VectorUntranspose8X32BitsIdentity, VectorMethodsTest) 
    {
        compare({0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080}, W->untransposeBits());
    }

    TEST(VectorTranspose8X32BitsAllOnes, VectorMethodsTest) 
    {
        compare({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, X->transposeBits());
    }

    TEST(VectorUntranspose8X32BitsAllOnes, VectorMethodsTest) 
    {
        compare({0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, X->untransposeBits());
    }

    TEST(VectorUntranspose8X32BitsAsymetric, VectorMethodsTest) 
    {
        compare({
            0x12A5AFD5,  // mot 0 : bits n°0 de tous les mots d'entrée
            0x3482A0A8,  // mot 1 : bits n°1
            0xCD71252C,  // mot 2 : bits n°2
            0x5D14A492,  // mot 3 : bits n°3
            0x23436B6A,  // mot 4 : bits n°4
            0xA8222282,  // mot 5 : bits n°5
            0xB80B8B8A,  // mot 6 : bits n°6
            0xF06E6A6A   // mot 7 : bits n°7
            }, Y->transposeBits());
        }
    }*/
}