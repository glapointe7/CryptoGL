#include "Hellman.hpp"

#include "MathematicalTools.hpp"
#include "Tools.hpp"
#include "converterTools.hpp"

#include <algorithm>

void Hellman::setModulo(const uint32_t &modulo)
{
   this->modulo = modulo;
}

void Hellman::setCoprime(const uint32_t &coprime)
{
   this->coprime = coprime;
}

// Vérifier la super croissance de la suite d'entiers donnée dans la clé privée.
void Hellman::setPrivateKey(const Numbers &private_key)
{
   if(isSuperIncresing(private_key))
   {
      this->private_key = private_key;
   }
   // Lancer une exception si la clé n'est pas super croissante.
}

// Vérifier si modulo et coprime sont copremiers.
void Hellman::setPublicKey()
{
   if(GCD(modulo, coprime) == 1 && modulo > coprime)
   {
      for(auto number : private_key)
      {
         public_key.push_back((number * coprime) % modulo);
      }
   }
   // Lancer une exception si la condition n'est pas respectée.
}

// Utilise l'algorithme Glouton pour retrouver le binaire initial.
void Hellman::executeGlouton(std::vector<bool> &bits, const uint32_t T, const unsigned int i) const
{
    uint32_t goal = T;
    const unsigned int priv_key_len = private_key.size();
    unsigned int j = priv_key_len - 1;
    const uint32_t k = i * priv_key_len;
    
    auto end = private_key.rend();
    for(auto it = private_key.rbegin(); it != end; ++it)
    {
        if(goal >= *it)
        {
            goal -= *it;
            bits[k + j] = 1;
        }
        j--;
    }
}

const AsymmetricCipher::Numbers Hellman::encode(const BytesContainer &clear_text)
{
   Numbers crypted;
   
   // Ayant la clé privée, le modulo et le coprime respectant les conditions,
   // on peut donc construire la clé publique.
   setPublicKey();
   const uint32_t block_size = private_key.size();
   
   // On convertit les octets recueillis en binaire.
   // Ensuite, on ajoute des 1 jusqu'à l'obtention d'un multiple de block_size.
   std::vector<bool> binary(convertBytesToBinary(clear_text));
   binary.insert(binary.end(), block_size - (binary.size() % block_size), 1);
   
   // Obtenir la chaîne binaire en blocs de block_size bits.
   std::vector<std::vector<bool> > bin_blocks(getBlockBinary(binary, block_size));
   
   // Calculer Somme_{i=0}^{block_size} (block[i] * publi_key[i]).
   for(auto block : bin_blocks)
   {
      uint32_t i = 0;
      uint32_t sum = 0;
      for(auto bit : block)
      {
         sum += bit * public_key[i];
         i++;
      }
      crypted.push_back(sum);
   }
   
   return crypted;
}

const AsymmetricCipher::BytesContainer Hellman::decode(const Numbers &cipher_text)
{  
   // On calcule x = coprime^{-1} (mod modulo).
   uint32_t inv_coprime = getModInverse(coprime, modulo);
   
   std::vector<bool> bits(cipher_text.size() * private_key.size(), 0);
   unsigned int i = 0;
   
   for(auto number : cipher_text)
   {
      uint32_t goal = (number * inv_coprime) % modulo;
      executeGlouton(bits, goal, i);
      i++;
   }
   
   return getBytes(bits);
}