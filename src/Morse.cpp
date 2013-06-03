
#include "Morse.hpp"

#include <algorithm>

#include "Tools.hpp"
#include "String.hpp"

const std::vector<Morse::ClassicalType> Morse::morse = {
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

const Morse::ClassicalType Morse::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted;
   crypted.reserve(6 * clear_text.length()); 

   for (const auto c : clear_text)
   {
      crypted += morse[alpha.find(c)];
      crypted += " ";
   }

   return crypted;
}

const Morse::ClassicalType Morse::decode(const ClassicalType &cipher_text)
{
   ClassicalType decrypted;
   decrypted.reserve(cipher_text.size() / 2);
   const std::vector<std::string> cipher_word(split(cipher_text));

   for (const auto str : cipher_word)
   {
      const uint32_t pos = std::find(morse.begin(), morse.end(), str) - morse.begin();
      decrypted += alpha[pos];
   }

   return decrypted;
}