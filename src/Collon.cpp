
#include "Collon.hpp"

// Series length have to be between 1 and text length.

void Collon::setBlockLength(const uint32_t block_len)
{
   if (block_len == 0)
   {
      throw ZeroBlockLength("The block length you provided has to be greater than zero.");
   }
   
   this->block_len = block_len;
}

const Collon::ClassicalType Collon::encode(const ClassicalType &clear_text)
{
   const uint32_t clear_len = clear_text.length();
   std::string line1, line2;
   line1.reserve(clear_len);
   line2.reserve(clear_len);
   ClassicalType crypted;
   crypted.reserve(clear_len * 2);

   // Build the cipher grid without duplicated letters.
   KeyType key_alpha = getKey();
   key_alpha.reserve(alpha.length());
   key_alpha.append(alpha);
   const Grid grid = getGrid(key_alpha);

   // Each characters in (x,y) is encoded by the digram AB such that A = (a,y) et B = (x,b).
   for (const auto c : clear_text)
   {
      const auto coords = getCharCoordinates(c, grid);
      line1.push_back(grid[coords.second][0]);
      line2.push_back(grid[dim - 1][coords.first]);
   }

   // The text is then read in blocks. For exemple, for a block of length 3, we have
   // L1 = MHD et L2 = JME => crypted = MHDJME.
   for (uint32_t j = 0; j < clear_len; j += block_len)
   {
      crypted.append(line1.substr(j, block_len));
      crypted.append(line2.substr(j, block_len));
   }

   return crypted;
}

const Collon::ClassicalType Collon::decode(const ClassicalType &cipher_text)
{
   const uint32_t cipher_len = cipher_text.length();
   const uint32_t line_len = cipher_len / 2;

   ClassicalType line1, line2;
   line1.reserve(line_len);
   line2.reserve(line_len);

   const Grid grid(getGrid(getKey() + alpha));

   // Put the digrams in 2 lines. Generally, the length of a line is not a multiple of
   // the block length. For that reason, we keep the rest stored in line_rest_len.
   const uint32_t double_block_len = block_len << 1;
   const uint32_t line_rest_len = line_len % double_block_len;
   const uint32_t line_blocks_len = cipher_len - (line_rest_len << 1);

   for (uint32_t i = 0; i < line_blocks_len; i += double_block_len)
   {
      line1 += cipher_text.substr(i, block_len);
      line2 += cipher_text.substr(i + block_len, block_len);
   }
   line1 += cipher_text.substr(line_blocks_len, line_rest_len);
   line2 += cipher_text.substr(line_blocks_len + line_rest_len, line_rest_len);

   // Let A = (x1, y1) and B = (x2, y2) be the 2-letter digram at iteration i.
   // Let C = (x, y) be the decoded letter. We have to get C = (x2, y1).
   ClassicalType decrypted;
   decrypted.reserve(line_len);
   for (uint32_t i = 0; i < line_len; ++i)
   {
      const auto A = getCharCoordinates(line1[i], grid);
      const auto B = getCharCoordinates(line2[i], grid);

      decrypted.push_back(grid[A.second][B.first]);
   }

   return decrypted;
}