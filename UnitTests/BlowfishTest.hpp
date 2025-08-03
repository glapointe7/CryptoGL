/*
 * Test vectors taken here : http://www.schneier.com/code/vectors.txt
 */
#pragma once

#include "Test.hpp"
#include "../src/Blowfish.hpp"

namespace UnitTests
{
    class BlowfishTest : public Test
    {
    protected:
       Blowfish *B, *CBC, *CFB, *OFB, *CTR;

       void setUp() override
       {
          B = new Blowfish({0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00});

          CBC = new Blowfish({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87}, OperationModes::CBC,
                  {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10});

          CFB = new Blowfish({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87}, OperationModes::CFB,
                  {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10});

          OFB = new Blowfish({0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xF0, 0xE1, 0xD2, 0xC3, 0xB4, 0xA5, 0x96, 0x87}, OperationModes::OFB,
                  {0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10});

          CTR = new Blowfish(StringTest("603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4").hexToBytes(), 
                  OperationModes::CTR, {0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF});
       }

       void tearDown() override
       {
          delete B;
          delete CBC;
          delete CFB;
          delete OFB;
          delete CTR;
       }
    };

    TEST(BlowfishECBEncode, BlowfishTest)
    {
       const BytesVector clear_text = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

       compare("7D856F9A613063F2", B->encode(clear_text));
    }

    TEST(BlowfishECBDecode, BlowfishTest)
    {
       const BytesVector clear_text = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
       const StringTest message = "7D856F9A613063F2";

       compare(clear_text, B->decode(message.hexToBytes()));
    }

    TEST(BlowfishCBCEncode, BlowfishTest)
    {
       const BytesVector clear_text = {0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x20, 0x4E, 0x6F, 
       0x77, 0x20, 0x69, 0x73, 0x20, 0x74, 0x68, 0x65, 0x20, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x00};

       compare("6B77B4D63006DEE605B156E27403979358DEB9E7154616D959F1652BD5FF92CC", CBC->encode(clear_text));
    }

    TEST(BlowfishCBCDecode, BlowfishTest)
    {   
       const StringTest message = "6B77B4D63006DEE605B156E27403979358DEB9E7154616D959F1652BD5FF92CC";
       compare("37363534333231204E6F77206973207468652074696D6520666F722000000000", 
               CBC->decode(message.hexToBytes()));
    }

    TEST(BlowfishCFBEncode, BlowfishTest)
    {
       const BytesVector clear_text = {0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x20, 0x4E, 0x6F, 
       0x77, 0x20, 0x69, 0x73, 0x20, 0x74, 0x68, 0x65, 0x20, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x00};

       compare("E73214A2822139CAF26ECF6D2EB9E76E3DA3DE04D1517200519D57A6C3384ECE", CFB->encode(clear_text));
    }

    TEST(BlowfishCFBDecode, BlowfishTest)
    {   
       const StringTest message = "E73214A2822139CAF26ECF6D2EB9E76E3DA3DE04D1517200519D57A6C3384ECE";
       compare("37363534333231204E6F77206973207468652074696D6520666F722000000000", 
               CFB->decode(message.hexToBytes()));
    }

    TEST(BlowfishOFBEncode, BlowfishTest)
    {
       const BytesVector clear_text = {0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x20, 0x4E, 0x6F, 
       0x77, 0x20, 0x69, 0x73, 0x20, 0x74, 0x68, 0x65, 0x20, 0x74, 0x69, 0x6D, 0x65, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x00};

       compare("E73214A2822139CA62B343CC5B65587310DD908D0C241B2263C2CF80DA46FBB8", OFB->encode(clear_text));
    }

    TEST(BlowfishOFBDecode, BlowfishTest)
    {   
       const StringTest message = "E73214A2822139CA62B343CC5B65587310DD908D0C241B2263C2CF80DA46FBB8";
       compare("37363534333231204E6F77206973207468652074696D6520666F722000000000", 
               OFB->decode(message.hexToBytes()));
    }

    TEST(BlowfishCTREncode, BlowfishTest)
    {
       const StringTest clear_text("6BC1BEE22E409F96E93D7E117393172AAE2D8A571E03AC9C9EB76F"
       "AC45AF8E5130C81C46A35CE411E5FBC1191A0A52EFF69F2445DF4F9B17AD2B417BE66C3710");

       compare("EF2CA093ECC80BD87DDC11D06DBCD38B3B5205EEDCCF58945B192755945003FC44B144F5DC1ACC5F14EACCA935125879FEED0C53019712B1B07438BA4C19C864", 
               CTR->encode(clear_text.hexToBytes()));
    }

    TEST(BlowfishCTRDecode, BlowfishTest)
    {   
       const StringTest clear_text("EF2CA093ECC80BD87DDC11D06DBCD38B3"
               "B5205EEDCCF58945B192755945003FC44B144F5DC1ACC5F14EACCA935125879FEED0C53019712B1B07438BA4C19C864");
       compare("6BC1BEE22E409F96E93D7E117393172AAE2D8A571E03AC9C9EB76FAC45AF8E5130C81C46A35CE411E5FBC1191A0A52EFF69F2445DF4F9B17AD2B417BE66C3710", 
               CTR->decode(clear_text.hexToBytes()));
    }
}