
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
#include "../testing/NoekeonTest.hpp"
#include "../testing/CAST128Test.hpp"
#include "../testing/CAST256Test.hpp"
#include "../testing/TwofishTest.hpp"
#include "../testing/HellmanMerkleKnapsackTest.hpp"
#include "../testing/CamelliaTest.hpp"
#include "../testing/Mysty1Test.hpp"
#include "../testing/SEALTest.hpp"
#include "../testing/HC256Test.hpp"
#include "../testing/ScreamTest.hpp"
//#include "../testing/SerpentTest.hpp"
#include "../testing/Salsa20Test.hpp"

//#include <sys/time.h>

int main(int argc, char **argv)
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();

   /*struct timeval tim;
   gettimeofday(&tim, NULL);
   const double t1 = tim.tv_sec + (tim.tv_usec / 1000000.0);
   
   gettimeofday(&tim, NULL);
   const double t2 = tim.tv_sec + (tim.tv_usec / 1000000.0);
   std::cout << "Time elapsed: " << t2 - t1 << " seconds";*/
}