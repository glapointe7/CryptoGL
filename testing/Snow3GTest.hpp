/*
 * Source : http://www.gsma.com/technicalprojects/wp-content/uploads/2012/04/Doc3-UEA2-UIA2-Spec-Implementors-Test-Data.pdf
 */
#ifndef SNOW3GTEST_HPP
#define SNOW3GTEST_HPP

#include <gtest/gtest.h>
#include "../src/Snow3G.hpp"
#include "../src/Digest.hpp"

class Snow3GTest : public ::testing::Test
{
protected:
   Snow3G *S, *S2;
   virtual void SetUp()
   {
      S = new Snow3G({0x2B, 0xD6, 0x45, 0x9F, 0x82, 0xC5, 0xB3, 0x00, 0x95, 0x2C, 0x49, 0x10, 0x48, 0x81, 0xFF, 0x48},
                     {0xEA, 0x02, 0x47, 0x14, 0xAD, 0x5C, 0x4D, 0x84, 0xDF, 0x1F, 0x9B, 0x25, 0x1C, 0x0B, 0xF4, 0x5F},
                     2);

      S2 = new Snow3G({0x8C, 0xE3, 0x3E, 0x2C, 0xC3, 0xC0, 0xB5, 0xFC, 0x1F, 0x3D, 0xE8, 0xA6, 0xDC, 0x66, 0xB1, 0xF3},
                     {0xD3, 0xC5, 0xD5, 0x92, 0x32, 0x7F, 0xB1, 0x1C, 0xDE, 0x55, 0x19, 0x88, 0xCE, 0xB2, 0xF9, 0xB7},
                     2);
   }
   virtual void TearDown()
   {
      delete S;
      delete S2;
   }
};

TEST_F(Snow3GTest, generateSet1)
{
   EXPECT_EQ("ABEE97047AC31373", Digest::hexDigest(S->generateKeystream()));
}

TEST_F(Snow3GTest, generateSet2)
{
   EXPECT_EQ("EFF8A342F751480F", Digest::hexDigest(S2->generateKeystream()));
}

#endif