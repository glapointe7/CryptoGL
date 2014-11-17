
#ifndef GRONSFELDTEST_HPP
#define	GRONSFELDTEST_HPP

#include "Test.hpp"
#include "../src/Vigenere.hpp"

namespace UnitTests
{
    class GronsfeldTest : public Test
    {
    protected:
       Gronsfeld *V;

       void setUp() override
       {
          V = new Gronsfeld({19, 4, 18, 19, 8, 13, 6});
       }

       void tearDown() override
       {
          delete V;
       }
    };

    TEST(GronsfeldEncode, GronsfeldTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(StringTest("LEDNBAOEMDIIEGBXINMGAVSFGIVYEEHKWTXTQETBVUGSJBMAZXIGURRZLYJEMOUNXVXAQUBKLL"), V->encode(clear_text));
    }

    TEST(GronsfeldDecode, GronsfeldTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(clear_text, V->decode("LEDNBAOEMDIIEGBXINMGAVSFGIVYEEHKWTXTQETBVUGSJBMAZXIGURRZLYJEMOUNXVXAQUBKLL"));
    }
}

#endif