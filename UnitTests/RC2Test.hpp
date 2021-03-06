
#ifndef RC2TEST_HPP
#define RC2TEST_HPP

#include "Test.hpp"
#include "../src/RC2.hpp"

namespace UnitTests
{
    class RC2Test : public Test
    {
    protected:
       RC2 *R, *C;

       void setUp() override
       {
          R = new RC2({0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
          C = new RC2({0x88, 0xbc, 0xa9, 0x0e, 0x90, 0x87, 0x5a, 0x7f, 0x0f, 0x79, 0xc3, 0x84, 0x62, 0x7b, 0xaf, 0xb2});
       }

       void tearDown() override
       {
          delete R;
          delete C;
       }
    };

    TEST(RC2EncodeWith8BytesKey, RC2Test)
    {   
       const BytesVector clear_text = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
       compare("30649EDF9BE7D2C2", R->encode(clear_text));
    }

    TEST(RC2DecodeWith8BytesKey, RC2Test)
    {
       compare("1000000000000001", R->decode(StringTest("30649EDF9BE7D2C2").hexToBytes()));
    }

    TEST(RC2EncodeWith16BytesKey, RC2Test)
    {   
       compare("2269552AB0F85CA6", C->encode(BytesVector(8, 0)));
    }

    TEST(RC2DecodeWith16BytesKey, RC2Test)
    {
       compare("0000000000000000", C->decode(StringTest("2269552AB0F85CA6").hexToBytes()));
    }
}

#endif