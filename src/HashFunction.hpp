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

template <class DataType, class EndianType>
class HashFunction
{
protected:
   typedef std::vector<DataType> DataTypeVector;
   
   HashFunction(const uint8_t block_size, const uint8_t output_size, const DataTypeVector &IV) 
      : block_size(block_size), output_size(output_size), IV(IV) {}
   
   virtual ~HashFunction() {}
   
   /* Input block length in bytes for a hash function. */
   const uint8_t block_size;
   
   /* Output hash size in bytes. */
   const uint8_t output_size;
   
   /* Number of rounds. */
   //const uint8_t rounds;
   
   /* Initial vector. */
   DataTypeVector IV;
   
   /* Salt */
   // const DataTypeVector salt;
   
   virtual const BytesVector encode(const BytesVector &) = 0;
   
   /* Initialize by padding the message and appending the length of the message to complete padding. */
   /*virtual const BytesVector initialize(const BytesVector &message)
   {
      BytesVector padded_message = appendPadding(message);
      padded_message = Length<BigEndian64>::append(padded_message, message.size() << 3);
      
      return padded_message;
   }
   
   virtual void finalize(DataTypeVector &hash)
   {
   
   }
   
   const BytesVector encode(const BytesVector &message)
   {
      const BytesVector padded_message = initialize(message);
      const uint64_t padded_message_size = padded_message.size();
      
      DataTypeVector hash(IV);
      for (uint64_t i = 0; i < padded_message_size; i += block_size)
      {              
         DataTypeVector int_block = getInputBlocks(padded_message, i);    
         compress(int_block, hash);
      }
      finalize(hash);

      return getOutput(hash);
   }*/
   
   /* Process the main algorithm of the hash function. */
   virtual void compress(DataTypeVector &int_block, DataTypeVector &state) = 0;
   
   template <class EndianLengthType>
   static const BytesVector appendLength(const BytesVector &bytes, const uint64_t length)
   {
      const BytesVector bytes_pad = EndianLengthType::toBytesVector(length);
      BytesVector padded_bytes(bytes);
      padded_bytes.insert(padded_bytes.end(), bytes_pad.begin(), bytes_pad.end());
      
      return padded_bytes;
   }
   
   virtual const BytesVector appendPadding(const BytesVector &data) const
   {
      const uint8_t rest = sizeof(DataType) << 1;
      BytesVector bytes_pad(data);
      bytes_pad.reserve(data.size() + (block_size << 1));

      // Append a bit '1' at the end.
      bytes_pad.push_back(0x80);

      // Pad with '0' bits at the end of bits_pad until the length is 448 (mod 512).
      const uint8_t bytes_pad_len = ((block_size << 1) - rest - (bytes_pad.size() & (block_size - 1))) & (block_size - 1);
      bytes_pad.insert(bytes_pad.end(), bytes_pad_len + rest - 8, 0);

      return bytes_pad;
   }
   
   /* Transform the input bytes to input block of integers. */
   const DataTypeVector getInputBlocks(const BytesVector &bytes, const uint64_t &block_index) const
   {      
      const uint8_t DataType_size = sizeof(DataType);

      DataTypeVector words;
      words.reserve(block_size / DataType_size);
      for (uint8_t k = 0; k < block_size; k += DataType_size)
      {
         words.push_back(EndianType::toInteger(BytesVector(bytes.begin() + k + block_index, bytes.begin() + k + block_index + DataType_size)));
      }
      
      return words;
   }
   
   /* Get the output hash with a specific size. */
   const BytesVector getOutput(const DataTypeVector &hash) const
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
      
      if (output_size & (DataType_size - 1))
      {
         const BytesVector bytes = EndianType::toBytesVector(hash[out_data_size]);
         output.insert(output.end(), bytes.begin(), bytes.begin() + 4);
      }

      return output;
   }
   
public:
   const BytesVector hmacEncode(const BytesVector &hmac_key, const BytesVector &message)
   {
      BytesVector key(hmac_key);
      if(hmac_key.size() > block_size)
      {
         key = encode(key);
      }
      
      BytesVector out_key_pad(block_size, 0x5C);
      BytesVector in_key_pad(block_size, 0x36);
      
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
};

#endif