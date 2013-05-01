#include <gtest/gtest.h>
#include "../src/Vigenere.hpp"

class CaesarTest : public ::testing::Test
{
protected:
   Caesar *C;

   virtual void SetUp()
   {
      C = new Caesar();
   }

   virtual void TearDown()
   {
      delete C;
   }
};

// Test Caesar encoding with key 3.
TEST_F(CaesarTest, encode) 
{
  const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
  
  // Encode le clear_text avec la clé = 3.
  C->setKey(3);
  EXPECT_EQ("VDOXWQLOLOSDUDLWTXHWXFRQQDLVODSURJUDPPDWLRQRULHQWHHREMHWVXUOHERXWGHVGRLJWV", C->encode(clear_text));
  
  // Encode le clear_text avec la clé = -3.
  C->setKey(-3);
  EXPECT_EQ("PXIRQKFIFIMXOXFQNRBQRZLKKXFPIXMOLDOXJJXQFLKLOFBKQBBLYGBQPROIBYLRQABPALFDQP", C->encode(clear_text));
  
  // Encode le clear_text avec la clé = 0.
  C->setKey(0);
  EXPECT_EQ(clear_text, C->encode(clear_text));
  
  // Encode le clear_text avec la clé = -89.
  C->setKey(-89);
  EXPECT_EQ("HPAJICXAXAEPGPXIFJTIJRDCCPXHAPEGDVGPBBPIXDCDGXTCITTDQYTIHJGATQDJISTHSDXVIH", C->encode(clear_text));
  
  // Encode le clear_text avec la clé = 100.
  C->setKey(100);
  EXPECT_EQ("OWHQPJEHEHLWNWEPMQAPQYKJJWEOHWLNKCNWIIWPEKJKNEAJPAAKXFAPOQNHAXKQPZAOZKECPO", C->encode(clear_text));
  
  // Encode le clear_text avec la clé = 32 et alpha de A-Z0-9.
  C->setAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
  C->setKey(32);
  EXPECT_EQ("O6HQPJEHEHL6N6EPMQAPQ8KJJ6EOH6LNKCN6II6PEKJKNEAJPAAK7FAPOQNHA7KQP9AO9KECPO", C->encode(clear_text));
  
  // Encode le clear_text avec la clé = -71 (53) et alpha = A-Za-z0-9.
  C->setAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  C->setKey(-71);
  EXPECT_EQ("J1CLKE9C9CG1I19KHL5KL3FEE19JC1GIF7I1DD1K9FEFI95EK55F2A5KJLIC52FLK45J4F97KJ", C->encode(clear_text));
  
  // Encode le clear_text avec la clé = 24 et alpha = A-T.
  C->setAlpha("ABCDEFGHIJKLMNOPQRST");
  C->setKey(24);
  EXPECT_EQ("CEPDDRMPMPTEBEMDADIDDGSRREMCPETBSKBEQQEDMSRSBMIRDIISFNIDCDBPIFSDDHICHSMKDC", C->encode(clear_text));
}