#include "MD5.hpp"

#include <stdio.h>
#include <string.h>
#include <vector>

// F, G, H et I sont des fonctions de base pour le calcul du MD5.

inline uint32_t MD5::F(uint32_t x, uint32_t y, uint32_t z)
{
   return (x & y) | (~x & z);
}

inline uint32_t MD5::G(uint32_t x, uint32_t y, uint32_t z)
{
   return (x & z) | (y & ~z);
}

inline uint32_t MD5::H(uint32_t x, uint32_t y, uint32_t z)
{
   return x ^ y ^ z;
}

inline uint32_t MD5::I(uint32_t x, uint32_t y, uint32_t z)
{
   return y ^ (x | ~z);
}

// rotate_left rotates x left n bits.

inline uint32_t MD5::rotate_left(uint32_t x, int n)
{
   return (x << n) | (x >> (32 - n));
}

// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.

inline void MD5::FF(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
   a = rotate_left(a + F(b, c, d) + x + ac, s) + b;
}

inline void MD5::GG(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
   a = rotate_left(a + G(b, c, d) + x + ac, s) + b;
}

inline void MD5::HH(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
   a = rotate_left(a + H(b, c, d) + x + ac, s) + b;
}

inline void MD5::II(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
   a = rotate_left(a + I(b, c, d) + x + ac, s) + b;
}

// default ctor, just initailize

MD5::MD5()
{
   finalized = false;
}

// nifty shortcut ctor, compute MD5 for string and finalize it right away

MD5::MD5(const std::string &text)
{
   finalized = false;
   update((const unsigned char*) text.c_str(), text.length());
   finalize();
}

// encodes input (uint32_t) into output (unsigned char). Assumes len is
// a multiple of 4.

void MD5::encode(uint8_t output[], const std::vector<uint32_t> &input, uint32_t len)
{
   for (uint32_t i = 0, j = 0; j < len; i++, j += 4)
   {
      output[j] = input[i] & 0xff;
      output[j + 1] = (input[i] >> 8) & 0xff;
      output[j + 2] = (input[i] >> 16) & 0xff;
      output[j + 3] = (input[i] >> 24) & 0xff;
   }
}

// Algorithme principal du MD5.

void MD5::transform(const uint8_t block[64])
{
   std::vector<uint32_t> hash(state);
   std::vector<uint32_t> x(16, 0);
   
   // decodes input (unsigned char) into output (uint32_t). Assumes len is a multiple of 4.
   for (unsigned int i = 0, j = 0; j < 64; i++, j += 4)
   {
      x[i] = ((uint32_t) block[j]) | (((uint32_t) block[j + 1]) << 8) |
              (((uint32_t) block[j + 2]) << 16) | (((uint32_t) block[j + 3]) << 24);
   }

   // Constantes pour la transformation MD5.
   const std::vector<uint32_t> routine = {7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6, 10, 15, 21};

   // valeurs obtenues par la formule k(i) = sin(i + 1) * 2^32 pour i = 0,...,63.
   const std::vector<unsigned int> k = {
      0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
      0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
      0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
      0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
      0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
      0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
      0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
      0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
      0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
      0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
      0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
      0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
      0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
      0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
      0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
      0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
   };

   FF(hash[0], hash[1], hash[2], hash[3], x[ 0], routine[0], k[0]);
   FF(hash[3], hash[0], hash[1], hash[2], x[ 1], routine[1], k[1]);
   FF(hash[2], hash[3], hash[0], hash[1], x[ 2], routine[2], k[2]);
   FF(hash[1], hash[2], hash[3], hash[0], x[ 3], routine[3], k[3]);
   FF(hash[0], hash[1], hash[2], hash[3], x[ 4], routine[0], k[4]);
   FF(hash[3], hash[0], hash[1], hash[2], x[ 5], routine[1], k[5]);
   FF(hash[2], hash[3], hash[0], hash[1], x[ 6], routine[2], k[6]);
   FF(hash[1], hash[2], hash[3], hash[0], x[ 7], routine[3], k[7]);
   FF(hash[0], hash[1], hash[2], hash[3], x[ 8], routine[0], k[8]);
   FF(hash[3], hash[0], hash[1], hash[2], x[ 9], routine[1], k[9]);
   FF(hash[2], hash[3], hash[0], hash[1], x[10], routine[2], k[10]);
   FF(hash[1], hash[2], hash[3], hash[0], x[11], routine[3], k[11]);
   FF(hash[0], hash[1], hash[2], hash[3], x[12], routine[0], k[12]);
   FF(hash[3], hash[0], hash[1], hash[2], x[13], routine[1], k[13]);
   FF(hash[2], hash[3], hash[0], hash[1], x[14], routine[2], k[14]);
   FF(hash[1], hash[2], hash[3], hash[0], x[15], routine[3], k[15]);

   GG(hash[0], hash[1], hash[2], hash[3], x[ 1], routine[4], k[16]);
   GG(hash[3], hash[0], hash[1], hash[2], x[ 6], routine[5], k[17]);
   GG(hash[2], hash[3], hash[0], hash[1], x[11], routine[6], k[18]);
   GG(hash[1], hash[2], hash[3], hash[0], x[ 0], routine[7], k[19]);
   GG(hash[0], hash[1], hash[2], hash[3], x[ 5], routine[4], k[20]);
   GG(hash[3], hash[0], hash[1], hash[2], x[10], routine[5], k[21]);
   GG(hash[2], hash[3], hash[0], hash[1], x[15], routine[6], k[22]);
   GG(hash[1], hash[2], hash[3], hash[0], x[ 4], routine[7], k[23]);
   GG(hash[0], hash[1], hash[2], hash[3], x[ 9], routine[4], k[24]);
   GG(hash[3], hash[0], hash[1], hash[2], x[14], routine[5], k[25]);
   GG(hash[2], hash[3], hash[0], hash[1], x[ 3], routine[6], k[26]);
   GG(hash[1], hash[2], hash[3], hash[0], x[ 8], routine[7], k[27]);
   GG(hash[0], hash[1], hash[2], hash[3], x[13], routine[4], k[28]);
   GG(hash[3], hash[0], hash[1], hash[2], x[ 2], routine[5], k[29]);
   GG(hash[2], hash[3], hash[0], hash[1], x[ 7], routine[6], k[30]);
   GG(hash[1], hash[2], hash[3], hash[0], x[12], routine[7], k[31]);

   HH(hash[0], hash[1], hash[2], hash[3], x[ 5], routine[8], k[32]);
   HH(hash[3], hash[0], hash[1], hash[2], x[ 8], routine[9], k[33]);
   HH(hash[2], hash[3], hash[0], hash[1], x[11], routine[10], k[34]);
   HH(hash[1], hash[2], hash[3], hash[0], x[14], routine[11], k[35]);
   HH(hash[0], hash[1], hash[2], hash[3], x[ 1], routine[8], k[36]);
   HH(hash[3], hash[0], hash[1], hash[2], x[ 4], routine[9], k[37]);
   HH(hash[2], hash[3], hash[0], hash[1], x[ 7], routine[10], k[38]);
   HH(hash[1], hash[2], hash[3], hash[0], x[10], routine[11], k[39]);
   HH(hash[0], hash[1], hash[2], hash[3], x[13], routine[8], k[40]);
   HH(hash[3], hash[0], hash[1], hash[2], x[ 0], routine[9], k[41]);
   HH(hash[2], hash[3], hash[0], hash[1], x[ 3], routine[10], k[42]);
   HH(hash[1], hash[2], hash[3], hash[0], x[ 6], routine[11], k[43]);
   HH(hash[0], hash[1], hash[2], hash[3], x[ 9], routine[8], k[44]);
   HH(hash[3], hash[0], hash[1], hash[2], x[12], routine[9], k[45]);
   HH(hash[2], hash[3], hash[0], hash[1], x[15], routine[10], k[46]);
   HH(hash[1], hash[2], hash[3], hash[0], x[ 2], routine[11], k[47]);

   II(hash[0], hash[1], hash[2], hash[3], x[ 0], routine[12], k[48]);
   II(hash[3], hash[0], hash[1], hash[2], x[ 7], routine[13], k[49]);
   II(hash[2], hash[3], hash[0], hash[1], x[14], routine[14], k[50]);
   II(hash[1], hash[2], hash[3], hash[0], x[ 5], routine[15], k[51]);
   II(hash[0], hash[1], hash[2], hash[3], x[12], routine[12], k[52]);
   II(hash[3], hash[0], hash[1], hash[2], x[ 3], routine[13], k[53]);
   II(hash[2], hash[3], hash[0], hash[1], x[10], routine[14], k[54]);
   II(hash[1], hash[2], hash[3], hash[0], x[ 1], routine[15], k[55]);
   II(hash[0], hash[1], hash[2], hash[3], x[ 8], routine[12], k[56]);
   II(hash[3], hash[0], hash[1], hash[2], x[15], routine[13], k[57]);
   II(hash[2], hash[3], hash[0], hash[1], x[ 6], routine[14], k[58]);
   II(hash[1], hash[2], hash[3], hash[0], x[13], routine[15], k[59]);
   II(hash[0], hash[1], hash[2], hash[3], x[ 4], routine[12], k[60]);
   II(hash[3], hash[0], hash[1], hash[2], x[11], routine[13], k[61]);
   II(hash[2], hash[3], hash[0], hash[1], x[ 2], routine[14], k[62]);
   II(hash[1], hash[2], hash[3], hash[0], x[ 9], routine[15], k[63]);

   state[0] += hash[0];
   state[1] += hash[1];
   state[2] += hash[2];
   state[3] += hash[3];
}

// MD5 block update operation. Continues an MD5 message-digest
// operation, processing another message block

void MD5::update(const unsigned char input[], uint32_t length)
{
   // compute number of bytes mod 64
   uint32_t index = count[0] / 8 % 64;

   // Update number of bits
   if ((count[0] += (length << 3)) < (length << 3))
   {
      count[1]++;
   }
   count[1] += (length >> 29);

   // number of bytes we need to fill in buffer
   uint32_t firstpart = 64 - index;

   uint32_t i;

   // transform as many times as possible.
   if (length >= firstpart)
   {
      // buffer = input.substr(0, firstpart);
      memcpy(&buffer[index], input, firstpart);
      transform(buffer);

      // transform chunks of 64 (64 bytes)
      for (i = firstpart; i + 64 <= length; i += 64)
      {
         transform(&input[i]);
      }
      // string input2; input2 += input[i];
      // transform(input2);
      index = 0;
   }
   else
   {
      i = 0;
   }

   // buffer += input.substr(i, length - i);
   memcpy(&buffer[index], &input[i], length - i);
}

// MD5 finalization. Ends an MD5 message-digest operation, writing the
// the message digest and zeroizing the context.

MD5& MD5::finalize()
{
   static unsigned char padding[64] = {
      0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
   };

   if (!finalized)
   {
      // Save number of bits
      unsigned char bits[8];
      encode(bits, count, 8);

      // pad out to 56 mod 64.
      uint32_t index = count[0] / 8 % 64;
      uint32_t padLen = (index < 56) ? (56 - index) : (120 - index);
      update(padding, padLen);

      // Append length (before padding)
      update(bits, 8);

      // Store state in digest
      encode(digest, state, 16);

      // Zeroize sensitive information.
      memset(buffer, 0, sizeof buffer);

      finalized = true;
   }

   return *this;
}

// Retourne la string représentant le codage en hexadécimal.

std::string MD5::hexdigest() const
{
   if (!finalized)
   {
      return "";
   }

   char buf[33];
   for (int i = 0; i < 16; i++)
   {
      sprintf(buf + i * 2, "%02x", digest[i]);
   }
   buf[32] = 0;

   return std::string(buf);
}