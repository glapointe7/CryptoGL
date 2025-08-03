#pragma once

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
       compare("TRRNKILKQVLQCKQRPUSRCLKQTVTKRMLUPQUCVPNLMPTRCLKQ", UA->encode(clear_text));
    }

    TEST(UnsortAlphaEncodeVertical, UnsortAlphaTest)
    {
       const StringTest clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";

       UA->setKey("TESTING");
       UA->setVerticalAlpha();
       compare("TFFYUQIUNWINRUNFVOJFRIUNTWTUFDIOVNORWVYIDVTFRIUN", UA->encode(clear_text));
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