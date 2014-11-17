
#ifndef SHA384TEST_HPP
#define SHA384TEST_HPP

#include "Test.hpp"
#include "../src/SHA2.hpp"

namespace UnitTests
{
    class SHA384Test : public Test
    {
    protected:
       SHA384 *S;

       void setUp() override
       {
          S = new SHA384();
       }

       void tearDown() override
       {
          delete S;
       }
    };

    TEST(SHA384EncodeNormalCaseText, SHA384Test)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "CA737F1014A48F4C0B6DD43CB177B0AFD9E5169367544C494011E3317DBF9A509CB1E5DC1E85A941BBEE3D7F2AFBC9B1";

       compare(hash, S->encode(clear_text.toBytes()).toHexString());
    }

    TEST(SHA384EncodeEmptyText, SHA384Test)
    {
       const StringTest hash = "38B060A751AC96384CD9327EB1B1E36A21FDB71114BE07434C0CC7BF63F6E1DA274EDEBFE76F65FBD51AD2F14898B95B";

       compare(hash, S->encode(BytesVector()).toHexString());
    }

    TEST(SHA384EncodeHMACNormalCaseText, SHA384Test)
    {
       const StringTest clear_text = "Hi There";
       const BytesVector key = {0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 
          0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B};
       const StringTest hash = "AFD03944D84895626B0825F4AB46907F15F9DADBE4101EC682AA034C7CEB"
          "C59CFAEA9EA9076EDE7F4AF152E8B2FA9CB6";

       compare(hash, S->hmacEncode(key, clear_text.toBytes()).toHexString());
    }
}

#endif