
//#include "Rabbit.hpp"
//#include "FrequenciesAnalysis.hpp"
//#include "RC4.hpp"
//#include "Delastelle.hpp"
//#include "Matrix.hpp"
//#include "Railfence.h"
//#include "Playfair.h"
//#include "Polybe.h"
#include "Vigenere.hpp"
//#include "Wolseley.h"
//#include "StringCipher.h"
//#include "Tools.h"
//#include "converterTools.h"
//#include "Morse.h"

#include <iostream>
//#include <vector>

using namespace std;

int main()
{
   const std::string text = "400 9 14 1 324 266 5 209 400 5 28 2 18 322 1 220 20 18 28 1 54 112 5 165 280 4 18 18 18 126 20";
   //const std::string text = "TIGARSESTENBAWATARNACHEONDIRAIT";
   VigenereMult *v = new VigenereMult();
   v->setKey("TABARNAK");
   cout << v->decode(text);
   
   /*const std::vector<unsigned char> key = {0xC2, 0x1F, 0xCF, 0x38, 0x81, 0xCD, 0x5E,
      0xE8, 0x62, 0x8A, 0xCC, 0xB0, 0xA9, 0x89, 0x0D, 0xF8};

   const std::vector<unsigned char> data = {0x02, 0xF7, 0x4A, 0x1C, 0x26, 0x45, 0x6B, 0xF5, 
   0xEC, 0xD6, 0xA5, 0x36, 0xF0, 0x54, 0x57, 0xB1, 0xA7, 0x8A, 0xC6, 0x89, 0x47, 0x6C, 0x69,
   0x7B, 0x39, 0x0C, 0x9C, 0xC5, 0x15, 0xD8, 0xE8, 0x88, 0xEF, 0x9A, 0x69, 0x71, 0x8B, 0x82,
   0x49, 0xA1, 0xA7, 0x3C, 0x5A, 0x6E, 0x5B, 0x90, 0x45, 0x95};
   
   Rabbit *R = new Rabbit();
   R->setKey(key);
   cout << Rabbit::hexDigest(R->decode(data));*/
   //const std::string text = "POSSNDGOBBPPDRQSVTCOVGDNZDRABSSKPOMDDSKTMMQZXIAHOXEEEYESYOPLHORTNIPGLNMSVNR";
   /*FrequenciesAnalysis *FA = new FrequenciesAnalysis();
   std::string text = FrequenciesAnalysis::load(R"(../GIT_CryptoCode/cipher_text.txt)");
   FA->calculateIC(text);
   cout << FA->getIC();*/

   /*RC4 *R = new RC4();
   R->setKey(R->getBytesFromString("Key"));
   cout << RC4::getStringFromBytes(R->decode(R->encode(R->getBytesFromString("Plaintext"))));*/
   //cout << RC4::hexDigest(R->encode(R->getBytesFromString("Plaintext")));

   /*Delastelle *D = new Delastelle();
   D->setBlockLength(5);
   D->setKey("DIMANCHE");
   D->setGridDimension(5);
   
   const std::string text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
   //const std::string text = "POSSNDGOBBPPDRQSVTCOVGDNZDRABSSKPOMDDSKTMMQZXIAHOXEEEYESYOPLHORTNIPGLNMSVNR";
   //std::string text = D->load(R"(../GIT_CryptoCode/cipher_text.txt)");
   std::cout << D->encode(text);*/

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
