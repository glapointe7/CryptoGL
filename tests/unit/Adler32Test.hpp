#pragma once

#include "Test.hpp"

#include "../../src/algorithms/check_sum/adler32/Adler32.hpp"
#include "../../src/core/types/String.hpp"

namespace UnitTests
{
    class Adler32Test : public Test
    {
    protected:
        Adler32 *A;
        
        void setUp() override 
        { 
            A = new Adler32(); 
        }
        
        void tearDown() override 
        { 
            delete A; 
        }
    };
    
    TEST(Adler32WithEmptyValue, Adler32Test)
    {
        compare(0x00000001u, A->encode({}));
    }

    TEST(Adler32NormalCase, Adler32Test)
    {
        const String message = "message digest";
        compare(0x29750586u, A->encode(message.toBytes()));
    }
}