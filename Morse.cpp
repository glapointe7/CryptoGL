
#include "Morse.hpp"

#include <algorithm>

#include "Tools.hpp"

Morse::Morse()
{
   morse = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
      ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
      "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----",
      ".----", "..---", "...--", "....-", ".....", "-....", "--...",
      "---..", "----.", ".-.-.-", "--..--", "..--.."};
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?");
}

// Encode un message clair en langage Morse.

const Morse::ClassicalType Morse::encode(const ClassicalType &clear_text)
{
   ClassicalType crypted = "";

   for (auto c : clear_text)
   {
      crypted += morse[alpha.find(c)] + " ";
   }

   return crypted;
}

// D�code un message crypt� en langage Morse.

const Morse::ClassicalType Morse::decode(const ClassicalType &cipher_text)
{
   ClassicalType decrypted = "";
   std::vector<std::string> cipher_word = split(cipher_text);

   for (auto str : cipher_word)
   {
      unsigned int pos = std::find(morse.begin(), morse.end(), str) - morse.begin();
      decrypted += alpha[pos];
   }

   return decrypted;
}