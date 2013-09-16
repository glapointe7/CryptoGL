
#ifndef PADDING_HPP
#define PADDING_HPP

#include "Types.hpp"

namespace Padding
{
   /* Pad 'data' with 'input_block_length' values given by 'fill_with'. */
   const BytesVector zeros(const BytesVector &data, const uint8_t block_size);
   
   /* Add the 'value' of the number of remaining byte 'value' times. */
   const BytesVector remainingValue(const BytesVector &data, const uint8_t block_size);
   
   /* Apply Pad10* rule from the specs. 1 bit upto block_size bits are appended. */
   const BytesVector _10Star(const BytesVector &data, const uint8_t block_size);
   
   /* Apply padding Pad10*1 rule from the specs. 2 bits upto block_size + 1 bits are appended. */
   const BytesVector _10Star1(const BytesVector &data, const uint8_t block_size);
}

#endif