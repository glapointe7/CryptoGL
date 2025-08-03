/*
 * Source : http://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/
 */
#pragma once

#include "Test.hpp"
#include "../src/Ripemd.hpp"

namespace UnitTests
{
    class Ripemd160Test : public Test
    {
    protected:
       Ripemd160 *S;

       void setUp() override
       {
          S = new Ripemd160();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(Ripemd160EncodeNormalCaseText, Ripemd160Test)
    {
       const StringTest clear_text = "message digest";
       const StringTest hash = "5D0689EF49D2FAE572B881B123A85FFA21595F36";

       compare(hash, S->encode(clear_text.toBytes()));
    }

    TEST(Ripemd160EncodeEmptyText, Ripemd160Test)
    {
       const StringTest hash = "9C1185A5C5E9FC54612808977EE8F548B2258D31";

       compare(hash, S->encode(BytesVector()));
    }

    TEST(Ripemd160EncodeHMACNormalCaseText, Ripemd160Test)
    {
       const StringTest clear_text = "message digest";
       const StringTest hash = "F83662CC8D339C227E600FCD636C57D2571B1C34";
       const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01, 0x23, 0x45, 0x67};

       compare(hash, S->hmacEncode(key, clear_text.toBytes()));
    }

    TEST(Ripemd160EncodeHMACEmptyText, Ripemd160Test)
    {
       const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01, 0x23, 0x45, 0x67};
       const StringTest hash = "CF387677BFDA8483E63B57E06C3B5ECD8B7FC055";

       compare(hash, S->hmacEncode(key, BytesVector()));
    }
}