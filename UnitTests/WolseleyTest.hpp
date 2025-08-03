#pragma once

#include "Test.hpp"
#include "../src/Wolseley.hpp"

namespace UnitTests
{
    class WolseleyTest : public Test
    {
    protected:
       Wolseley *W;

       void setUp() override
       {
          W = new Wolseley("TESTING");
       }

       void tearDown() override
       {
          delete W;
       }
    };

    TEST(WolseleyEncode, WolseleyTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       compare("XQFNZUVFVFBQGQVZANYZNOCUUQVXFQBGCRGQDDQZVCUCGVYUZYYCPJYZXNGFYPCNZMYXMCVRZX", W->encode(clear_text));
    }

    TEST(WolseleyDecode, WolseleyTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       W->setKey("TESTING");
       compare(clear_text, W->decode("XQFNZUVFVFBQGQVZANYZNOCUUQVXFQBGCRGQDDQZVCUCGVYUZYYCPJYZXNGFYPCNZMYXMCVRZX"));
    }
}