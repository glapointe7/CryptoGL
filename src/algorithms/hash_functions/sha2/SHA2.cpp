#include "SHA2.hpp"

using namespace CryptoGL;


void SHA512_t::makeNewIV(const BytesVector &code)
{
   SHA512 S;
   const UInt64Vector IV_512 = S.getIV();
   constexpr uint64_t A5_64BITS_CONSTANT = 0xa5a5a5a5a5a5a5a5;
   
   UInt64Vector tmp_IV;
   tmp_IV.reserve(8);
   for (uint8_t i = 0; i < 8; ++i)
   {
      tmp_IV.push_back(IV_512[i] ^ A5_64BITS_CONSTANT);
   }

   S.setIV(tmp_IV);

   // Encode the string 'SHA-512/224' or 'SHA-512/256'.
   BytesVector data = {0x53, 0x48, 0x41, 0x2D, 0x35, 0x31, 0x32, 0x2F};
   data.extend(code);
   const BytesVector answer = S.encode(data);

   // Get the new IV vector.
   setIV(BigEndian64::toIntegersVector(answer));
}

BytesVector SHA512_224::getOutput(const UInt64Vector &hash) const
{
   BytesVector output = BigEndian64::toBytesVector(hash, 3);
   output.extend(BigEndian64::toBytesVector(hash[3]), 0, 4);

   return output;
}