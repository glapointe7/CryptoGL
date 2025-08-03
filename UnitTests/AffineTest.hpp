#pragma once

#include "Test.hpp"
#include "../src/Affine.hpp"

namespace UnitTests
{
    class AffineTest : public Test
    {
    protected:
       Affine *A;
       static const StringTest clear_text;
       static const StringTest cipher_text;

       void setUp() override
       {
          A = new Affine(17, 7);
       }

       void tearDown() override
       {
          delete A;
       }
    };
    
    const StringTest AffineTest::clear_text = "DIESISTEINEGEHEIMENACHRICHT";
    const StringTest AffineTest::cipher_text = "GNXBNBSXNUXFXWXNDXUHPWKNPWS";

    TEST(AffineEncode, AffineTest) 
    {
        compare(cipher_text, A->encode(clear_text));
    }

    TEST(AffineDecode, AffineTest) 
    {
        compare(clear_text, A->decode(cipher_text));
    }
}