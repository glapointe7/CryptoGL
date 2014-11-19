
#ifndef BASE64TEST_HPP
#define BASE64TEST_HPP

#include "Test.hpp"
#include "../src/Base64.hpp"

namespace UnitTests
{
    class Base64Test : public Test
    {
    protected:
       Base64 *A;

       void setUp() override
       {
          A = new Base64();
       }

       void tearDown() override
       {
          delete A;
       }
    };

    TEST(Base64Encode, Base64Test) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare("U0FMVVROSUxJTFBBUkFJVFFVRVRVQ09OTkFJU0xBUFJPR1JBTU1BVElPTk9SSUVOVEVFT0JKRVRTVVJMRUJPVVRERVNET0lHVFM=", 
              A->encode(clear_text.toBytes()));
    }

    TEST(Base64Decode, Base64Test) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(clear_text, A->decode("U0FMVVROSUxJTFBBUkFJVFFVRVRVQ09OTkFJU0xBUFJPR1"
              "JBTU1BVElPTk9SSUVOVEVFT0JKRVRTVVJMRUJPVVRERVNET0lHVFM=").toString());
    }
}

#endif