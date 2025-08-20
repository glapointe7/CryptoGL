#pragma once

#include "Test.hpp"
#include "../../src/algorithms/common/Base64.hpp"
#include "../../src/core/types/String.hpp"

namespace UnitTests
{
    class Base64Test : public Test
    {
    protected:
       Base64 base_64;

       void setUp() override
       {
          
       }

       void tearDown() override
       {
          
       }
    };

    TEST(Base64Encode, Base64Test) 
    {
      const String clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare("U0FMVVROSUxJTFBBUkFJVFFVRVRVQ09OTkFJU0xBUFJPR1JBTU1BVElPTk9SSUVOVEVFT0JKRVRTVVJMRUJPVVRERVNET0lHVFM=", 
              base_64.encode(clear_text.toBytes()));
    }

    TEST(Base64Decode, Base64Test) 
    {
      const String clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(clear_text, base_64.decode("U0FMVVROSUxJTFBBUkFJVFFVRVRVQ09OTkFJU0xBUFJPR1"
              "JBTU1BVElPTk9SSUVOVEVFT0JKRVRTVVJMRUJPVVRERVNET0lHVFM=").toString());
    }
}