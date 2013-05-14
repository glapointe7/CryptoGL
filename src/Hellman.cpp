#include "Hellman.hpp"

#include "MathematicalTools.hpp"
#include "Tools.hpp"
#include "converterTools.hpp"

#include "exceptions/IntegersNotCoprime.hpp"
#include "exceptions/BadModulo.hpp"

#include <numeric>

void Hellman::setModulo(const uint64_t &modulo)
{
   this->modulo = modulo;
}

void Hellman::setDividend(const uint64_t &dividend)
{
   this->dividend = dividend;
}

// Check if the have super increasing terms in the private key set.

void Hellman::setPrivateKey(const UInt64Container &private_key)
{
   if (!isSuperIncresing(private_key))
   {
      throw KeyNotSuperIncreasing("Your private key have to be super increasing.");
   }

   this->private_key = private_key;
}

// Build the public key.

void Hellman::setPublicKey()
{
   if (GCD(modulo, dividend) != 1)
   {
      throw IntegersNotCoprime("The modulo and dividend have to be coprime.");
   }
   
   if (modulo <= dividend)
   {
      throw BadModulo("Your modulo have to be greater than your dividend.");
   }
   
   /*const uint64_t sum = std::accumulate(private_key.begin(), private_key.end(), 0);
   if(modulo < sum)
   {
      throw BadModulo("Your modulo have to be greater than the sum of numbers in your private key.");
   }*/
   
   for (const auto number : private_key)
   {
      public_key.push_back((number * dividend) % modulo);
   }
}

// Use Glouton's algorithm to find the initial binary vector.

void Hellman::executeGlouton(std::vector<bool> &bits, const uint64_t T, const uint32_t i) const
{
   uint64_t goal = T;
   const uint32_t priv_key_len = private_key.size();
   uint32_t j = priv_key_len - 1;
   const uint32_t k = i * priv_key_len;

   const auto end = private_key.rend();
   for (auto it = private_key.rbegin(); it != end; ++it)
   {
      if (goal >= *it)
      {
         goal -= *it;
         bits[k + j] = 1;
      }
      j--;
   }
}

const Hellman::UInt64Container Hellman::encode(const BytesContainer &clear_text)
{
   UInt64Container crypted;

   // To encode, we need to build the public key.
   setPublicKey();
   const uint32_t block_size = private_key.size();

   // Converts given bytes into binary vector.
   // Then, add ones until we get a multiple of block_size.
   std::vector<bool> binary(convertBytesToBinary(clear_text));
   const uint64_t rest = binary.size() % block_size;
   if(rest != 0)
   {
      binary.insert(binary.end(), block_size - rest, 1);
   }

   // Split the binary vector into blocks of block_size bits.
  const std::vector<std::vector<bool> > bin_blocks(getBlockBinary(binary, block_size));

   // Calculate Sum_{i=0}^{block_size} (block[i] * public_key[i]).
   for (const auto block : bin_blocks)
   {
      uint32_t i = 0;
      uint64_t sum = 0;
      for (const auto bit : block)
      {
         sum += bit * public_key[i];
         i++;
      }
      crypted.push_back(sum);
   }

   return crypted;
}

const Hellman::BytesContainer Hellman::decode(const UInt64Container &cipher_text)
{
   // We have to calculate x = dividend^{-1} (mod modulo).
   const uint64_t inv_coprime = getModInverse(dividend, modulo);

   std::vector<bool> bits(cipher_text.size() * private_key.size(), 0);
   uint32_t i = 0;

   for (const auto number : cipher_text)
   {
      uint64_t goal = (number * inv_coprime) % modulo;
      executeGlouton(bits, goal, i);
      i++;
   }

   return getBytes(bits);
}