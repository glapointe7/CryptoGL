
#ifndef WOLSELEYTEST_HPP
#define	WOLSELEYTEST_HPP

#include <gtest/gtest.h>
#include "../src/Wolseley.hpp"

class WolseleyTest : public ::testing::Test
{
protected:
   Wolseley *W;

   virtual void SetUp()
   {
      W = new Wolseley();
   }

   virtual void TearDown()
   {
      delete W;
   }
};

TEST_F(WolseleyTest, encode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   W->setKey("TESTING");
   EXPECT_EQ("XQFNZUVFVFBQGQVZANYZNOCUUQVXFQBGCRGQDDQZVCUCGVYUZYYCPJYZXNGFYPCNZMYXMCVRZX", W->encode(clear_text));
}

TEST_F(WolseleyTest, decode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   W->setKey("TESTING");
   EXPECT_EQ(clear_text, W->decode("XQFNZUVFVFBQGQVZANYZNOCUUQVXFQBGCRGQDDQZVCUCGVYUZYYCPJYZXNGFYPCNZMYXMCVRZX"));
}

#endif