/*
 * Source : http://www.ecrypt.eu.org/stream/ciphers/hc256/hc256.pdf
 */
#ifndef HC256_HPP
#define HC256_HPP

#include "SynchronousStreamCipher.hpp"

#include "Bits.hpp"

class HC256 : public SynchronousStreamCipher<UInt32Vector, BigEndian32, 2048>
{
public:
   HC256(const BytesVector &key, const BytesVector &IV) { setIV(IV); setKey(key); }
   
   /* Generate 2048 bytes of keystream. */
   virtual UInt32Vector generateKeystream() final;
   
   virtual void setKey(const BytesVector &key) final;
   void setIV(const BytesVector &IV);
      
private:
   virtual void keySetup() final;
         
   static uint32_t g(const uint32_t x, const uint32_t y, const UInt32Vector &K);
   static uint32_t h(const uint32_t x, const UInt32Vector &K);
   static uint32_t updateSubkeys(UInt32Vector &K, const UInt32Vector &S, const uint16_t index);
   
   static constexpr uint32_t F1(const uint32_t X)
   {
      return Bits::rotateRight(X, 7) ^ Bits::rotateRight(X, 18) ^ (X >> 3);
   }
   
   static constexpr uint32_t F2(const uint32_t X)
   {
      return Bits::rotateRight(X, 17) ^ Bits::rotateRight(X, 19) ^ (X >> 10);
   }
   
   BytesVector IV;
   
   UInt32Vector P, Q;
};

#endif