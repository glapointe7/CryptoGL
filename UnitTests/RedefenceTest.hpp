
#ifndef REDEFENCETEST_HPP
#define	REDEFENCETEST_HPP

#include "Test.hpp"
#include "../src/Railfence.hpp"

namespace UnitTests
{
    class RedefenceTest : public Test
    {
    protected:
       Redefence *R;

       void setUp() override
       {
          R = new Redefence({4,2,5,0,3,1});
       }

       void tearDown() override
       {
          delete R;
       }
    };

    TEST(RedefenceEncodeWithNoOffset, RedefenceTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
    // "SPUPIEEI ALATCARTOEOLBOG LIREOLOANTBRODT ULAUNSGMONJUUSS TIIQNIRMREESTE NTAAITD"
       compare(StringTest("TIIQNIRMREESTELIREOLOANTBRODTNTAAITDSPUPIEEIULAUNSGMONJUUSSALATCARTOEOLBOG"), R->encode(clear_text));
    }

    TEST(RedefenceDecodeWithNoOffset, RedefenceTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       compare(clear_text, R->decode("TIIQNIRMREESTELIREOLOANTBRODTNTAAITDSPUPIEEIULAUNSGMONJUUSSALATCARTOEOLBOG"));
    }

    TEST(RedefenceEncodeWithOffset, RedefenceTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       R->setOffset(4);
       // IQIMESE NLTUATAMINTUDS TIIENLRARTERTD ULATNAGTOEJLUOS SLPRUOPOINEBEOIT AACROOBG
       compare(StringTest("SLPRUOPOINEBEOITTIIENLRARTERTDAACROOBGIQIMESEULATNAGTOEJLUOSNLTUASAMINTUDS"), R->encode(clear_text));
    }

    TEST(RedefenceDecodeWithOffset, RedefenceTest)
    {
       const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

       R->setOffset(4);
       compare(clear_text, R->decode("SLPRUOPOINEBEOITTIIENLRARTERTDAACROOBGIQIMESEULATNAGTOEJLUOSNLTUASAMINTUDS"));
    }
}

#endif