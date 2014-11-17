
#ifndef TIGERTEST_HPP
#define TIGERTEST_HPP

#include "Test.hpp"
#include "../src/Tiger.hpp"

namespace UnitTests
{
    class TigerTest : public Test
    {
    protected:
       Tiger *T1, *T2, *T3, *Tiger2;

       void setUp() override
       {
          T1 = new Tiger(Tiger::HashSize::_128bits);
          T2 = new Tiger(Tiger::HashSize::_160bits);
          T3 = new Tiger(Tiger::HashSize::_192bits);
          Tiger2 = new Tiger(Tiger::HashSize::_192bits, Tiger::Version::tiger2);
       }

       void tearDown() override
       {
          delete T1;
          delete T2;
          delete T3;
          delete Tiger2;
       }
    };

    TEST(Tiger128EncodeNormalCase, TigerTest)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "6D12A41E72E644F017B6F0E2F7B44C62";

       compare(hash, T1->encode(clear_text.toBytes()).toHexString());
    }

    TEST(Tiger128EncodeEmptyText, TigerTest)
    {
       const StringTest hash = "3293AC630C13F0245F92BBB1766E1616";

       compare(hash, T1->encode({}).toHexString());
    }

    TEST(Tiger160EncodeNormalCase, TigerTest)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "6D12A41E72E644F017B6F0E2F7B44C6285F06DD5";

       compare(hash, T2->encode(clear_text.toBytes()).toHexString());
    }

    TEST(Tiger160EncodeEmptyText, TigerTest)
    {
       const StringTest hash = "3293AC630C13F0245F92BBB1766E16167A4E5849";

       compare(hash, T2->encode({}).toHexString());
    }

    TEST(Tiger192EncodeNormalCase, TigerTest)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "6D12A41E72E644F017B6F0E2F7B44C6285F06DD5D2C5B075";

       compare(hash, T3->encode(clear_text.toBytes()).toHexString());
    }

    TEST(Tiger192EncodeEmptyText, TigerTest)
    {
       const StringTest hash = "3293AC630C13F0245F92BBB1766E16167A4E58492DDE73F3";

       compare(hash, T3->encode({}).toHexString());
    }

    TEST(Tiger2_192EncodeNormalCase, TigerTest)
    {
       const StringTest clear_text = "The quick brown fox jumps over the lazy dog";
       const StringTest hash = "976ABFF8062A2E9DCEA3A1ACE966ED9C19CB85558B4976D8";

       compare(hash, Tiger2->encode(clear_text.toBytes()).toHexString());
    }

    TEST(Tiger2_192EncodeEmptyText, TigerTest)
    {
       const StringTest hash = "4441BE75F6018773C206C22745374B924AA8313FEF919F41";

       compare(hash, Tiger2->encode({}).toHexString());
    }

    TEST(Tiger192EncodeHMACNormalCase, TigerTest)
    {
       const StringTest clear_text = "message digest";
       const StringTest hash = "381B28645F3AFA6C509D207DC1FCB00680C90D3A290B4391";
       const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
       0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
       0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

       compare(hash, T3->hmacEncode(key, clear_text.toBytes()).toHexString());
    }

    TEST(Tiger192EncodeHMACEmptyText, TigerTest)
    {
       const StringTest hash = "2FA999C02ABA3644471B5A7CCF542BF61827E8F53BEA0195";
       const BytesVector key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
       0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
       0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

       compare(hash, T3->hmacEncode(key, {}).toHexString());
    }
}

#endif