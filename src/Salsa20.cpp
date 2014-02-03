#include "Salsa20.hpp"

#include "Bits.hpp"
#include "Endian.hpp"
#include "FunctionComposition.hpp"
#include "exceptions/BadKeyLength.hpp"
#include "Endian.hpp"
#include "Vector.hpp"

constexpr std::array<std::array<uint8_t, 4>, 4> Salsa20::sigma;
constexpr std::array<std::array<uint8_t, 4>, 4> Salsa20::tau;

void Salsa20::setKey(const BytesVector &key)
{
   const uint8_t key_size = key.size();
   if(key_size != 16 && key_size != 32)
   {
      throw BadKeyLength("Your key length has to be 16 or 32 bytes.", key_size);
   }
   
   this->key = key;
}

void Salsa20::setIV(const BytesVector &IV)
{
   const uint8_t iv_size = IV.size();
   if(iv_size != 8)
   {
      throw BadIVLength("Your IV length has to be 8 bytes.", iv_size);
   }
   
   this->IV = IV;
}

UInt32Vector Salsa20::quarterRound(const UInt32Vector &Y)
{
   UInt32Vector Z(4, 0);
   Z[1] = Y[1] ^ Bits::rotateLeft(Y[0] + Y[3], 7);
   Z[2] = Y[2] ^ Bits::rotateLeft(Z[1] + Y[0], 9);
   Z[3] = Y[3] ^ Bits::rotateLeft(Z[2] + Z[1], 13);
   Z[0] = Y[0] ^ Bits::rotateLeft(Z[3] + Z[2], 18);
   
   return Z;
}

UInt32Vector Salsa20::rowRound(const UInt32Vector &Y)
{
   UInt32Vector Z = quarterRound(Y);
   Z.reserve(16);
   
   UInt32Vector tmp = quarterRound({Y[5], Y[6], Y[7], Y[4]});
   Vector::extend(Z, {tmp[3], tmp[0], tmp[1], tmp[2]});
   
   tmp = quarterRound({Y[10], Y[11], Y[8], Y[9]});
   Vector::extend(Z, {tmp[2], tmp[3], tmp[0], tmp[1]});
   
   tmp = quarterRound({Y[15], Y[12], Y[13], Y[14]});
   Vector::extend(Z, {tmp[1], tmp[2], tmp[3], tmp[0]});
   
   return Z;
}

UInt32Vector Salsa20::columnRound(const UInt32Vector &Y)
{
   UInt32Vector Z(16, 0);
   
   UInt32Vector tmp = quarterRound({Y[0], Y[4], Y[8], Y[12]});
   Z[0] = tmp[0]; Z[4] = tmp[1]; Z[8] = tmp[2]; Z[12] = tmp[3];
   
   tmp = quarterRound({Y[5], Y[9], Y[13], Y[1]});
   Z[5] = tmp[0]; Z[9] = tmp[1]; Z[13] = tmp[2]; Z[1] = tmp[3];
   
   tmp = quarterRound({Y[10], Y[14], Y[2], Y[6]});
   Z[10] = tmp[0]; Z[14] = tmp[1]; Z[2] = tmp[2]; Z[6] = tmp[3];
   
   tmp = quarterRound({Y[15], Y[3], Y[7], Y[11]});
   Z[15] = tmp[0]; Z[3] = tmp[1]; Z[7] = tmp[2]; Z[11] = tmp[3];
   
   return Z;
}

UInt32Vector Salsa20::doubleRound(const UInt32Vector &Y)
{
   return composer(rowRound, columnRound)(Y);
}

void Salsa20::keySetup()
{
   subkeys.reserve(64);
   if(key.size() == 32)
   {
      subkeys.insert(subkeys.end(), std::begin(sigma[0]), std::end(sigma[0]));
      Vector::extend(subkeys, key, 0, 16);
      subkeys.insert(subkeys.end(), std::begin(sigma[1]), std::end(sigma[1]));
      IVSetup();
      Vector::extend(subkeys, LittleEndian64::toBytesVector(counter));
      subkeys.insert(subkeys.end(), std::begin(sigma[2]), std::end(sigma[2]));
      Vector::extend(subkeys, key, 16);
      subkeys.insert(subkeys.end(), std::begin(sigma[3]), std::end(sigma[3]));
   }
   else
   {
      subkeys.insert(subkeys.end(), std::begin(tau[0]), std::end(tau[0]));
      Vector::extend(subkeys, key);
      subkeys.insert(subkeys.end(), std::begin(tau[1]), std::end(tau[1]));
      IVSetup();
      Vector::extend(subkeys, LittleEndian64::toBytesVector(counter));
      subkeys.insert(subkeys.end(), std::begin(tau[2]), std::end(tau[2]));
      Vector::extend(subkeys, key);
      subkeys.insert(subkeys.end(), std::begin(tau[3]), std::end(tau[3]));
   }
}

void Salsa20::IVSetup()
{
   Vector::extend(subkeys, IV);
}

UInt32Vector Salsa20::generateKeystream()
{  
   keySetup();
   counter++;
   const UInt32Vector x = LittleEndian32::toIntegersVector(subkeys);
   const UInt32Vector z = compose<10>(doubleRound)(x);
   
   UInt32Vector result;
   result.reserve(16);
   for(uint8_t i = 0; i < 16; ++i)
   {
      result.push_back(Bits::bytesSwap(z[i] + x[i]));
   }
   
   return result;
}