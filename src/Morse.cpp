
#include "Morse.hpp"

#include <algorithm>

#include "String.hpp"
#include "Vector.hpp"

using namespace CryptoGL;

const std::array<ClassicalType, 36> Morse::morse = {
   {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
   ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
   "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----",
   ".----", "..---", "...--", "....-", ".....", "-....", "--...",
   "---..", "----."}
};

Morse::Morse()
{
   setAlpha(ClassicalType::uppercase_digits);
}

ClassicalType Morse::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted(6 * clear_text.length());
   for (const auto c : clear_text)
   {
      const uint32_t found = alpha.find(c);
      crypted.append(morse[found]);
      crypted.push_back(' ');
   }

   return crypted;
}

ClassicalType Morse::decode(const ClassicalType &cipher_text)
{
   ClassicalType decrypted(cipher_text.length() / 2);
   const std::vector<ClassicalType> cipher_word = cipher_text.split(' ');

   for (const auto &str : cipher_word)
   {
      const uint32_t pos = std::distance(morse.begin(), std::find(morse.begin(), morse.end(), str));
      decrypted.push_back(alpha[pos]);
   }

   return decrypted;
}