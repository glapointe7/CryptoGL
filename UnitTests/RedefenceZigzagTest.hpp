#pragma once

#include "Test.hpp"
#include "../src/Railfence.hpp"

namespace UnitTests
{
    class RedefenceZigzagTest : public Test
    {
    protected:
       RedefenceZigzag *R;

       void setUp() override
       {
          R = new RedefenceZigzag({3,1,4,0,2,5}, {3,4,6});
       }

       void tearDown() override
       {
          delete R;
       }
    };

    TEST(RedefenceZigzagEncodeWithNoOffset, RedefenceZigzagTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAM";

       compare("LAUSGAUNLATCNAARIQRMSTPUNPLIIREOILOTA", R->encode(clear_text));
    }

    TEST(RedefenceZigzagDecodeWithNoOffset, RedefenceZigzagTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAM";

       compare(clear_text, R->decode("LAUSGAUNLATCNAARIQRMSTPUNPLIIREOILOTA"));
    }

    TEST(RedefenceZigzagEncodeWithOffset, RedefenceZigzagTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAM";

       R->setOffset(2);
       compare("NATARAAULLUTCNSARIORLIEOLSTIPQUNIPMAG", R->encode(clear_text));
    }

    TEST(RedefenceZigzagDecodeWithOffset, RedefenceZigzagTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAM";

       R->setOffset(2);
       compare(clear_text, R->decode("NATARAAULLUTCNSARIORLIEOLSTIPQUNIPMAG"));
    }
}