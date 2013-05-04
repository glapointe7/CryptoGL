
#ifndef RAILFENCETEST_HPP
#define	RAILFENCETEST_HPP

#include <gtest/gtest.h>
#include "../src/Railfence.hpp"

class RailfenceTest : public ::testing::Test
{
protected:
   Railfence *R;

   virtual void SetUp()
   {
      R = new Railfence();
   }

   virtual void TearDown()
   {
      delete R;
   }
};

TEST_F(RailfenceTest, encode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   R->setKey(6);
   EXPECT_EQ("SPUPIEEIALATCARTOEOLBOGLIREOLOANTBRODTULAUNSGMONJUUSSTIIQNIRMREESTENTAAITD", R->encode(clear_text));
}

TEST_F(RailfenceTest, decode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   R->setKey(6);
   EXPECT_EQ(clear_text, R->decode("SPUPIEEIALATCARTOEOLBOGLIREOLOANTBRODTULAUNSGMONJUUSSTIIQNIRMREESTENTAAITD"));
}

#endif