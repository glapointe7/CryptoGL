#pragma once

#include "Test.hpp"
#include "../src/Serpent.hpp"

namespace UnitTests
{
    class SerpentTest : public Test
    {
    protected:
        //Serpent *Set2_128bits;, *Set4_Vector0_128bits; 
        //Serpent *Set4_Vector1_128bits;
        //Serpent *Set2_192bits, *Set4_Vector0_192bits;
        //Serpent *Set4_Vector1_192bits;
        Serpent *Set2_256bits;

        void setUp() override
        {
            // 128-bit key
            //Set2_128bits = new Serpent(String("00000000000000000000000000000000").hexToBytes());

            //Set4_Vector0_128bits = new Serpent(String("000102030405060708090A0B0C0D0E0F").hexToBytes());

            //Set4_Vector1_128bits = new Serpent(String("2BD6459F82C5B300952C49104881FF48").hexToBytes());

            // 192-bit key
            /*Set2_192bits = new Serpent(String("000000000000000000000000000000000000000000000000").hexToBytes());

            Set4_Vector0_192bits = new Serpent(String("000102030405060708090A0B0C0D0E0F1011121314151617").hexToBytes());

            Set4_Vector1_192bits = new Serpent(String("2BD6459F82C5B300952C49104881FF482BD6459F82C5B300").hexToBytes());*/
            
            // 256-bit key
            Set2_256bits = new Serpent(String("0000000000000000000000000000000000000000000000000000000000000000").hexToBytes());
            //Set2_256bits = new Serpent(String("00112233445566778899AABBCCDDEEFF").hexToBytes());
        }

        void tearDown() override
        {
            //delete Set2_128bits;
            //delete Set4_Vector0_128bits;
            //delete Set4_Vector1_128bits;
            /*delete Set2_192bits;
            delete Set4_Vector0_192bits;
            delete Set4_Vector1_192bits;*/
            delete Set2_256bits;
        }
    };

    // Test vectors from Serpent reference implementation (ECB mode)
    // Source: https://www.cs.technion.ac.il/~biham/Reports/Serpent/
    /*TEST(SerpentEncodeECB128Bits_Set2_Vector0, SerpentTest)
    {
        const BytesVector clear_text = String("80000000000000000000000000000000").hexToBytes();
        compare("A3B35DE7C358DDD82644678C64B8BCBB", Set2_128bits->encode(clear_text));
    }

    TEST(SerpentEncodeECB128Bits_Set2_Vector1, SerpentTest)
    {
        const BytesVector clear_text = String("40000000000000000000000000000000").hexToBytes();
        compare("04ABCFE4E0AF27FF92A2BB10949D7DD2", Set2_128bits->encode(clear_text));
    }

    TEST(SerpentEncodeECB128Bits_Set3_Vector0, SerpentTest)
    {
        const BytesVector clear_text = String("00000000000000000000000000000000").hexToBytes();
        //compare("3620B17AE6A993D09618B8768266BAE9", Set2_128bits->encode(clear_text));
        compare("90E7A5BA9497FA1BFC00F7D1A3A86A1E", Set2_128bits->encode(clear_text));
    }

    TEST(SerpentDecodeECB128Bits_Set3_Vector0, SerpentTest)
    {
        const BytesVector cipher_text = String("00000000000000000000000000000000").hexToBytes();
        compare("47C6786045BB9D30F4029E7CCCCD1CAE", Set2_128bits->decode(cipher_text));
    }

    TEST(SerpentEncodeECB128Bits_Set4_Vector0, SerpentTest)
    {
        const BytesVector clear_text = String("00112233445566778899AABBCCDDEEFF").hexToBytes();
        compare("563E2CF8740A27C164804560391E9B27", Set4_Vector0_128bits->encode(clear_text));
    }

    TEST(SerpentEncodeECB128Bits_Set4_Vector1, SerpentTest)
    {
        const BytesVector clear_text = String("EA024714AD5C4D84EA024714AD5C4D84").hexToBytes();
        compare("92D7F8EF2C36C53409F275902F06539F", Set4_Vector1_128bits->encode(clear_text));
    }*/



    /*TEST(SerpentEncodeECB192Bits_Set2_Vector0, SerpentTest)
    {
        const BytesVector clear_text = String("80000000000000000000000000000000").hexToBytes();
        compare("23F5F432AD687E0D4574C16459618ABB", Set2_192bits->encode(clear_text));
    }

    TEST(SerpentEncodeECB192Bits_Set2_Vector1, SerpentTest)
    {
        const BytesVector clear_text = String("40000000000000000000000000000000").hexToBytes();
        compare("56CD894936F6E9A4A4304CAE06F97CE3", Set2_192bits->encode(clear_text));
    }

    TEST(SerpentEncodeECB192Bits_Set4_Vector0, SerpentTest)
    {
        const BytesVector clear_text = String("00112233445566778899AABBCCDDEEFF").hexToBytes();
        compare("6AB816C82DE53B93005008AFA2246A02", Set4_Vector0_192bits->encode(clear_text));
    }

    TEST(SerpentEncodeECB192Bits_Set4_Vector1, SerpentTest)
    {
        const BytesVector clear_text = String("EA024714AD5C4D84EA024714AD5C4D84").hexToBytes();
        compare("827B18C2678A239DFC5512842000E204", Set4_Vector1_192bits->encode(clear_text));
    }*/



    TEST(SerpentEncodeECB256Bits_Set2, SerpentTest)
    {
        const BytesVector clear_text = String("00000000000000000000000000000000").hexToBytes();
        compare("49672BA898D98DF95019180445491089", Set2_256bits->encode(clear_text));
    }
}