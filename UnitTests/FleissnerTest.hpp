
#ifndef FLEISSNERTEST_HPP
#define	FLEISSNERTEST_HPP

#include "Test.hpp"
#include "../src/Fleissner.hpp"

namespace UnitTests
{
    class FleissnerTest : public Test
    {
    protected:
        Fleissner *F;
        void setUp() override
        {
            F = new Fleissner({
                Coordinates(0u, 1u),
                Coordinates(0u, 3u),
                Coordinates(0u, 5u),
                Coordinates(1u, 4u),
                Coordinates(2u, 2u),
                Coordinates(3u, 1u),
                Coordinates(3u, 4u),
                Coordinates(4u, 5u),
                Coordinates(5u, 3u)
            }, 6u, true);
        }
        
        void tearDown() override
        {
            delete F;
        }
    };
    
    TEST(FleissnerEncode, FleissnerTest)
    {
        const StringTest clear_text = "JECHIFFRECETEXTEAVECUNCACHETOURNANT";

        compare("TJEEOCCUCRHENUITNAEFNCFXTATAERCAHEEV", F->encode(clear_text));
    }
    
    TEST(FleissnerDecode, FleissnerTest)
    {
        const StringTest clear_text = "JECHIFFRECETEXTEAVECUNCACHETOURNANTA";

        compare(clear_text, F->decode("TJEEOCCUCRHENUITNAEFNCFXTATAERCAHEEV"));
    }
}

#endif