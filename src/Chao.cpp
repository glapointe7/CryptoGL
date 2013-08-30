#include "Chao.hpp"

#include "exceptions/BadAlphaLength.hpp"

#include <algorithm>

const uint8_t Chao::nadir = 13;

Chao::Chao(const ClassicalType &left_alpha, const ClassicalType &right_alpha)
{
   checkAlpha(left_alpha);
   checkAlpha(right_alpha);
   if (left_alpha.length() != right_alpha.length())
   {
      throw BadAlphaLength("Your left alphabet has to have the same length as the right one.", left_alpha.length());
   }

   this->left_alpha = left_alpha;
   this->right_alpha = right_alpha;
}

void Chao::leftShiftAlphabet(const int8_t ch, const uint8_t index)
{
   std::rotate(left_alpha.begin(), left_alpha.begin() + index, left_alpha.end());
   const int8_t tmp = left_alpha[1];
   left_alpha.erase(left_alpha.begin() + 1);
   left_alpha.insert(left_alpha.begin() + nadir, tmp);
}

void Chao::rightShiftAlphabet(const int8_t ch, const uint8_t index)
{
   std::rotate(right_alpha.begin(), right_alpha.begin() + index + 1, right_alpha.end());
   const int8_t tmp = right_alpha[2];
   right_alpha.erase(right_alpha.begin() + 2);
   right_alpha.insert(right_alpha.begin() + nadir, tmp);
}

const ClassicalType Chao::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted;
   crypted.reserve(clear_text.length());

   for (const auto c : clear_text)
   {
      const uint8_t index = right_alpha.find(c);
      const int8_t corresponding_char = left_alpha[index];
      crypted.push_back(corresponding_char);
      leftShiftAlphabet(corresponding_char, index);
      rightShiftAlphabet(corresponding_char, index);
   }

   return crypted;
}

const ClassicalType Chao::decode(const ClassicalType &cipher_text)
{
   ClassicalType decrypted;
   decrypted.reserve(cipher_text.length());

   for (const auto c : cipher_text)
   {
      const uint8_t index = left_alpha.find(c);
      const int8_t corresponding_char = right_alpha[index];
      decrypted.push_back(corresponding_char);
      leftShiftAlphabet(corresponding_char, index);
      rightShiftAlphabet(corresponding_char, index);
   }

   return decrypted;
}