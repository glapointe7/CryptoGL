#ifndef DELASTELLETEST_HPP
#define DELASTELLETEST_HPP

#include <gtest/gtest.h>
#include "../src/Delastelle.hpp"

class DelastelleTest : public ::testing::Test
{
protected:
   Delastelle *D;

   virtual void SetUp()
   {
      D = new Delastelle("ERABLE");
   }

   virtual void TearDown()
   {
      delete D;
   }
};

TEST_F(DelastelleTest, encode)
{
   const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   EXPECT_EQ("OLSNLKALLLOEJJISUTILVQIEZAOABXOPRCPAIQSNMMELRIMELUBAEBCSULOHETOOLCPPBCJSUTQ", D->encode(clear_text));
}

TEST_F(DelastelleTest, decode)
{
   const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTSX";

   EXPECT_EQ(clear_text, D->decode("OLSNLKALLLOEJJISUTILVQIEZAOABXOPRCPAIQSNMMELRIMELUBAEBCSULOHETOOLCPPBCJSUTQ"));
}

#endif