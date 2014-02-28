
#ifndef REDEFENCEZIGZAGTEST_HPP
#define	REDEFENCEZIGZAGTEST_HPP

#include <gtest/gtest.h>
#include "../src/Railfence.hpp"

class RedefenceZigzagTest : public ::testing::Test
{
protected:
   RedefenceZigzag *R;

   virtual void SetUp()
   {
      R = new RedefenceZigzag({3,1,4,0,2,5}, {3,4,6});
   }

   virtual void TearDown()
   {
      delete R;
   }
};

TEST_F(RedefenceZigzagTest, encode)
{
   const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAM";

   EXPECT_EQ("LAUSGAUNLATCNAARIQRMSTPUNPLIIREOILOTA", R->encode(clear_text));
}

TEST_F(RedefenceZigzagTest, decode)
{
   const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAM";

   EXPECT_EQ(clear_text, R->decode("LAUSGAUNLATCNAARIQRMSTPUNPLIIREOILOTA"));
}

TEST_F(RedefenceZigzagTest, encodeWithOffset)
{
   const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAM";
   
   R->setOffset(2);
   EXPECT_EQ("NATARAAULLUTCNSARIORLIEOLSTIPQUNIPMAG", R->encode(clear_text));
}

TEST_F(RedefenceZigzagTest, decodeWithOffset)
{
   const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAM";

   R->setOffset(2);
   EXPECT_EQ(clear_text, R->decode("NATARAAULLUTCNSARIORLIEOLSTIPQUNIPMAG"));
}

#endif