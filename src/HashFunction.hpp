/*
 * HMAC source : http://tools.ietf.org/html/rfc2104
 */
#ifndef HASHFUNCTION_HPP
#define HASHFUNCTION_HPP

#include <vector>
#include <string>

#include "Endianness.hpp"

template <class UInt, class Endian>
class HashFunction
{
protected:
   typedef std::vector<uint8_t> BytesContainer;
   typedef std::vector<UInt> UIntContainer;
   typedef std::vector<uint32_t> WordsContainer;
   typedef std::vector<uint64_t> DWordsContainer;
   
   explicit HashFunction(const uint8_t in_block_length) : in_block_length(in_block_length) {}
   virtual ~HashFunction() {}
   
   /* Input block length in bytes for a hash function. */
   const uint8_t in_block_length;
   
   virtual const BytesContainer encode(const BytesContainer &) = 0;
     
   virtual const BytesContainer appendPadding(const BytesContainer &data) const
   {
      const uint8_t UInt_bit_size = sizeof(UInt) << 4;
      const uint8_t rest = sizeof(UInt) << 1;
      BytesContainer bytes_pad(data);
      bytes_pad.reserve(data.size() + (UInt_bit_size << 1));

      // Append a bit '1' at the end.
      bytes_pad.push_back(0x80);

      // Pad with '0' bits at the end of bits_pad until the length is 448 (mod 512).
      const uint8_t bytes_pad_len = ((UInt_bit_size << 1) - rest - (bytes_pad.size() & (UInt_bit_size - 1))) & (UInt_bit_size - 1);
      bytes_pad.insert(bytes_pad.end(), bytes_pad_len + rest - 8, 0);

      return bytes_pad;
   }
   
   /* Append the initial length of the message after padding. */
   template<class Endian_type>
   static void appendLength(BytesContainer &bytes, const uint64_t &length)
   {
      Endian_type *E = new Endian_type();
      E->toBytes(length);
      const BytesContainer bytes_pad(E->getBytes());
      bytes.insert(bytes.end(), bytes_pad.begin(), bytes_pad.end());
      delete E;
   }
   
   const UIntContainer getInputBlocks(const BytesContainer &bytes, const uint64_t &block_index) const
   {      
      Endian *E = new Endian();
      const uint8_t UInt_size = E->getIntSize();

      UIntContainer words;
      words.reserve(in_block_length / UInt_size);
      for (uint8_t k = 0; k < in_block_length; k += UInt_size)
      {
         E->toInteger(BytesContainer(bytes.begin() + k + block_index, bytes.begin() + k + block_index + UInt_size));
         words.push_back(E->getValue());
         E->resetValue();
      }
      delete E;
      
      return words;
   }
   
   static const BytesContainer getOutput(const uint8_t max_words, const UIntContainer &hash)
   {
      Endian *E = new Endian();
      const uint8_t UInt_size = E->getIntSize();
      BytesContainer output;
      output.reserve(max_words << 2);

      uint16_t max = max_words;
      if(UInt_size == 8)
      {
         max >>= 1;
      }
      
      for (uint8_t j = 0; j < max; ++j)
      {
         E->toBytes(hash[j]);
         const BytesContainer bytes(E->getBytes());
         output.insert(output.end(), bytes.begin(), bytes.end());
      }
      
      if (max_words % 2 && UInt_size == 8)
      {
         E->toBytes(hash[max]);
         const BytesContainer bytes(E->getBytes());
         output.insert(output.end(), bytes.begin(), bytes.begin() + 4);
      }
      delete E;

      return output;
   }
   
public:
   const BytesContainer hmacEncode(const BytesContainer &hmac_key, const BytesContainer &message)
   {
      BytesContainer key(hmac_key);
      if(hmac_key.size() > in_block_length)
      {
         key = encode(key);
      }
      
      BytesContainer out_key_pad(in_block_length, 0x5C);
      BytesContainer in_key_pad(in_block_length, 0x36);
      
      const uint8_t key_len = key.size();
      for(uint8_t i = 0; i < key_len; ++i)
      {
         out_key_pad[i] ^= key[i];
         in_key_pad[i] ^= key[i];
      }
      in_key_pad.insert(in_key_pad.end(), message.begin(), message.end());
      
      const BytesContainer in_key_pad_encoded = encode(in_key_pad);
      out_key_pad.insert(out_key_pad.end(), in_key_pad_encoded.begin(), in_key_pad_encoded.end());
      
      return encode(out_key_pad);
   }
};

#endif