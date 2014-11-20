/*
 * Source : https://www.cosic.esat.kuleuven.be/nessie/testvectors/bc/noekeon/Noekeon-Direct-128-128.verified.test-vectors
 */
#ifndef NOEKEONTEST_HPP
#define NOEKEONTEST_HPP

#include "Test.hpp"
#include "../src/Noekeon.hpp"

namespace UnitTests
{
    class NoekeonTest : public Test
    {
    protected:
       Noekeon *R, *C;

       void setUp() override
       {
          R = new Noekeon({0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});
          C = new Noekeon({0x2B, 0xD6, 0x45, 0x9F, 0x82, 0xC5, 0xB3, 0x00, 0x95, 0x2C, 0x49, 0x10, 0x48, 0x81, 0xFF, 0x48});
       }

       void tearDown() override
       {
          delete R;
          delete C;
       }
    };

    TEST(NoekeonEncodeZeroValues, NoekeonTest)
    {   
       compare("EA6552BA793546C261E4B3E90433F5A2", R->encode(BytesVector(16, 0)));
    }

    TEST(NoekeonDecodeZeroValues, NoekeonTest)
    {
       const StringTest code("EA6552BA793546C261E4B3E90433F5A2");
       compare("00000000000000000000000000000000", R->decode(code.hexToBytes()));
    }

    TEST(NoekeonEncodeNormalCase, NoekeonTest)
    {
       const BytesVector clear_text = {
          0x47, 0x65, 0xF3, 0xDA, 0x10, 0xCD, 0x3D, 0x04, 0x73, 0x86, 0x77, 0x42, 0xB5, 0xE5, 0xCC, 0x3C
       };
       compare("EA024714AD5C4D84EA024714AD5C4D84", C->encode(clear_text));
    }

    TEST(NoekeonDecodeNormalCase, NoekeonTest)
    {
       const StringTest code("EA024714AD5C4D84EA024714AD5C4D84");
       compare("4765F3DA10CD3D0473867742B5E5CC3C", C->decode(code.hexToBytes()));
    }
}

#endif