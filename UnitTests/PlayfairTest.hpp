#ifndef PLAYFAIRTEST_HPP
#define PLAYFAIRTEST_HPP

#include "Test.hpp"
#include "../src/Playfair.hpp"

namespace UnitTests
{
    class PlayfairTest : public Test
    {
    protected:
       Playfair *P;

       void setUp() override
       {
          P = new Playfair("TESTING");
       }

       void tearDown() override
       {
          delete P;
       }
    };

    TEST(PlayfairEncode, PlayfairTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(StringTest("EBFZETNKNKOBODNEMYSEYGREEDNIHDQMMAODPUOGENREPMNSTESVAVJPSETXZRSAMVNGSIARTCEI"), P->encode(clear_text));
    }

    TEST(PlayfairDecode, PlayfairTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMXMATIONORIENTEXEOBJETSURLEBOUTDESDOIGTS";

      compare(clear_text, P->decode("EBFZETNKNKOBODNEMYSEYGREEDNIHDQMMAODPUOGENREPMNSTESVAVJPSETXZRSAMVNGSIARTCEI"));
    }
}

#endif