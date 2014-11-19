
#ifndef NIHILISTESTEST_HPP
#define	NIHILISTESTEST_HPP

#include "Test.hpp"
#include "../src/Nihilistes.hpp"

namespace UnitTests
{
    class NihilistesTest : public Test
    {
    protected:
       Nihilistes *P;

       void setUp() override
       {
          P = new Nihilistes("DIFFICILE", "EASY");
       }

       void tearDown() override
       {
          delete P;
       }
    };

    TEST(NihilistesEncode, NihilistesTest) 
    {
      const StringTest clear_text = "LECOYOTEHURLE";

      compare("36435889755789754673876942", P->encode(clear_text));
    }

    TEST(NihilistesDecode, NihilistesTest) 
    {
      const StringTest clear_text = "LECOYOTEHURLE";

      compare(clear_text, P->decode("36435889755789754673876942"));
    }
}

#endif