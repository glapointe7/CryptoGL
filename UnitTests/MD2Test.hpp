
#ifndef MD2TEST_HPP
#define MD2TEST_HPP

#include "Test.hpp"
#include "../src/MD2.hpp"

namespace UnitTests
{
    class MD2Test : public Test
    {
    protected:
       MD2 *S;

       void setUp() override
       {
          S = new MD2();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(MD2EncodeNormalCaseText, MD2Test)
    {
       const StringTest clear_text = "message digest";
       const StringTest hash = "AB4F496BFB2A530B219FF33031FE06B0";

       compare(hash, S->encode(clear_text.toBytes()));
    }

    TEST(MD2EncodeEmptyText, MD2Test)
    {
       const StringTest hash = "8350E5A3E24C153DF2275C9F80692773";

       compare(hash, S->encode(BytesVector()));
    }
}

#endif