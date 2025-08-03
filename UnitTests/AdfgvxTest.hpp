#pragma once

#include "Test.hpp"
#include "../src/Adfgvx.hpp"

namespace UnitTests
{
    class AdfgvxTest : public Test
    {
    protected:
       Adfgvx *A;

       void setUp() override
       {
          A = new Adfgvx("GABRIEL", "C1OFWJYMT5B4I7A28SP30QHXKEUL6DVRGZN9");
       }

       void tearDown() override
       {
          delete A;
       }
    };

    TEST(AdfgvxEncode, AdfgvxTest)
    {
       const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
       const ClassicalType cipher_text = "XDGFAVAFGAFDVVDAXVFFAXFFFFDDAFFFDFADVXVDDXXXGFGFGVVFAXDADDFDDAXAFXFFVAFFAVVDFFX"
                                         "AVVFGVDFXVVVDGFXAGFDAXVADXVVXDXVAGFVFXXXDFFFFDFVFVFFXFXAXFDFFFXDFFXDVFDFVFX";

       compare(cipher_text, A->encode(clear_text));
    }

    TEST(AdfgvxDecode, AdfgvxTest)
    {
       const ClassicalType clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS999";

       compare(clear_text, A->decode("XDGFAVAFGAFDVVDAXVFFAXFFFFDDAFFFDFADVXVDDXXXGFGFGVVFAXDADDFDDAXAFXFFVAFFAVVDFFX"
               "AVVFGVDFXVVVDGFXAGFDAXVADXVVXDXVAGFVFXXXDFFFFDFVFVFFXFXAXFDFFFXDFFXDVFDFVFX"));
    }
}