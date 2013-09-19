/*
 * HMAC source : http://tools.ietf.org/html/rfc2104
 */
#ifndef HASHFUNCTION_HPP
#define HASHFUNCTION_HPP

#include <vector>
#include <string>

#include "Types.hpp"

#include "BigEndian.hpp"
#include "LittleEndian.hpp"

/* 
 * DataType : An unsigned integer for the IV and integer input block.
 * EndianType : An endian class type : BigEndian<DataType> or LittleEndian<DataType>.
 */
template <typename DataType, class EndianType>
class HashFunction
{      
static_assert(std::is_integral<DataType>::value, "Type 'DataType' must be an integral type.");
   
protected:
   using DataTypeVector = std::vector<DataType>;
   
   HashFunction(const uint8_t input_block_size, const uint8_t output_size, const DataTypeVector &IV) 
      : output_size(output_size), input_block_size(input_block_size), IV(IV) {}
   
   virtual ~HashFunction() {}
   
   /* Hash the initial message. */
   virtual const BytesVector encode(const BytesVector &message) = 0;
   
   /* Take a block of data and an initial state and return the hashed block (not final) in 'state'. */
   virtual void compress(DataTypeVector &int_block, DataTypeVector &state) = 0;
   
   /* Output hash size in bytes. */
   const uint8_t output_size;
   
   /* Number of rounds. */
   //const uint8_t rounds;
      
   /* Size of the input block to be hashed. */
   const uint8_t input_block_size;
      
   /* Append length in bits of the initial message to the padded message. */
   template <class EndianLengthType>
   static const BytesVector appendLength(const BytesVector &bytes, const uint64_t length)
   {
      const BytesVector bytes_pad = EndianLengthType::toBytesVector(length);
      BytesVector padded_bytes(bytes);
      padded_bytes.insert(padded_bytes.end(), bytes_pad.begin(), bytes_pad.end());
      
      return padded_bytes;
   }
   
   /* Pad the initial message. */
   virtual const BytesVector pad(const BytesVector &data) const
   {
      const uint8_t rest = sizeof(DataType) << 1;
      BytesVector bytes_pad(data);
      bytes_pad.reserve(data.size() + (input_block_size << 1));

      // Append a bit '1' at the end.
      bytes_pad.push_back(0x80);

      // Pad with '0' bits at the end of bits_pad until the length is 448 (mod 512).
      const uint8_t bytes_pad_len = ((input_block_size << 1) - rest - (bytes_pad.size() 
                     & (input_block_size - 1))) & (input_block_size - 1);
      bytes_pad.insert(bytes_pad.end(), bytes_pad_len + rest - 8, 0);

      return bytes_pad;
   }
   
   /* Transform the input bytes to input block of integers. */
   const DataTypeVector getInputBlocks(const BytesVector &bytes, const uint64_t &block_index) const
   {      
      const uint8_t DataType_size = sizeof(DataType);

      DataTypeVector int_block;
      int_block.reserve(input_block_size / DataType_size);
      for (uint8_t k = 0; k < input_block_size; k += DataType_size)
      {
         int_block.push_back(EndianType::toInteger(BytesVector(bytes.begin() + k + block_index, bytes.begin() + k + block_index + DataType_size)));
      }
      
      return int_block;
   }
   
   /* Get the output hash with a specific size. */
   virtual const BytesVector getOutput(const DataTypeVector &hash) const
   {
      BytesVector output;
      output.reserve(output_size);

      const uint8_t DataType_size = sizeof(DataType);      
      const uint8_t out_data_size = output_size / DataType_size;
      for (uint8_t j = 0; j < out_data_size; ++j)
      {
         const BytesVector bytes = EndianType::toBytesVector(hash[j]);
         output.insert(output.end(), bytes.begin(), bytes.end());
      }

      return output;
   }
      
public:   
   const DataTypeVector getIV() const
   {
      return IV;
   }
   
   void setIV(const DataTypeVector &IV)
   {
      this->IV = IV;
   }
   
   const BytesVector hmacEncode(const BytesVector &hmac_key, const BytesVector &message)
   {
      BytesVector key(hmac_key);
      if(hmac_key.size() > input_block_size)
      {
         key = encode(key);
      }
      
      BytesVector out_key_pad(input_block_size, 0x5C);
      BytesVector in_key_pad(input_block_size, 0x36);
      
      const uint8_t key_len = key.size();
      for(uint8_t i = 0; i < key_len; ++i)
      {
         out_key_pad[i] ^= key[i];
         in_key_pad[i] ^= key[i];
      }
      in_key_pad.insert(in_key_pad.end(), message.begin(), message.end());
      
      const BytesVector in_key_pad_encoded = encode(in_key_pad);
      out_key_pad.insert(out_key_pad.end(), in_key_pad_encoded.begin(), in_key_pad_encoded.end());
      
      return encode(out_key_pad);
   }
   
private:
   /* Initial vector. */
   DataTypeVector IV;
};

#endif