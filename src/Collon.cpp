#include "Collon.hpp"

using namespace CryptoGL;

// Series length have to be between 1 and text length.

/*void Collon::setBlockLength(const uint32_t block_len)
{
   if (block_len == 0)
   {
      throw ZeroBlockLength("The block length you provided has to be greater than zero.");
   }
   
   this->block_len = block_len;
}*/

ClassicalType Collon::encode(const ClassicalType &clear_text)
{
    const uint32_t clear_len = clear_text.length();
    ClassicalType line1(clear_len), line2(clear_len);

    // Each characters in (x,y) is encoded by the digram AB such that A = (a,y) et B = (x,b).
    for (const auto c : clear_text)
    {
        const Coordinates coords = grid.getCharCoordinates(c);
        line1.push_back(grid.at(coords.y, 0));
        line2.push_back(grid.at(grid.getDimension() - 1, coords.x));
    }

    // The text is then read in blocks. For exemple, for a block of length 3, we have
    // L1 = MHD et L2 = JME => crypted = MHDJME.
    ClassicalType crypted(clear_len * 2);
    for (uint32_t j = 0; j < clear_len; j += block_len)
    {
        crypted.append(line1.substr(j, block_len));
        crypted.append(line2.substr(j, block_len));
    }

    return crypted;
}

ClassicalType Collon::decode(const ClassicalType &cipher_text)
{
    const uint32_t cipher_len = cipher_text.length();
    const uint32_t line_len = cipher_len / 2;

    ClassicalType line1(line_len), line2(line_len);

    // Put the digrams in 2 lines. Generally, the length of a line is not a multiple of
    // the block length. For that reason, we keep the rest stored in line_rest_len.
    const uint32_t double_block_len = block_len * 2;
    const uint32_t line_rest_len = line_len % double_block_len;
    const uint32_t line_blocks_len = cipher_len - (line_rest_len * 2);

    for (uint32_t i = 0; i < line_blocks_len; i += double_block_len)
    {
        line1.append(cipher_text.substr(i, block_len));
        line2.append(cipher_text.substr(i + block_len, block_len));
    }
    line1.append(cipher_text.substr(line_blocks_len, line_rest_len));
    line2.append(cipher_text.substr(line_blocks_len + line_rest_len, line_rest_len));

    // Let A = (x1, y1) and B = (x2, y2) be the 2-letter digram at iteration i.
    // Let C = (x, y) be the decoded letter. We have to get C = (x2, y1).
    ClassicalType decrypted(line_len);
    for (uint32_t i = 0; i < line_len; ++i)
    {
        const Coordinates line1_coords = grid.getCharCoordinates(line1[i]);
        const Coordinates line2_coords = grid.getCharCoordinates(line2[i]);
        decrypted.push_back(grid.at(line1_coords.y, line2_coords.x));
    }

    return decrypted;
}