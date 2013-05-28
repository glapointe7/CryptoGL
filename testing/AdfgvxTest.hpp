#ifndef ADFGVXTEST_HPP
#define ADFGVXTEST_HPP

#include <gtest/gtest.h>
#include "../src/Adfgvx.hpp"

class AdfgvxTest : public ::testing::Test
{
protected:
   Adfgvx *A;

   virtual void SetUp()
   {
      A = new Adfgvx("GABRIEL");
   }

   virtual void TearDown()
   {
      delete A;
   }
};

TEST_F(AdfgvxTest, encode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
   const std::vector<std::string> grid_key = {"C1OFWJ", "YMT5B4", "I7A28S", "P30QHX", "KEUL6D", "VRGZN9"};

   A->setGridKey(grid_key);
   EXPECT_EQ("XDGFAVAFGAFDVVDAXVFFAXFFFFDDAFFFDFADVXVDDXXXGFGFGVVFAXDADDFDDAXAFXFFVAFFAVVDFFX"
           "AVVFGVDFXVVVDGFXAGFDAXVADXVVXDXVAGFVFXXXDFFFFDFVFVFFXFXAXFDFFFXDFFXDVFDFVFX", A->encode(clear_text));
}

TEST_F(AdfgvxTest, decode)
{
   const std::string clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS999";
   const std::vector<std::string> grid_key = {"C1OFWJ", "YMT5B4", "I7A28S", "P30QHX", "KEUL6D", "VRGZN9"};

   A->setGridKey(grid_key);
   EXPECT_EQ(clear_text, A->decode("XDGFAVAFGAFDVVDAXVFFAXFFFFDDAFFFDFADVXVDDXXXGFGFGVVFAXDADDFDDAXAFXFFVAFFAVVDFFX"
           "AVVFGVDFXVVVDGFXAGFDAXVADXVVXDXVAGFVFXXXDFFFFDFVFVFFXFXAXFDFFFXDFFXDVFDFVFX"));
}

#endif