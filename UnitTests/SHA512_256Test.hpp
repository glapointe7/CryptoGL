
#ifndef SHA512_256TEST_HPP
#define SHA512_256TEST_HPP

#include "Test.hpp"
#include "../src/SHA2.hpp"

namespace UnitTests
{
    class SHA512_256Test : public Test
    {
    protected:
       SHA512_256 *S;

       void setUp() override
       {
          S = new SHA512_256();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(SHA512_256EncodeNormalCaseText, SHA512_256Test)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "DD9D67B371519C339ED8DBD25AF90E976A1EEEFD4AD3D889005E532FC5BEF04D";

       compare(hash, S->encode(clear_text.toBytes()).toHexString());
    }

    TEST(SHA512_256EncodeEmptyText, SHA512_256Test)
    {
       const StringTest hash = "C672B8D1EF56ED28AB87C3622C5114069BDD3AD7B8F9737498D0C01ECEF0967A";

       compare(hash, S->encode({}).toHexString());
    }
}

#endif