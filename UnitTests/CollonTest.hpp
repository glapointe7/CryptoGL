#pragma once

#include "Test.hpp"
#include "../src/Collon.hpp"

namespace UnitTests
{
    class CollonTest : public Test
    {
    protected:
       Collon *C;

       void setUp() override
       {
          C = new Collon("TESTING");
       }

       void tearDown() override
       {
          delete C;
       }
    };

    TEST(CollonEncode, CollonTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare("TGFUTXVZUUTTFTFZYZYZMGMGTXVZVYTMUTTUYUVUUGMTTUYVZZGTTFGVYXZVMMMGMXZVUZGMMGTVUUV"
              "UTMTMMYVZVZTTTTTYVZUVTMGFTVVXXVTTUMFUXUZZTGMUTVXVUUGTTGMZVXZVTGTTYUUX", C->encode(clear_text));
    }

    TEST(CollonDecode, CollonTest) 
    {
      const StringTest clear_text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";

      compare(clear_text, C->decode("TGFUTXVZUUTTFTFZYZYZMGMGTXVZVYTMUTTUYUVUUGMTTUYVZZGTTFGVYXZV"
              "MMMGMXZVUZGMMGTVUUVUTMTMMYVZVZTTTTTYVZUVTMGFTVVXXVTTUMFUXUZZTGMUTVXVUUGTTGMZVXZVTGTTYUUX"));
    }
}