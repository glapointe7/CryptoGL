
#include "Vigenere.h"

Vigenere::Vigenere(GetCharFunction charDecode, GetCharFunction charEncode)
   : charDecode(charDecode), charEncode(charEncode)
{
}

void Vigenere::setKey(const std::string &key)
{
   this->key = key;
}

std::string Vigenere::encode(const std::string &clear_text)
{
   std::string crypted = "";
   crypted.reserve(clear_text.length());

   unsigned int i = 0;
   unsigned int klen = key.length();
   
   for (auto c : clear_text)
   {
      // Si on atteint la longueur de la clé, alors on recommence au début de la clé.
      if (i == klen)
      {
         i = 0;
      }
      
       crypted += alpha[charEncode(alpha.find(c), alpha.find(key[i]))];
      i++;
   }
   
   return crypted;
}

std::string Vigenere::decode(const std::string &cipher_text)
{
   std::string decrypted = "";
   decrypted.reserve(cipher_text.length());

   unsigned int i = 0;
   unsigned int klen = key.length();
   
   for (auto c : cipher_text)
   {
      // Si on atteint la longueur de la clé, alors on recommence au début de la clé.
      if (i == klen)
      {
         i = 0;
      }
      
       decrypted += alpha[charDecode(alpha.find(c), alpha.find(key[i]))];
      i++;
   }
   
   return decrypted;
}

// TODO Coder le encode/decode standard de vigenere, mais appeler la bonne fonction charDecode
// et charEncode qui existe dans this. Oublie pas, c'est comme une variable normale et comme
// une fonction normale, donc tu l'utilise comme tel, soit text += charEncode(c, i);

/*std::string Vigenere::get(const std::string &data, const Vigenere_types type)
{
   std::string text = "";
   text.reserve(data.length());

   unsigned int i = 0;
   unsigned int klen = key.length();

   VigenereBase *V = VigenereFactory::createVigenere(type);
   
   for (auto c : data)
   {
      // Si on atteint la longueur de la clé, alors on recommence au début de la clé.
      if (i == klen)
      {
         i = 0;
      }
      
       text += V->getChars(c, i);
      i++;
   }
   
   return text;
}*/

// Retourne la clé inversée dans l'alphabet donné.
/*std::string Vigenere::getMinusKey() const
{
   std::string minus_key = "";
   unsigned int key_len = key.length();
   minus_key.reserve(key_len);
   
   for(auto c : key)
   {
      minus_key += alpha[26 - alpha.find(c) - 1];
   }
   
   return minus_key;
}*/

/*std::string Vigenere::encode(const std::string &clear_text)
{
   std::string crypted = "";
   crypted.reserve(clear_text.length());

   unsigned int i = 0;
   unsigned int klen = key.length();

   for (auto c : clear_text)
   {
      // Si on atteint la longueur de la clé, alors on recommence au début de la clé.
      if (i == klen)
      {
         i = 0;
      }
      
      crypted += alpha[(alpha.find(c) + alpha.find(key[i])) % 26];
      i++;
   }

   return crypted;
}

// D�code un texte encod� par le chiffre de Vigen�re.

std::string Vigenere::decode(const std::string &cipher_text)
{
   std::string decrypted = "";
   decrypted.reserve(cipher_text.length());

   unsigned int i = 0;
   unsigned int klen = key.length();

   for (auto c : cipher_text)
   {
      if (i == klen)
      {
         i = 0;
      }

      decrypted += alpha[(alpha.find(c) - alpha.find(key[i]) + 26) % 26];
      i++;
   }

   return decrypted;
}*/
