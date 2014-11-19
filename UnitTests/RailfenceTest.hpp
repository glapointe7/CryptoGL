#ifndef RAILFENCETEST_HPP
#define RAILFENCETEST_HPP

#include "Test.hpp"
#include "../src/Railfence.hpp"

namespace UnitTests
{
    class RailfenceTest : public Test
    {
    protected:
       Railfence *R;

       void setUp() override
       {
          R = new Railfence(6);
       }

       void tearDown() override
       {
          delete R;
       }
    };

    TEST(RailfenceEncodeNormalCase, RailfenceTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       compare("SPUPIEEIALATCARTOEOLBOGLIREOLOANTBRODTULAUNSGMONJUUSSTIIQNIRMREESTENTAAITD", R->encode(clear_text));
    }

    TEST(RailfenceDecodeNormalCase, RailfenceTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       compare(clear_text, R->decode("SPUPIEEIALATCARTOEOLBOGLIREOLOANTBRODTULAUNSGMONJUUSSTIIQNIRMREESTENTAAITD"));
    }

    TEST(RailfenceEncodeWithOffset, RailfenceTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       R->setOffset(4);
       // IQIMESE NLTUATAMINTUDS TIIENLRARTERTD ULATNAGTOEJLUOS SLPRUOPOINEBEOIT AACROOBG
       compare("IQIMESENLTUASAMINTUDSTIIENLRARTERTDULATNAGTOEJLUOSSLPRUOPOINEBEOITAACROOBG", R->encode(clear_text));
    }

    TEST(RailfenceDecodeWithOffset, RailfenceTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       R->setOffset(4);
       compare(clear_text, R->decode("IQIMESENLTUASAMINTUDSTIIENLRARTERTDULATNAGTOEJLUOSSLPRUOPOINEBEOITAACROOBG"));
    }
}

#endif