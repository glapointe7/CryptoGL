#ifndef POLYBETEST_HPP
#define POLYBETEST_HPP

#include <gtest/gtest.h>
#include "../src/Polybius.hpp"

class PolybeTest : public ::testing::Test
{
protected:
   Polybius *P;

   virtual void SetUp()
   {
      P = new Polybius("TESTING");
   }

   virtual void TearDown()
   {
      delete P;
   }
};

TEST_F(PolybeTest, encode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ("132235511115143514354322452214114451121151244215152214133522434542214522414"
          "1221114421542451412151112124223331211135145351223425111251213254214211113", P->encode(clear_text));
}

TEST_F(PolybeTest, decode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  EXPECT_EQ(clear_text, P->decode("13223551111514351435432245221411445112115124421515221413352243454221452"
          "24141221114421542451412151112124223331211135145351223425111251213254214211113"));
}

#endif