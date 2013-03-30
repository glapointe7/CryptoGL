
#include "Morse.h"

#include <algorithm>

#include "Tools.h"
#include "Data.h"

Morse::Morse()
{
   clear_text = Data::load("clear_text.txt");
   cipher_text = Data::load("cipher_text.txt", true);

   morse = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
      ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
      "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----",
      ".----", "..---", "...--", "....-", ".....", "-....", "--...",
      "---..", "----.", ".-.-.-", "--..--", "..--.."};
   setAlpha("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,?");
}

// Encode un message clair en langage Morse.

string Morse::encode(const std::string &)
{
   string crypted = "";

   for (auto c : clear_text)
   {
      crypted += morse[alpha.find(c)] + " ";
   }

   Data::save("cipher_text.txt", crypted);

   return crypted;
}

// D�code un message crypt� en langage Morse.

string Morse::decode(const std::string &)
{
   string decrypted = "";
   vector<string> cipher_word = split(cipher_text);

   for (auto it : cipher_word)
   {
      unsigned int pos = find(morse.begin(), morse.end(), it) - morse.begin();
      decrypted += alpha[pos];
   }

   Data::save("clear_text.txt", decrypted);

   return decrypted;
}