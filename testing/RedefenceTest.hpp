
#ifndef REDEFENCETEST_HPP
#define	REDEFENCETEST_HPP

#include <gtest/gtest.h>
#include "../src/Railfence.hpp"

class RedefenceTest : public ::testing::Test
{
protected:
   Redefence *R;

   virtual void SetUp()
   {
      R = new Redefence({4,2,5,0,3,1});
   }

   virtual void TearDown()
   {
      delete R;
   }
};

TEST_F(RedefenceTest, encode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
// "SPUPIEEI ALATCARTOEOLBOG LIREOLOANTBRODT ULAUNSGMONJUUSS TIIQNIRMREESTE NTAAITD"
   EXPECT_EQ("TIIQNIRMREESTELIREOLOANTBRODTNTAAITDSPUPIEEIULAUNSGMONJUUSSALATCARTOEOLBOG", R->encode(clear_text));
}

/*TEST_F(RedefenceTest, decode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   EXPECT_EQ(clear_text, R->decode("TIIQNIRMREESTELIREOLOANTBRODTNTAAITDSPUPIEEIULAUNSGMONJUUSSALATCARTOEOLBOG"));
}*/

#endif