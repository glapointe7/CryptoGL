
#include "Morse.h"

#include <algorithm>

#include "Tools.h"

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

string Morse::encode(const std::string &clear_text)
{
   string crypted = "";

   for (auto c : clear_text)
   {
      crypted += morse[alpha.find(c)] + " ";
   }

   return crypted;
}

// D�code un message crypt� en langage Morse.

string Morse::decode(const std::string &cipher_text)
{
   string decrypted = "";
   vector<string> cipher_word = split(cipher_text);

   for (auto str : cipher_word)
   {
      unsigned int pos = find(morse.begin(), morse.end(), str) - morse.begin();
      decrypted += alpha[pos];
   }

   return decrypted;
}