
#ifndef BLAKETEST_HPP
#define BLAKETEST_HPP

#include "Test.hpp"
#include "../src/Blake.hpp"

namespace UnitTests
{
    class BlakeTest : public Test
    {
    protected:
       Blake512 *S;
       Blake384 *T;
       Blake256 *U;
       Blake224 *V;

       void setUp() override
       {
          S = new Blake512();
          T = new Blake384();
          U = new Blake256();
          V = new Blake224();
       }

       void tearDown() override
       {
          delete S;
          delete T;
          delete U;
          delete V;
       }
    };

    TEST(BlakeEncode512BitsEmptyText, BlakeTest)
    {
       const StringTest hash = "A8CFBBD73726062DF0C6864DDA65DEFE58EF0CC52A5625090FA17601E1EECD1B62"
               "8E94F396AE402A00ACC9EAB77B4D4C2E852AAAA25A636D80AF3FC7913EF5B8";

       compare(hash, S->encode({}));
    }

    TEST(BlakeEncode384BitsEmptyText, BlakeTest)
    {
       const StringTest hash = "C6CBD89C926AB525C242E6621F2F5FA73AA4AFE3D9E24AED727FAAADD6AF38B620BDB623DD2B4788B1C8086984AF8706";

       compare(hash, T->encode({}));
    }

    TEST(BlakeEncode256BitsEmptyText, BlakeTest)
    {
       const StringTest hash = "716F6E863F744B9AC22C97EC7B76EA5F5908BC5B2F67C61510BFC4751384EA7A";

       compare(hash, U->encode({}));
    }

    TEST(BlakeEncode224BitsEmptyText, BlakeTest)
    {
       const StringTest hash = "7DC5313B1C04512A174BD6503B89607AECBEE0903D40A8A569C94EED";

       compare(hash, V->encode({}));
    }

    TEST(BlakeEncode512BitsShortText, BlakeTest)
    {
       const StringTest clear_text = "119713CC83EEEF";
       const StringTest hash = "6E66BAE94DF2233958B1BE831AFA7678E247104CAFDF41C15AEADA5AC18715F1D4512114"
               "F299527A8434ED5DAA99B12BA7BB9465F6799CB0BFF9A31FD34C22D9";

       compare(hash, S->encode(clear_text.hexToBytes()));
    }

    TEST(BlakeEncode384BitsShortText, BlakeTest)
    {
       const StringTest clear_text("119713CC83EEEF");
       const StringTest hash = "3011CA63A5583CFE37EC4F3CF6477859D9C7BA11A72E5AD07F8A67D62DA643FA41617DB2EEDC017A8B43421B650B715C";

       compare(hash, T->encode(clear_text.hexToBytes()));
    }

    TEST(BlakeEncode256BitsShortText, BlakeTest)
    {
       const StringTest clear_text("119713CC83EEEF");
       const StringTest hash = "70C092FD5C8C21E9EF4BBC82A5C7819E262A530A748CAF285FF0CBA891954F1E";

       compare(hash, U->encode(clear_text.hexToBytes()));
    }

    TEST(BlakeEncode224BitsShortText, BlakeTest)
    {
       const StringTest clear_text("119713CC83EEEF");
       const StringTest hash = "F258E503C5CB61B124F5B8E154D639470A534ACA6B3B7FCCA7F683BC";

       compare(hash, V->encode(clear_text.hexToBytes()));
    }

    /*TEST(BlakeTest, encodeHMACText)
    {
       const std::string clear_text = "Hi There";
       const std::vector<uint8_t> key = {0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 
          0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B};
       std::string hash = "87AA7CDEA5EF619D4FF0B4241A1D6CB02379F4E2CE4EC2787AD0B30545E1"
          "7CDEDAA833B7D6B8A702038B274EAEA3F4E4BE9D914EEB61F1702E696C203A126854";

       compare(hash, Vector::toHexString(S->hmacEncode(key, String::toBytes(clear_text))));
    }*/
}

#endif