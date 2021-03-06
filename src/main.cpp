
#include "../UnitTests/TestContainer.hpp"

#include "../UnitTests/Adler32Test.hpp"
#include "../UnitTests/AffineTest.hpp"
#include "../UnitTests/AdfgvxTest.hpp"
#include "../UnitTests/AESTest.hpp"
#include "../UnitTests/Base64Test.hpp"
#include "../UnitTests/BeaufortGermanTest.hpp"
#include "../UnitTests/BeaufortTest.hpp"
#include "../UnitTests/BlakeTest.hpp"
#include "../UnitTests/BlowfishTest.hpp"
#include "../UnitTests/CAST128Test.hpp"
#include "../UnitTests/CAST256Test.hpp"
#include "../UnitTests/CaesarTest.hpp"
#include "../UnitTests/CamelliaTest.hpp"
#include "../UnitTests/ChaoTest.hpp"
#include "../UnitTests/CollonTest.hpp"
#include "../UnitTests/DESTest.hpp"
#include "../UnitTests/DelastelleTest.hpp"
#include "../UnitTests/FleissnerTest.hpp"
#include "../UnitTests/FourSquaresTest.hpp"
#include "../UnitTests/GronsfeldTest.hpp"
#include "../UnitTests/HC256Test.hpp"
#include "../UnitTests/HellmanMerkleKnapsackTest.hpp"
#include "../UnitTests/HillTest.hpp"
#include "../UnitTests/IDEATest.hpp"
#include "../UnitTests/KeccakTest.hpp"
#include "../UnitTests/LRCTest.hpp"
#include "../UnitTests/MD2Test.hpp"
#include "../UnitTests/MD4Test.hpp"
#include "../UnitTests/MD5Test.hpp"
#include "../UnitTests/MorseTest.hpp"
#include "../UnitTests/Mysty1Test.hpp"
#include "../UnitTests/NihilistesTest.hpp"
#include "../UnitTests/NoekeonTest.hpp"
#include "../UnitTests/PlayfairTest.hpp"
#include "../UnitTests/PolybeTest.hpp"
#include "../UnitTests/RC2Test.hpp"
#include "../UnitTests/RC4Test.hpp"
#include "../UnitTests/RC5Test.hpp"
#include "../UnitTests/RC6Test.hpp"
#include "../UnitTests/RabbitTest.hpp"
#include "../UnitTests/RailfenceTest.hpp"
#include "../UnitTests/RedefenceTest.hpp"
#include "../UnitTests/RedefenceZigzagTest.hpp"
#include "../UnitTests/Ripemd128Test.hpp"
#include "../UnitTests/Ripemd160Test.hpp"
#include "../UnitTests/Ripemd256Test.hpp"
#include "../UnitTests/Ripemd320Test.hpp"
#include "../UnitTests/RozierTest.hpp"
#include "../UnitTests/SEALTest.hpp"
#include "../UnitTests/SHA1Test.hpp"
#include "../UnitTests/SHA224Test.hpp"
#include "../UnitTests/SHA256Test.hpp"
#include "../UnitTests/SHA384Test.hpp"
#include "../UnitTests/SHA512Test.hpp"
#include "../UnitTests/SHA512_224Test.hpp"
#include "../UnitTests/SHA512_256Test.hpp"
#include "../UnitTests/Salsa20Test.hpp"
#include "../UnitTests/ScreamTest.hpp"
#include "../UnitTests/SkipjackTest.hpp"
#include "../UnitTests/Snow3GTest.hpp"
#include "../UnitTests/TigerTest.hpp"
#include "../UnitTests/TranspositionColumnsTest.hpp"
#include "../UnitTests/TranspositionDoubleTest.hpp"
#include "../UnitTests/TranspositionRowsTest.hpp"
#include "../UnitTests/TripleDESTest.hpp"
#include "../UnitTests/TwoSquaresTest.hpp"
#include "../UnitTests/TwofishTest.hpp"
#include "../UnitTests/UnsortAlphaTest.hpp"
#include "../UnitTests/VigenereMultTest.hpp"
#include "../UnitTests/VigenereTest.hpp"
#include "../UnitTests/WhirlpoolTest.hpp"
#include "../UnitTests/WolseleyTest.hpp"
#include "../UnitTests/XTEATest.hpp"
//#include "../testing/SerpentTest.hpp"

#include "../UnitTests/StringMethodsTest.hpp"
#include "../UnitTests/VectorMethodsTest.hpp"


int main(int argc, char **argv)
{   
    UnitTests::TestContainer::runAllTests();
}