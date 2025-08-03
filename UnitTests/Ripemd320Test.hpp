/*
 * Source : http://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/
 */
#pragma once

#include "Test.hpp"
#include "../src/Ripemd.hpp"

namespace UnitTests
{
    class Ripemd320Test : public Test
    {
    protected:
       Ripemd320 *S;

       void setUp() override
       {
          S = new Ripemd320();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(Ripemd320EncodeNormalCaseText, Ripemd320Test)
    {
       const StringTest clear_text = "message digest";
       const StringTest hash = "3A8E28502ED45D422F68844F9DD316E7B98533FA3F2A91D29F84D425C88D6B4EFF727DF66A7C0197";

       compare(hash, S->encode(clear_text.toBytes()));
    }

    TEST(Ripemd320EncodeEmptyText, Ripemd320Test)
    {
       const StringTest hash = "22D65D5661536CDC75C1FDF5C6DE7B41B9F27325EBC61E8557177D705A0EC880151C3A32A00899B8";

       compare(hash, S->encode(BytesVector()));
    }
}