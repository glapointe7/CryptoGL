#pragma once

#include "Test.hpp"
#include "../src/Chao.hpp"

namespace UnitTests
{
    class ChaoTest : public Test
    {
    protected:
       Chao *C;

       void setUp() override
       {
          C = new Chao("HXUCZVAMDSLKPEFJRIGTWOBNYQ", "PTLNBQDEOYSFAVZKGJRIHWXUMC");
       }

       void tearDown() override
       {
          delete C;
       }
    };

    TEST(ChaoEncode, ChaoTest)
    {
       const StringTest clear_text = "WELLDONEISBETTERTHANWELLSAID";

       compare("OAHQHCNYNXTSZJRRHJBYHQKSOUJY", C->encode(clear_text));
    }

    TEST(ChaoDecode, ChaoTest)
    {
       const StringTest clear_text = "WELLDONEISBETTERTHANWELLSAID";

       compare(clear_text, C->decode("OAHQHCNYNXTSZJRRHJBYHQKSOUJY"));
    }
}