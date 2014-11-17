/*
 * Source : http://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/
 */
#ifndef RIPEMD256TEST_HPP
#define RIPEMD256TEST_HPP

#include "Test.hpp"
#include "../src/Ripemd.hpp"

namespace UnitTests
{
    class Ripemd256Test : public Test
    {
    protected:
       Ripemd256 *S;

       void setUp() override
       {
          S = new Ripemd256();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(Ripemd256EncodeNormalCaseText, Ripemd256Test)
    {
       const StringTest clear_text = "message digest";
       const StringTest hash = "87E971759A1CE47A514D5C914C392C9018C7C46BC14465554AFCDF54A5070C0E";

       compare(hash, S->encode(clear_text.toBytes()).toHexString());
    }

    TEST(Ripemd256EncodeEmptyText, Ripemd256Test)
    {
       const StringTest hash = "02BA4C4E5F8ECD1877FC52D64D30E37A2D9774FB1E5D026380AE0168E3C5522D";

       compare(hash, S->encode(BytesVector()).toHexString());
    }
}

#endif