#ifndef TRANSPOSITIONROWSTEST_HPP
#define TRANSPOSITIONROWSTEST_HPP

#include <gtest/gtest.h>
#include "../src/Transposition.hpp"

class TranspositionRowsTest : public ::testing::Test
{
protected:
   TranspositionRows *TR;

   virtual void SetUp()
   {
      TR = new TranspositionRows();
   }

   virtual void TearDown()
   {
      delete TR;
   }
};

TEST_F(TranspositionRowsTest, encode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   TR->setKey("VENDREDI");
   EXPECT_EQ("UIANLLTSAILATPRITOUCNEUQSPAARILNAAGMTRMOOEOINNRIOEEJTEBTLOUBURESSIDOGEDTXXSXXXXT", TR->encode(clear_text));
}

TEST_F(TranspositionRowsTest, decode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTSXXXXXX";

   TR->setKey("VENDREDI");
   EXPECT_EQ(clear_text, TR->decode("UIANLLTSAILATPRITOUCNEUQSPAARILNAAGMTRMOOEOINNRIOEEJTEBTLOUBURESSIDOGEDTXXSXXXXT"));
}

#endif