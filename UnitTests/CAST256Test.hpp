#pragma once

#include "Test.hpp"
#include "../src/CAST256.hpp"

namespace UnitTests
{
    class CAST256Test : public Test
    {
    protected:
       CAST256 *C128, *C192, *C256;

       void setUp() override
       {
          C128 = new CAST256({0x23, 0x42, 0xbb, 0x9e, 0xfa, 0x38, 0x54, 0x2c, 0x0a, 0xf7, 0x56, 0x47, 0xf2, 0x9f, 0x61, 0x5d});
          C192 = new CAST256({0x23, 0x42, 0xbb, 0x9e, 0xfa, 0x38, 0x54, 0x2c, 0xbe, 0xd0, 0xac, 0x83, 0x94, 0x0a, 0xc2, 0x98, 
                  0xba, 0xc7, 0x7a, 0x77, 0x17, 0x94, 0x28, 0x63});
          C256 = new CAST256({0x23, 0x42, 0xbb, 0x9e, 0xfa, 0x38, 0x54, 0x2c, 0xbe, 0xd0, 0xac, 0x83, 0x94, 0x0a, 0xc2, 0x98, 
                  0x8d, 0x7c, 0x47, 0xce, 0x26, 0x49, 0x08, 0x46, 0x1c, 0xc1, 0xb5, 0x13, 0x7a, 0xe6, 0xb6, 0x04});
       }

       void tearDown() override
       {
          delete C128;
          delete C192;
          delete C256;
       }
    };

    TEST(CAST256Encode128Bits, CAST256Test)
    {
       compare("C842A08972B43D20836C91D1B7530F6B", C128->encode(BytesVector(16, 0)));
    }

    TEST(CAST256Decode128Bits, CAST256Test)
    {
       const StringTest code = "C842A08972B43D20836C91D1B7530F6B";
       compare("00000000000000000000000000000000", C128->decode(code.hexToBytes()));
    }

    TEST(CAST256Encode192Bits, CAST256Test)
    {
       compare("1B386C0210DCADCBDD0E41AA08A7A7E8", C192->encode(BytesVector(16, 0)));
    }

    TEST(CAST256Decode192Bits, CAST256Test)
    {
       const StringTest code = "1B386C0210DCADCBDD0E41AA08A7A7E8";
       compare("00000000000000000000000000000000", C192->decode(code.hexToBytes()));
    }

    TEST(CAST256Encode256Bits, CAST256Test)
    {
       compare("4F6A2038286897B9C9870136553317FA", C256->encode(BytesVector(16, 0)));
    }

    TEST(CAST256Decode256Bits, CAST256Test)
    {
       const StringTest code = "4F6A2038286897B9C9870136553317FA";
       compare("00000000000000000000000000000000", C256->decode(code.hexToBytes()));
    }
}