
#ifndef WHIRLPOOLTEST_HPP
#define WHIRLPOOLTEST_HPP

#include "Test.hpp"
#include "../src/Whirlpool.hpp"

namespace UnitTests
{
    class WhirlpoolTest : public Test
    {
    protected:
       Whirlpool *S;

       void setUp() override
       {
          S = new Whirlpool();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(WhirlpoolEncodeEmptyValue, WhirlpoolTest)
    {
       const StringTest hash = "19FA61D75522A4669B44E39C1D2E1726C530232130D407F89AFEE0964997F7A73E83B"
               "E698B288FEBCF88E3E03C4F0757EA8964E59B63D93708B138CC42A66EB3";

       compare(hash, S->encode(BytesVector()));
    }

    TEST(WhirlpoolEncodeNormalCaseText, WhirlpoolTest)
    {
       const StringTest clear_text = "message digest";
       const StringTest hash = "378C84A4126E2DC6E56DCC7458377AAC838D00032230F53CE1F5700C0FFB4D3B842155"
               "7659EF55C106B4B52AC5A4AAA692ED920052838F3362E86DBD37A8903E";

       compare(hash, S->encode(clear_text.toBytes()));
    }
}

#endif