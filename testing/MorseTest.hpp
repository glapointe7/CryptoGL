#ifndef MORSETEST_HPP
#define MORSETEST_HPP

#include <gtest/gtest.h>
#include "../src/Morse.hpp"

class MorseTest : public ::testing::Test
{
protected:
   Morse *M;

   virtual void SetUp()
   {
      M = new Morse();
   }

   virtual void TearDown()
   {
      delete M;
   }
};

TEST_F(MorseTest, encode)
{
   const ClassicalType clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";
   
   EXPECT_EQ(".- - - . -. -.. --- -. ... ...- --- ... .. -. ... - .-. ..- -.-. - .. --- "
           "-. ... .- ...- .- -. - .--. --- ..- .-. ... ..- .. ...- .-. . --- "
           ".--. .-. .- - .. --- -. ... ", M->encode(clear_text));
}

TEST_F(MorseTest, decode)
{
   const ClassicalType clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";
   
   EXPECT_EQ(clear_text, M->decode(".- - - . -. -.. --- -. ... ...- --- ... .. -. ... - .-. ..- -.-. - .. --- "
           "-. ... .- ...- .- -. - .--. --- ..- .-. ... ..- .. ...- .-. . --- "
           ".--. .-. .- - .. --- -. ... "));
}

#endif