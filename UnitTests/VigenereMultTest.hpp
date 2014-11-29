
#ifndef VIGENEREMULTTEST_HPP
#define	VIGENEREMULTTEST_HPP

#include "Test.hpp"
#include "../src/Vigenere.hpp"

namespace UnitTests
{
    class VigenereMultTest : public Test
    {
    protected:
       VigenereMult *V;
       static const StringTest clear_text;
       static const StringTest cipher_text;

       void setUp() override
       {
          V = new VigenereMult("THEKEY");
       }

       void tearDown() override
       {
          delete V;
       }
    };
    
    const StringTest VigenereMultTest::clear_text = "THISISVIGENEREWITHMULTIPLICATION";
    const StringTest VigenereMultTest::cipher_text = "400 64 45 209 45 475 440 72 35 55 70 125 360 40 115 99 100 200 260 168 60 220 45 400 240 72 15 11 100 225 300 112";

    TEST(VigenereMultEncode, VigenereMultTest) 
    {
      compare(cipher_text, V->encode(clear_text));
    }

    TEST(VigenereMultDecode, VigenereMultTest) 
    {
      const StringTest clear_text = "THISISVIGENEREWITHMULTIPLICATION";

      compare(clear_text, V->decode(cipher_text));
    }
}

#endif