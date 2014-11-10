#include "Affine.hpp"

#include "MathematicalTools.hpp"

using namespace CryptoGL;

void Affine::setKey(const int32_t a, const int32_t b)
{
   this->a = a;
   this->b = b;
}

// Encode with the formula : y = (ax + b) (mod alpha_len).
ClassicalType Affine::encode(const ClassicalType &clear_text)
{
   const uint32_t alpha_len = alpha.length();
   ClassicalType crypted(clear_text.length());
   
   for(const auto c : clear_text)
   {
      const uint32_t number = ((((a * alpha.find(c)) + b) % alpha_len) + alpha_len) % alpha_len;
      crypted.push_back(alpha[number]);
   }
   
   return crypted;
}

// Decode with the formula : y = a^-1(y - b) (mod alpha_len).
ClassicalType Affine::decode(const ClassicalType &cipher_text)
{
   const int32_t alpha_len = alpha.length();
   ClassicalType decrypted(cipher_text.length());
   
   const int32_t a_inv = Maths::getModInverse(a, alpha_len);
   
   for(const auto c : cipher_text)
   {
      const uint8_t number = ((a_inv * (static_cast<int8_t>((alpha.find(c)) - b)) % alpha_len) + alpha_len) % alpha_len;
      decrypted.push_back(alpha[number]);
   }
   
   return decrypted;
}