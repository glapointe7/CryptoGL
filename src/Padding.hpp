
#ifndef PADDING_HPP
#define PADDING_HPP

#include "Types.hpp"

/* This class shouldn't be used publicly nor by the user. */
class Padding
{
private:
   /* SpongeFunction, HashFunction and BlockCipher classes can use this class. */
   friend class MD2;
   
   template <class SubkeyType, class DataType, uint8_t InputBlockSize, class EndianType>
   friend class BlockCipher;
   
   template <typename T>
   friend class SpongeFunction;
   
   /* Pad 'data' with 'input_block_length' values given by 'fill_with'. */
   static const BytesVector zeros(const BytesVector &data, const uint8_t block_size);
   
   /* Add the 'value' of the number of remaining byte 'value' times. */
   static const BytesVector remainingValue(const BytesVector &data, const uint8_t block_size);
   
   /* Apply Pad10* rule from the specs. 1 bit upto block_size bits are appended. */
   static const BytesVector _10Star(const BytesVector &data, const uint8_t block_size);
   
   /* Apply padding Pad10*1 rule from the specs. 2 bits upto block_size + 1 bits are appended. */
   static const BytesVector _10Star1(const BytesVector &data, const uint8_t block_size, 
            const uint8_t first_byte = 0x01, const uint8_t last_byte = 0x80);
};

#endif