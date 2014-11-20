
#ifndef TRIPLEDESTEST_HPP
#define TRIPLEDESTEST_HPP

#include "Test.hpp"
#include "../src/TripleDES.hpp"

namespace UnitTests
{
    class TripleDESTest : public Test
    {
    protected:
       TripleDES *D;

       void setUp() override
       {
          const BytesVector key1 = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
          const BytesVector key2 = {0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01};
          const BytesVector key3 = {0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23};
          D = new TripleDES(key1, key2, key3);
       }

       void tearDown() override
       {
          delete D;
       }
    };

    TEST(TripleDESEncode, TripleDESTest)
    {
       const StringTest clear_text = "The qufck brown fox jump";

       compare("A826FD8CE53B855FCCE21C8112256FE668D5C05DD9B6B900",
               D->encode(clear_text.toBytes()));
    }

    TEST(TripleDESDecode, TripleDESTest)
    {
       const StringTest clear_text = "The qufck brown fox jump";
       const StringTest message("A826FD8CE53B855FCCE21C8112256FE668D5C05DD9B6B900");

       compare(clear_text, D->decode(message.hexToBytes()).toString());
    }
}

#endif