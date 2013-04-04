#include "Delastelle.hpp"
//#include "Railfence.h"
//#include "Playfair.h"
//#include "Polybe.h"
//#include "Wolseley.h"
//#include "Cesar.h"
//#include "StringCipher.h"
//#include "Tools.h"
//#include "converterTools.h"
//#include "Morse.h"
#include <iostream>

using namespace std;

int main()
{
   Delastelle *D = new Delastelle();
   D->setBlockLength(5);
   D->setKey("DIMANCHE");
   D->setGridDimension(5);
   
   //string text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
   //string text = "POSSNDGOBBPPDRQSVTCOVGDNZDRABSSKPOMDDSKTMMQZXIAHOXEEEYESYOPLHORTNIPGLNMSVNR";
   string text = D->load(R"(../GIT_CryptoCode/cipher_text.txt)");
   cout << D->decode(text);
   
   //Playfair *P = new Playfair();
   //P->setKey("MAISON");
   //cout << P->encode();

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
