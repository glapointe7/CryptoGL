#pragma once

#include "Test.hpp"
#include "../src/Polybius.hpp"

namespace UnitTests
{
    class PolybeTest : public Test
    {
    protected:
       Polybius *P;

       void setUp() override
       {
          P = new Polybius("TESTING");
       }

       void tearDown() override
       {
          delete P;
       }
    };

    TEST(PolybeEncode, PolybeTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare("132235511115143514354322452214114451121151244215152214133522434542214522414"
              "1221114421542451412151112124223331211135145351223425111251213254214211113", P->encode(clear_text));
    }

    TEST(PolybeDecode, PolybeTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(clear_text, P->decode("13223551111514351435432245221411445112115124421515221413352243454221452"
              "24141221114421542451412151112124223331211135145351223425111251213254214211113"));
    }
}