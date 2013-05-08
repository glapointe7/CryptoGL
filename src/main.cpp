/*
 * .
 */
#include <gtest/gtest.h>

#include "../testing/CaesarTest.hpp"
#include "../testing/VigenereTest.hpp"
#include "../testing/RozierTest.hpp"
#include "../testing/BeaufortTest.hpp"
#include "../testing/BeaufortGermanTest.hpp"
#include "../testing/PlayfairTest.hpp"
#include "../testing/PolybeTest.hpp"
#include "../testing/CollonTest.hpp"
#include "../testing/AdfgvxTest.hpp"
#include "../testing/DelastelleTest.hpp"
#include "../testing/RailfenceTest.hpp"
#include "../testing/WolseleyTest.hpp"
#include "../testing/SHA1Test.hpp"
#include "../testing/HellmanTest.hpp"
#include "../testing/TranspositionRowsTest.hpp"
#include "../testing/TranspositionColumnsTest.hpp"
#include "../testing/TranspositionDoubleTest.hpp"
#include "../testing/MorseTest.hpp"
#include "../testing/UnsortAlphaTest.hpp"
#include "../testing/DESTest.hpp"
#include "../testing/RC4Test.hpp"
#include "../testing/HillTest.hpp"
#include "../testing/AffineTest.hpp"

//#include "attacks/CaesarAttack.hpp"
#include "Affine.hpp"

// int argc, char **argv
int main()
{
   /*CaesarAttack *CA = new CaesarAttack();
   CA->setFilename(R"(../GIT_CryptoCode/src/attacks/caesar_attack2.txt)");
   CA->setAlpha(String::lower_ordered_alpha_numeric);
   std::string cipher_text(CA->load(R"(../GIT_CryptoCode/src/attacks/mtc3-esslinger-05-caesar.txt)"));
   CA->attack(cipher_text);*/
   
   //::testing::InitGoogleTest(&argc, argv);
   //return RUN_ALL_TESTS();
   
   /*Matrix M;
   M.setModulo(29);
   
   const std::vector<std::vector<int32_t> > Mat = {{1,3,5},
                                                   {0,3,7},
                                                   {4,2,6}};
   M.setMatrix(Mat);
   std::cout << "det(M) = " << Matrix::det(M) << "\n\n";
   
   try
   {
      Matrix M_inv = Matrix::inverse(M);
      Matrix::display(M_inv);
   }
   catch(BadMatrix &BM)
   {
      std::cout << BM.what() << "\n\n";
   }*/
   
   //std::cout << getModInverse(28, 29);
   //const std::string text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
   /*const std::string text = "XFADDFDAVXAFDFVAFFDFDFDFFFFDAFGFFFDDDDAFDDVFDVFXAXAXFFFVVXFFAFFAXXVAFAAVVXXDVAFAFFVAFFFFFDVDXXDFXXFXFVFAXDVAXFGXDDXFDAVVVVVADFDAGFGDAFAXXDFAVVDDXDDFFF";
   const std::string key = "MARCEL";
   const std::vector<std::string> grid_key = {"CLOFWJ", "YMT5B4", "I7A28S", "P30QHX", "KEUL6D", "VRGZN9"};
   
   Adfgvx *A = new Adfgvx();
   A->setKey(key);
   A->setGridKey(grid_key);
   std::cout << A->decode(text);
    */
   /*const std::string data = "abc";
   SHA1 *sha1 = new SHA1();
   std::cout << SHA1::hexDigest(sha1->encode(SHA1::getBytesFromString(data)));
    */
   /*const std::string data = "abc";
   MD5 *md5 = new MD5();
   std::cout << MD5::hexDigest(md5->encode(MD5::getBytesFromString(data)));
    */
   /*const std::vector<unsigned char> data = {
      0x59, 0x6F, 0x75, 0x72, 0x20, 0x6C, 0x69, 0x70, 0x73, 0x20, 0x61, 0x72, 0x65, 0x20, 0x73, 0x6D, 0x6F,
      0x6F, 0x74, 0x68, 0x65, 0x72, 0x20, 0x74, 0x68, 0x61, 0x6E, 0x20, 0x76, 0x61, 0x73, 0x65,
      0x6C, 0x69, 0x6E, 0x65, 0x0D, 0x0A
   };
   
   const std::vector<unsigned char> key = {0x0E, 0x32, 0x92, 0x32, 0xEA, 0x6D, 0x0D, 0x73};*/

   /*const std::vector<unsigned char> data = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
   const std::vector<unsigned char> key = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};
   DES *D = new DES();
   D->setKey(key);
   std::cout << DES::hexDigest(D->encode(data));*/

   /*const std::string text = "400 9 14 1 324 266 5 209 400 5 28 2 18 322 1 220 20 18 28 1 54 112 5 165 280 4 18 18 18 126 20";
   //const std::string text = "TIGARSESTENBAWATARNACHEONDIRAIT";
   VigenereMult *v = new VigenereMult();
   v->setKey("TABARNAK");
   cout << v->decode(text);*/

   /*const std::vector<unsigned char> key = {0x91, 0x28, 0x13, 0x29, 0x2E, 0x3D, 0x36, 0xFE,
      0x3B, 0xFC, 0x62, 0xF1, 0xDC, 0x51, 0xC3, 0xAC
   };

   const std::vector<unsigned char> data = {0x3D, 0x2D, 0xF3, 0xC8, 0x3E, 0xF6, 0x27,
      0xA1, 0xE9, 0x7F, 0xC3, 0x84, 0x87, 0xE2, 0x51, 0x9C, 0xF5, 0x76, 0xCD, 0x61,
      0xF4, 0x40, 0x5B, 0x88, 0x96, 0xBF, 0x53, 0xAA, 0x85, 0x54, 0xFC, 0x19, 0xE5,
      0x54, 0x74, 0x73, 0xFB, 0xDB, 0x43, 0x50, 0x8A, 0xE5, 0x3B, 0x20, 0x20, 0x4D,
      0x4C, 0x5E
   };

   Rabbit *R = new Rabbit();
   R->setKey(key);
   std::vector<unsigned char> enc(R->encode(data));
   std::cout << Rabbit::hexDigest(enc) << "\n";
   R->reset();
   std::cout << Rabbit::hexDigest(R->decode(enc));
    */
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
}