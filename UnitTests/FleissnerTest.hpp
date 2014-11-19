
#ifndef FLEISSNERTEST_HPP
#define	FLEISSNERTEST_HPP

#include "Test.hpp"
#include "../src/Fleissner.hpp"

namespace UnitTests
{
    class FleissnerTest : public Test
    {
    protected:
       Fleissner *F;

       void setUp() override
       {
          F = new Fleissner({std::make_pair(0u, 1u), std::make_pair(0u, 3u), std::make_pair(0u, 5u),
             std::make_pair(1u, 4u),
             std::make_pair(2u, 2u),
             std::make_pair(3u, 1u), std::make_pair(3u, 4u),
             std::make_pair(4u, 5u),
             std::make_pair(5u, 3u)
          }, 6u, true);
       }

       void tearDown() override
       {
          delete F;
       }
    };

    TEST(FleissnerEncode, FleissnerTest)
    {
       const StringTest clear_text = "JECHIFFRECETEXTEAVECUNCACHETOURNANT";

       compare("TJEEOCCUCRHENUITNAEFNCFXTATAERCAHEEV", F->encode(clear_text));
    }

    TEST(FleissnerDecode, FleissnerTest)
    {
       const StringTest clear_text = "JECHIFFRECETEXTEAVECUNCACHETOURNANTA";

       compare(clear_text, F->decode("TJEEOCCUCRHENUITNAEFNCFXTATAERCAHEEV"));
    }
}

#endif