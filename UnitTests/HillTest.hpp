#ifndef HILLTEST_HPP
#define HILLTEST_HPP

#include "Test.hpp"
#include "../src/Hill.hpp"

namespace UnitTests
{
    class HillTest : public Test
    {
    protected:
       Hill *H;

       void setUp() override
       {
          H = new Hill({{3, 4}, {5, 9}});
       }

       void tearDown() override
       {
          delete H;
       }
    };

    TEST(HillEncode2X2, HillTest)
    {
       const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       compare("CMJBFEQJQJTXZHWDYAKJQOQFNNSUHDJUOUZHGMYPCKRJFBMHVBQQNIKJEKRCQDSQRSGANLWQZX", H->encode(clear_text));
    }

    TEST(HillDecode2X2, HillTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       compare(clear_text, H->decode("CMJBFEQJQJTXZHWDYAKJQOQFNNSUHDJUOUZHGMYPCKRJFBMHVBQQNIKJEKRCQDSQRSGANLWQZX"));
    }

    TEST(HillEncode3X3, HillTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       H->setKey({{1, 3, 1}, {1, 1, 0}, {2, 9, 3}});
       compare("DSRMNQXTJEABZRGJJIDXFFQNVNYZDFCGRFXJWYCFBWUBVHMNJXIAPMBXZELSVFHCNMJWXSWOSLJ", H->encode(clear_text));
    }

    TEST(HillDecode3X3, HillTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTSX";

       H->setKey({{1, 3, 1}, {1, 1, 0}, {2, 9, 3}});
       compare(clear_text, H->decode("DSRMNQXTJEABZRGJJIDXFFQNVNYZDFCGRFXJWYCFBWUBVHMNJXIAPMBXZELSVFHCNMJWXSWOSLJ"));
    }
}

#endif