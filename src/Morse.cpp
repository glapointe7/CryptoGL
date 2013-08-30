
#include "Morse.hpp"

#include <algorithm>

#include "Tools.hpp"
#include "String.hpp"

const std::vector<ClassicalType> Morse::morse = {
   ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
   ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
   "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----",
   ".----", "..---", "...--", "....-", ".....", "-....", "--...",
   "---..", "----."
};

Morse::Morse()
{
   setAlpha(String::uppercase_digits);
}

const ClassicalType Morse::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted;
   crypted.reserve(6 * clear_text.length()); 

   for (const auto c : clear_text)
   {
      const uint32_t found = alpha.find(c);
      crypted.append(morse[found]);
      crypted.push_back(' ');
   }

   return crypted;
}

const ClassicalType Morse::decode(const ClassicalType &cipher_text)
{
   ClassicalType decrypted;
   decrypted.reserve(cipher_text.length() / 2);
   const std::vector<ClassicalType> cipher_word(split(cipher_text));

   for (const auto str : cipher_word)
   {
      const uint32_t pos = std::distance(morse.begin(), std::find(morse.begin(), morse.end(), str));
      decrypted.push_back(alpha[pos]);
   }

   return decrypted;
}