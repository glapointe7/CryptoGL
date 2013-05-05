
#ifndef DELASTELLETEST_HPP
#define	DELASTELLETEST_HPP

#include <gtest/gtest.h>
#include "../src/Delastelle.hpp"

class DelastelleTest : public ::testing::Test
{
protected:
   Delastelle *D;

   virtual void SetUp()
   {
      D = new Delastelle();
   }

   virtual void TearDown()
   {
      delete D;
   }
};

TEST_F(DelastelleTest, encode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   D->setKey("ERABLE");
   D->setBlockLength(5);
   EXPECT_EQ("OLSNLKALLLOEJJISUTILVQIEZAOABXOPRCPAIQSNMMELRIMELUBAEBCSULOHETOOLCPPBCJSUTQ", D->encode(clear_text));
}

TEST_F(DelastelleTest, decode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTSX";

   D->setKey("ERABLE");
   D->setBlockLength(5);
   EXPECT_EQ(clear_text, D->decode("OLSNLKALLLOEJJISUTILVQIEZAOABXOPRCPAIQSNMMELRIMELUBAEBCSULOHETOOLCPPBCJSUTQ"));
}

#endif