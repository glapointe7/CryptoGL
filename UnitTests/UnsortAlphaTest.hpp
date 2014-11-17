#ifndef UNSORTALPHATEST_HPP
#define UNSORTALPHATEST_HPP

#include "Test.hpp"
#include "../src/UnsortAlpha.hpp"
#include "../src/String.hpp"

namespace UnitTests
{
    class UnsortAlphaTest : public Test
    {
    protected:
       UnsortAlpha *UA;

       void setUp() override
       {
          UA = new UnsortAlpha("TESTING");
       }

       void tearDown() override
       {
          delete UA;
       }
    };

    TEST(UnsortAlphaEncodeHorizontal, UnsortAlphaTest)
    {
       const StringTest clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";

       UA->setHorizontalAlpha();
       compare(StringTest("TRRNKILKQVLQCKQRPUSRCLKQTVTKRMLUPQUCVPNLMPTRCLKQ"), UA->encode(clear_text));
    }

    TEST(UnsortAlphaEncodeVertical, UnsortAlphaTest)
    {
       const StringTest clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";

       UA->setKey("TESTING");
       UA->setVerticalAlpha();
       compare(StringTest("TFFYUQIUNWINRUNFVOJFRIUNTWTUFDIOVNORWVYIDVTFRIUN"), UA->encode(clear_text));
    }

    TEST(UnsortAlphaDecodeHorizontal, UnsortAlphaTest)
    {
       const StringTest clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";

       UA->setKey("TESTING");
       UA->setHorizontalAlpha();
       compare(clear_text, UA->decode("TRRNKILKQVLQCKQRPUSRCLKQTVTKRMLUPQUCVPNLMPTRCLKQ"));
    }

    TEST(UnsortAlphaDecodeVertical, UnsortAlphaTest)
    {
       const StringTest clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";

       UA->setKey("TESTING");
       UA->setVerticalAlpha();
       compare(clear_text, UA->decode("TFFYUQIUNWINRUNFVOJFRIUNTWTUFDIOVNORWVYIDVTFRIUN"));
    }
}

#endif