
#ifndef ADLER32TEST_HPP
#define	ADLER32TEST_HPP

#include "Test.hpp"

#include "../src/Adler32.hpp"

namespace UnitTests
{
    class Adler32Test : public Test
    {
    private:
        Adler32 *A;
        
    public:
        void setUp() override 
        { 
            A = new Adler32(); 
        }
        
        void tearDown() override 
        { 
            delete A; 
        }
        
        void run() override
        {
            runWithEmptyValue();
            runNormalCase();
        }
        
        void runWithEmptyValue()
        {
            setName("Adler32WithEmptyValue");
            compare(0x00000001u, A->encode({}));
        }
        
        void runNormalCase()
        {
            setName("Adler32NormalCase");
            const String message = "message digest";
            compare(0x29750586u, A->encode(message.toBytes()));
        }

    };
}

#endif