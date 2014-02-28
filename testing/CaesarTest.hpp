#ifndef CAESARTEST_HPP
#define CAESARTEST_HPP

#include <gtest/gtest.h>
#include "../src/Vigenere.hpp"
#include "../src/String.hpp"

class CaesarTest : public ::testing::Test
{
protected:
   Caesar *C;
   
   virtual void SetUp()
   {
      C = new Caesar(3);
   }

   virtual void TearDown()
   {
      delete C;
   }
};

TEST_F(CaesarTest, encode)
{
   const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   // Encode le clear_text avec la clé = 3.
   EXPECT_EQ("VDOXWQLOLOSDUDLWTXHWXFRQQDLVODSURJUDPPDWLRQRULHQWHHREMHWVXUOHERXWGHVGRLJWV", C->encode(clear_text));

   // Encode le clear_text avec la clé = 0.
   C->setKey(0);
   EXPECT_EQ(clear_text, C->encode(clear_text));

   // Encode le clear_text avec la clé = -89.
   C->setKey(-89);
   EXPECT_EQ("HPAJICXAXAEPGPXIFJTIJRDCCPXHAPEGDVGPBBPIXDCDGXTCITTDQYTIHJGATQDJISTHSDXVIH", C->encode(clear_text));

   // Encode le clear_text avec la clé = 32 et alpha de A-Z0-9.
   C->setAlpha(ClassicalType::uppercase_digits);
   C->setKey(32);
   EXPECT_EQ("O6HQPJEHEHL6N6EPMQAPQ8KJJ6EOH6LNKCN6II6PEKJKNEAJPAAK7FAPOQNHA7KQP9AO9KECPO", C->encode(clear_text));

   // Encode le clear_text avec la clé = -71 (53) et alpha = A-Za-z0-9.
   C->setAlpha(ClassicalType::alpha_numeric);
   C->setKey(-71);
   EXPECT_EQ("J1CLKE9C9CG1I19KHL5KL3FEE19JC1GIF7I1DD1K9FEFI95EK55F2A5KJLIC52FLK45J4F97KJ", C->encode(clear_text));
}

// Teste le décodage de Caesar.

TEST_F(CaesarTest, decode)
{
   const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   C->setKey(3);
   EXPECT_EQ(clear_text, C->decode("VDOXWQLOLOSDUDLWTXHWXFRQQDLVODSURJUDPPDWLRQRULHQWHHREMHWVXUOHERXWGHVGRLJWV"));

   C->setKey(0);
   EXPECT_EQ(clear_text, C->decode(clear_text));

   C->setKey(-89);
   EXPECT_EQ(clear_text, C->decode("HPAJICXAXAEPGPXIFJTIJRDCCPXHAPEGDVGPBBPIXDCDGXTCITTDQYTIHJGATQDJISTHSDXVIH"));

   C->setAlpha(ClassicalType::uppercase_digits);
   C->setKey(32);
   EXPECT_EQ(clear_text, C->decode("O6HQPJEHEHL6N6EPMQAPQ8KJJ6EOH6LNKCN6II6PEKJKNEAJPAAK7FAPOQNHA7KQP9AO9KECPO"));

   C->setAlpha(ClassicalType::alpha_numeric);
   C->setKey(-71);
   EXPECT_EQ(clear_text, C->decode("J1CLKE9C9CG1I19KHL5KL3FEE19JC1GIF7I1DD1K9FEFI95EK55F2A5KJLIC52FLK45J4F97KJ"));
}

/*TEST_F(CaesarTest, encodeFile)
{
   const std::string clear_text = C->load(R"(../GIT_CryptoCode/testing/testFiles/clear_text.txt)");
   C->setKey(3);
   
   EXPECT_EQ("VDOXWQLOLOSDUDLWTXHWXFRQQDLVODSURJUDPPDWLRQRULHQWHHREMHWVXUOHERXWGHVGRLJWV", C->encode(clear_text));
}

// Teste le décodage de Caesar.

TEST_F(CaesarTest, decodeFile)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

   C->setKey(3);
   EXPECT_EQ(clear_text, C->decode("VDOXWQLOLOSDUDLWTXHWXFRQQDLVODSURJUDPPDWLRQRULHQWHHREMHWVXUOHERXWGHVGRLJWV"));
}*/

#endif