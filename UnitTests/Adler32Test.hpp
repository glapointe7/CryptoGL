
#ifndef ADLER32TEST_HPP
#define	ADLER32TEST_HPP

#include "Test.hpp"

namespace UnitTests
{
    class Adler32Test : public Test
    {
    protected:
        void setUp() override { A = new Adler32(); }
        virtual void run() = 0;
        void tearDown() override { delete A; }
        
        Adler32 *A;
    };
    
    
    class Adler32NormalTest : public Adler32Test
    {
    public:
        Adler32NormalTest() : Test("Adler32NormalTest") {}
        
        void run() override
        {
            const String message = "message digest";
            compare(0x29750586, A->encode(message.toBytes()));
        }
    };
    
    class Adler32EmptyTest : public Adler32Test
    {
    public:
        Adler32EmptyTest() : Test("Adler32EmptyTest") {}
        
        void run() override
        {
            compare(0x00000001, A->encode({}));
        }
    };
}

#endif