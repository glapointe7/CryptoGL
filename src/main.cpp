
#include <gtest/gtest.h>

#include "../testing/CaesarTest.hpp"
#include "../testing/VigenereTest.hpp"
#include "../testing/VigenereMultTest.hpp"
#include "../testing/RozierTest.hpp"
#include "../testing/BeaufortTest.hpp"
#include "../testing/BeaufortGermanTest.hpp"
#include "../testing/GronsfeldTest.hpp"
#include "../testing/PlayfairTest.hpp"
#include "../testing/PolybeTest.hpp"
#include "../testing/CollonTest.hpp"
#include "../testing/AdfgvxTest.hpp"
#include "../testing/DelastelleTest.hpp"
#include "../testing/RailfenceTest.hpp"
#include "../testing/RedefenceTest.hpp"
#include "../testing/RedefenceZigzagTest.hpp"
#include "../testing/WolseleyTest.hpp"
#include "../testing/SHA1Test.hpp"
#include "../testing/SHA224Test.hpp"
#include "../testing/SHA256Test.hpp"
#include "../testing/SHA384Test.hpp"
#include "../testing/SHA512Test.hpp"
#include "../testing/SHA512_224Test.hpp"
#include "../testing/SHA512_256Test.hpp"
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
#include "../testing/RabbitTest.hpp"
#include "../testing/MD2Test.hpp"
#include "../testing/MD4Test.hpp"
#include "../testing/MD5Test.hpp"
#include "../testing/TripleDESTest.hpp"
#include "../testing/BlowfishTest.hpp"
#include "../testing/FleissnerTest.hpp"
#include "../testing/Base64Test.hpp"
#include "../testing/NihilistesTest.hpp"
#include "../testing/ChaoTest.hpp"
#include "../testing/TwoSquaresTest.hpp"
#include "../testing/FourSquaresTest.hpp"
#include "../testing/Ripemd128Test.hpp"
#include "../testing/Ripemd160Test.hpp"
#include "../testing/Ripemd256Test.hpp"
#include "../testing/Ripemd320Test.hpp"
#include "../testing/AESTest.hpp"
#include "../testing/TigerTest.hpp"
#include "../testing/IDEATest.hpp"
#include "../testing/SkipjackTest.hpp"
#include "../testing/RC2Test.hpp"
#include "../testing/RC5Test.hpp"
#include "../testing/RC6Test.hpp"
#include "../testing/XTEATest.hpp"
#include "../testing/KeccakTest.hpp"
#include "../testing/WhirlpoolTest.hpp"
#include "../testing/BlakeTest.hpp"

#include <sys/time.h>

int main(int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();

   /*struct timeval tim;
   gettimeofday(&tim, NULL);
   const double t1 = tim.tv_sec + (tim.tv_usec / 1000000.0);
   
   //const std::vector<std::string> grid_key = {"CLOFWJ", "YMT5B4", "I7A28S", "P30QHX", "KEUL6D", "VRGZN9"};
   
   Railfence *A = new Railfence(25);
   A->setAlpha(String::uppercase_digits);
   A->setOffset(4);
   A->encodeFile(R"(../GIT_CryptoCode/testing/TestFiles/clear_text.txt)",
           R"(../GIT_CryptoCode/testing/TestFiles/cipher_text.txt)");
   //C->decodeFile(R"(../GIT_CryptoCode/testing/TestFiles/cipher_text.txt)",
   //        R"(../GIT_CryptoCode/testing/TestFiles/clear_text2.txt)");
   delete A;
   
   gettimeofday(&tim, NULL);
   const double t2 = tim.tv_sec + (tim.tv_usec / 1000000.0);
   std::cout << "Time elapsed: " << t2 - t1 << " seconds";*/

   /*const std::string pi = "141592653589793238462643383279502884197169399375105820974944"
           "59230781640628620899862803482534211706798214808651328230664709384460955058223172"
           "53594081284811174502841027019385211055596446229489549303819644288109756659334461"
           "28475648233786783165271201909145648566923460348610454326648213393607260249141273"
           "72458700660631558817488152092096282925409171536436789259036001133053054882046652"
           "13841469519415116094330572703657595919530921861173819326117931051185480744623799"
           "62749567351885752724891227938183011949129833673362440656643086021394946395224737"
           "19070217986094370277053921717629317675238467481846766940513200056812714526356082"
           "77857713427577896091736371787214684409012249534301465495853710507922796892589235"
           "42019956112129021960864034418159813629774771309960518707211349999998372978049951"
           "05973173281609631859502445945534690830264252230825334468503526193118817101000313"
           "78387528865875332083814206171776691473035982534904287554687311595628638823537875"
           "93751957781857780532171226806613001927876611195909216420198938095257201065485863"
           "278865936153381827968230301952";

   Gronsfeld *G = new Gronsfeld();
   std::string cipher_text(G->load(R"(../GIT_CryptoCode/src/attacks/mtc3-esslinger-06-onetimepad-cipher.txt)"));
   G->eraseBadCharacters(cipher_text);
   const uint32_t cipher_len = cipher_text.length();
   const uint32_t double_len = cipher_len << 1;
   
   std::ofstream out(R"(../GIT_CryptoCode/src/attacks/Gronsfeld_attack.txt)");
   
   for(uint8_t offset = 1; offset < 100; ++offset)
   {
      std::vector<int32_t> key;
      key.reserve(cipher_len);
      for(uint16_t i = 0; i < double_len; i += 2)
      {
         const int32_t number = atoi(pi.substr(offset + i, 2).c_str());
         key.push_back(number);
      }
      
      G->setKey(key);
      out << G->decode(cipher_text) << "\n";
   }
   out.close();*/

   /*TranspositionAttack *TA = new TranspositionAttack();
   TA->setFilename(R"(../GIT_CryptoCode/src/attacks/TranspoRowsAttack.txt)");
   TA->setKeyLength(4);
   TA->setAlpha(String::letters + "-");
   std::string cipher_text(TA->load(R"(../GIT_CryptoCode/src/attacks/mtc3-esslinger-16-cipher-en.txt)"));
   TA->attack(cipher_text);*/

   /*CaesarAttack *CA = new CaesarAttack();
   CA->setFilename(R"(../GIT_CryptoCode/src/attacks/caesar_attack_dwarf.txt)");
   CA->setAlpha(String::letters);
   std::string cipher_text("dlsjvtlAvPumvythApvuZljBypAFDpAopuAolZuvD");
   CA->attack(cipher_text);*/

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