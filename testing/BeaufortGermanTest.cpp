
#ifndef BEAUFORTGERMANTEST_HPP
#define	BEAUFORTGERMANTEST_HPP

#include <gtest/gtest.h>
#include "../src/Vigenere.hpp"

class BeaufortGermanTest : public ::testing::Test
{
protected:
   BeaufortGerman *V;

   virtual void SetUp()
   {
      V = new BeaufortGerman();
   }

   virtual void TearDown()
   {
      delete V;
   }
};

TEST_F(BeaufortGermanTest, encode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  V->setKey("TESTING");
  EXPECT_EQ("ZWTBLACSETWSEUPPYBWGOJKVUSVMSWXYGTLHIUHLVIUKZPWANLAWIBRNZQZSWOIBPLLKQIPCBZ", V->encode(clear_text));
  
  /*V->setAlpha(String::alpha_numeric);
  V->setKey("V1Gen3rE");
  EXPECT_EQ("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ", V->encode(clear_text));
  
  V->setAlpha(String::letters);
  V->setKey("vvvvvvvv");
  EXPECT_EQ("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON", V->encode(clear_text));*/
}

TEST_F(BeaufortGermanTest, decode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  V->setKey("TESTING");
  EXPECT_EQ(clear_text, V->decode("ZWTBLACSETWSEUPPYBWGOJKVUSVMSWXYGTLHIUHLVIUKZPWANLAWIBRNZQZSWOIBPLLKQIPCBZ"));
  
  /*V->setAlpha(String::alpha_numeric);
  V->setKey("V1Gen3rE");
  EXPECT_EQ(clear_text, V->decode("n1Ry6GzPdCVe43zXlLKx755Ri1Owy36Vj7XezFrXdFTs4BvRo5KsoCvXnLXpr45Yo4KwqHzKoJ"));
  
  V->setAlpha(String::letters);
  V->setKey("vvvvvvvv");
  EXPECT_EQ(clear_text, V->decode("NvGPOIDGDGKvMvDOLPzOPxJIIvDNGvKMJBMvHHvODJIJMDzIOzzJwEzONPMGzwJPOyzNyJDBON"));*/
}

#endif