#ifndef COLLONTEST_HPP
#define COLLONTEST_HPP

#include <gtest/gtest.h>
#include "../src/Collon.hpp"

class CollonTest : public ::testing::Test
{
protected:
   Collon *C;

   virtual void SetUp()
   {
      C = new Collon();
   }

   virtual void TearDown()
   {
      delete C;
   }
};

TEST_F(CollonTest, encode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  C->setKey("TESTING");
  C->setBlockLength(5);
  EXPECT_EQ("TGFUTXVZUUTTFTFZYZYZMGMGTXVZVYTMUTTUYUVUUGMTTUYVZZGTTFGVYXZVMMMGMXZVUZGMMGTVUUV"
          "UTMTMMYVZVZTTTTTYVZUVTMGFTVVXXVTTUMFUXUZZTGMUTVXVUUGTTGMZVXZVTGTTYUUX", C->encode(clear_text));
}

TEST_F(CollonTest, decode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  C->setKey("TESTING");
  C->setBlockLength(5);
  EXPECT_EQ(clear_text, C->decode("TGFUTXVZUUTTFTFZYZYZMGMGTXVZVYTMUTTUYUVUUGMTTUYVZZGTTFGVYXZV"
          "MMMGMXZVUZGMMGTVUUVUTMTMMYVZVZTTTTTYVZUVTMGFTVVXXVTTUMFUXUZZTGMUTVXVUUGTTGMZVXZVTGTTYUUX"));
}

#endif