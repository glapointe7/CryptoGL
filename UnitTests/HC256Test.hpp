/*
 * Source : http://www.ecrypt.eu.org/stream/ciphers/hc256/hc256.pdf
 */
#pragma once

#include "Test.hpp"
#include "../src/HC256.hpp"

namespace UnitTests
{
    class HC256Test : public Test
    {
    protected:
       HC256 *R;

       void setUp() override
       {
          R = new HC256(BytesVector(32, 0), BytesVector(32, 0));
       }

       void tearDown() override
       {
          delete R;
       }
    };

    TEST(HC256EncodeZeroValues, HC256Test)
    {
       const StringTest clear_text = "8589075B0DF3F6D82FC0C5425179B6A63465F053F2891F808B24744E18480B72"
          "EC2792CDBF4DCFEB7769BF8DFA14AEE47B4C50E8EAF3A9C8F506016C81697E32";

       compare(clear_text, R->encode(BytesVector(64, 0)));
    }
}