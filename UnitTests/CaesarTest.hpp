#ifndef CAESARTEST_HPP
#define CAESARTEST_HPP

#include "Test.hpp"
#include "../src/Vigenere.hpp"

namespace UnitTests
{
    class CaesarTest : public Test
    {
    protected:
       Caesar *C;
       static const StringTest clear_text;

       void setUp() override
       {
          C = new Caesar(3);
       }

       void tearDown() override
       {
          delete C;
       }
    };
    
    const StringTest CaesarTest::clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

    TEST(CaesarEncodeKey3, CaesarTest)
    {
       //const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
       compare("VDOXWQLOLOSDUDLWTXHWXFRQQDLVODSURJUDPPDWLRQRULHQWHHREMHWVXUOHERXWGHVGRLJWV", C->encode(clear_text));
    }
    
    TEST(CaesarEncodeKey0, CaesarTest)
    {
       C->setKey(0);
       compare(clear_text, C->encode(clear_text));
    }

    TEST(CaesarEncodeKeyMinus89, CaesarTest)
    {
       C->setKey(-89);
       compare("HPAJICXAXAEPGPXIFJTIJRDCCPXHAPEGDVGPBBPIXDCDGXTCITTDQYTIHJGATQDJISTHSDXVIH", C->encode(clear_text));
    }

    TEST(CaesarEncodeKey32AlphaNum, CaesarTest)
    {
       C->setAlpha(StringTest::uppercase_digits);
       C->setKey(32);
       compare("O6HQPJEHEHL6N6EPMQAPQ8KJJ6EOH6LNKCN6II6PEKJKNEAJPAAK7FAPOQNHA7KQP9AO9KECPO", C->encode(clear_text));
    }

    TEST(CaesarEncodeKeyMinus71AlphaNum, CaesarTest)
    {
       C->setAlpha(StringTest::alpha_numeric);
       C->setKey(-71);
       compare("J1CLKE9C9CG1I19KHL5KL3FEE19JC1GIF7I1DD1K9FEFI95EK55F2A5KJLIC52FLK45J4F97KJ", C->encode(clear_text));
    }

    // Decode Caesar section

    TEST(CaesarDecodeKey3, CaesarTest)
    {
       //const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       C->setKey(3);
       compare(clear_text, C->decode("VDOXWQLOLOSDUDLWTXHWXFRQQDLVODSURJUDPPDWLRQRULHQWHHREMHWVXUOHERXWGHVGRLJWV"));
    }

    TEST(CaesarDecodeKey0, CaesarTest)
    {
       C->setKey(0);
       compare(clear_text, C->decode(clear_text));
    }

    TEST(CaesarDecodeKeyMinus89, CaesarTest)
    {
       C->setKey(-89);
       compare(clear_text, C->decode("HPAJICXAXAEPGPXIFJTIJRDCCPXHAPEGDVGPBBPIXDCDGXTCITTDQYTIHJGATQDJISTHSDXVIH"));
    }

    TEST(CaesarDecodeKey32AlphaNum, CaesarTest)
    {
       C->setAlpha(ClassicalType::uppercase_digits);
       C->setKey(32);
       compare(clear_text, C->decode("O6HQPJEHEHL6N6EPMQAPQ8KJJ6EOH6LNKCN6II6PEKJKNEAJPAAK7FAPOQNHA7KQP9AO9KECPO"));
    }

    TEST(CaesarDecodeKeyMinus71AlphaNum, CaesarTest)
    {
       C->setAlpha(ClassicalType::alpha_numeric);
       C->setKey(-71);
       compare(clear_text, C->decode("J1CLKE9C9CG1I19KHL5KL3FEE19JC1GIF7I1DD1K9FEFI95EK55F2A5KJLIC52FLK45J4F97KJ"));
    }

    /*TEST(CaesarTest, encodeFile)
    {
       const std::string clear_text = C->load(R"(../GIT_CryptoCode/testing/testFiles/clear_text.txt)");
       C->setKey(3);

       compare("VDOXWQLOLOSDUDLWTXHWXFRQQDLVODSURJUDPPDWLRQRULHQWHHREMHWVXUOHERXWGHVGRLJWV", C->encode(clear_text));
    }

    // Teste le décodage de Caesar.

    TEST(CaesarTest, decodeFile)
    {
       const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       C->setKey(3);
       compare(clear_text, C->decode("VDOXWQLOLOSDUDLWTXHWXFRQQDLVODSURJUDPPDWLRQRULHQWHHREMHWVXUOHERXWGHVGRLJWV"));
    }*/
}

#endif