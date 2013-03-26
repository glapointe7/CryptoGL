#include "Playfair.h"
//#include "Polybe.h"
//#include "Wolseley.h"
//#include "Cesar.h"
//#include "StringCipher.h"
//#include "Tools.h"
//#include "converterTools.h"
//#include "clsMorse.h"
#include <iostream>

int main()
{   
    Playfair *P = new Playfair();
    P->setKey("MAISON");
    StringCipher *sc_c = P;
    
    cout << sc_c->encode();
    
    
    //string key = "ABCDEEFDABE";
    //string text = "iI PArait Que TU vEUX que Je tEstE kpTa1 aVEc mA pRoPre cLE";
    //string symbol = "01";
    //replaceChar(text, 'a', 'A');
    //convertMajMinToSymbol(text, symbol);
    //removeRepeatedLetters(key);
    //cout << key;
    //cout << convertTextToBinaryString(key);
    /*vector<uint_fast32_t> seq = {3, 5, 15, 25, 54, 110, 225};
    const uint_fast32_t nb = 10;
    const uint_fast32_t modulo = 439;
    Hellman H("plain.txt", "crypted.txt");
    H.encode(modulo, nb, seq);*/

}