#include "Affine.hpp"

#include "MathematicalTools.hpp"

void Affine::setKey(const int32_t a, const int32_t b)
{
   this->a = a;
   this->b = b;
}

// Encode with the formula : y = (ax + b) (mod alpha_len).
const Affine::ClassicalType Affine::encode(const ClassicalType &clear_text)
{
   const uint8_t alpha_len = alpha.length();
   ClassicalType crypted = "";
   crypted.reserve(clear_text.length());
   
   for(auto c : clear_text)
   {
      const int32_t number = ((a * static_cast<int32_t>(alpha.find(c))) + b) % alpha_len;
      crypted += alpha[(number + alpha_len) % alpha_len];
   }
   
   return crypted;
}

// Decode with the formula : y = a^-1(y - b) (mod alpha_len).
const Affine::ClassicalType Affine::decode(const ClassicalType &cipher_text)
{
   const uint8_t alpha_len = alpha.length();
   ClassicalType decrypted = "";
   decrypted.reserve(cipher_text.length());
   const int32_t a_inv = getModInverse(a, alpha_len);
   
   for(auto c : cipher_text)
   {
      const int32_t number = (a_inv * (static_cast<int32_t>(alpha.find(c)) - b)) % alpha_len;
      decrypted += alpha[(number + alpha_len) % alpha_len];
   }
   
   return decrypted;
}