/*
 * Source : http://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/
 */
#pragma once

#include "Test.hpp"
#include "../src/Ripemd.hpp"

namespace UnitTests
{
    class Ripemd128Test : public Test
    {
    protected:
       Ripemd128 *S;

       void setUp() override
       {
          S = new Ripemd128();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(Ripemd128EncodeNormalCaseText, Ripemd128Test)
    {
       const StringTest clear_text = "message digest";
       const StringTest hash = "9E327B3D6E523062AFC1132D7DF9D1B8";

       compare(hash, S->encode(clear_text.toBytes()));
    }

    TEST(Ripemd128EncodeEmptyText, Ripemd128Test)
    {
       const StringTest hash = "CDF26213A150DC3ECB610F18F6B38B46";

       compare(hash, S->encode(BytesVector()));
    }

    TEST(Ripemd128EncodeHMACNormalCaseText, Ripemd128Test)
    {
       const StringTest clear_text = "message digest";
       const StringTest hash = "E8503A8AEC2289D82AA0D8D445A06BDD";
       const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

       compare(hash, S->hmacEncode(key, clear_text.toBytes()));
    }

    TEST(Ripemd128EncodeHMACEmptyText, Ripemd128Test)
    {
       const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
       const StringTest hash = "AD9DB2C1E22AF9AB5CA9DBE5A86F67DC";

       compare(hash, S->hmacEncode(key, BytesVector()));
    }
}