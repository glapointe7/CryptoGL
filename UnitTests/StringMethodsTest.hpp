
#ifndef STRINGMETHODSTEST_HPP
#define	STRINGMETHODSTEST_HPP

#include "Test.hpp"
#include "../src/String.hpp"

namespace UnitTests
{
    class StringMethodsTest : public Test
    {
    protected:
       String *str;

       void setUp() override
       {
          str = new String("THIS IS A TEST !");
       }

       void tearDown() override
       {
          delete str;
       }
    };

    TEST(StringRemoveDuplicates, StringMethodsTest) 
    {
        str->removeDuplicates();
        compare("THIS AE!", *str);
    }

    TEST(StringRemoveChars, StringMethodsTest) 
    {
        str->removeChars("T ");
        compare("HISISAES!", *str);
    }
}

#endif	