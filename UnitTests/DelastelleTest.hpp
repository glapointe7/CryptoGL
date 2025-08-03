#pragma once

#include "Test.hpp"
#include "../src/Delastelle.hpp"

namespace UnitTests
{
    class DelastelleTest : public Test
    {
    protected:
       Delastelle *D;

       void setUp() override
       {
          D = new Delastelle("ERABLE");
       }

       void tearDown() override
       {
          delete D;
       }
    };

    TEST(DelastelleEncode, DelastelleTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       compare("OLSNLKALLLOEJJISUTILVQIEZAOABXOPRCPAIQSNMMELRIMELUBAEBCSULOHETOOLCPPBCJSUTQ", D->encode(clear_text));
    }

    TEST(DelastelleDecode, DelastelleTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTSX";

       compare(clear_text, D->decode("OLSNLKALLLOEJJISUTILVQIEZAOABXOPRCPAIQSNMMELRIMELUBAEBCSULOHETOOLCPPBCJSUTQ"));
    }
}