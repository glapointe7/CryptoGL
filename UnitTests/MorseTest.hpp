#pragma once

#include "Test.hpp"
#include "../src/Morse.hpp"

namespace UnitTests
{
    class MorseTest : public Test
    {
    protected:
       Morse *M;

       void setUp() override
       {
          M = new Morse();
       }

       void tearDown() override
       {
          delete M;
       }
    };

    TEST(MorseEncode, MorseTest)
    {
       const StringTest clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";

       compare(".- - - . -. -.. --- -. ... ...- --- ... .. -. ... - .-. ..- -.-. - .. --- "
               "-. ... .- ...- .- -. - .--. --- ..- .-. ... ..- .. ...- .-. . --- "
               ".--. .-. .- - .. --- -. ... ", M->encode(clear_text));
    }

    TEST(MorseDecode, MorseTest)
    {
       const StringTest clear_text = "ATTENDONSVOSINSTRUCTIONSAVANTPOURSUIVREOPRATIONS";

       compare(clear_text, M->decode(".- - - . -. -.. --- -. ... ...- --- ... .. -. ... - .-. ..- -.-. - .. --- "
               "-. ... .- ...- .- -. - .--. --- ..- .-. ... ..- .. ...- .-. . --- "
               ".--. .-. .- - .. --- -. ... "));
    }
}