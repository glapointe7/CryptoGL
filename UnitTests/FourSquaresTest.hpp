
#ifndef FOURSQUARESTEST_HPP
#define	FOURSQUARESTEST_HPP

#include "Test.hpp"
#include "../src/FourSquares.hpp"

namespace UnitTests
{
    class FourSquaresTest : public Test
    {
    protected:
       FourSquares *P;

       void setUp() override
       {
          P = new FourSquares("ROMEO", "JULIETTE");
       }

       void tearDown() override
       {
          delete P;
       }
    };

    TEST(FourSquaresEncode, FourSquaresTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(StringTest("NIHVSMCKCKNJNLGQNVARXJKMHIFQHUQNIDNLJHANGKLKSBEMTEAMAAARNYPHOEHZSEERAKCCSR"), P->encode(clear_text));
    }

    TEST(FourSquaresDecode, FourSquaresTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(clear_text, P->decode("NIHVSMCKCKNJNLGQNVARXJKMHIFQHUQNIDNLJHANGKLKSBEMTEAMAAARNYPHOEHZSEERAKCCSR"));
    }
}

#endif