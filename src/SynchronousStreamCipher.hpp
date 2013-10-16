
#ifndef SYNCHRONOUSSTREAMCIPHER_HPP
#define SYNCHRONOUSSTREAMCIPHER_HPP

#include "StreamCipher.hpp"

#include "BigEndian.hpp"


/*template <class KeystreamType, class EndianType, uint32_t KeystreamSize, typename GenerateKeystream>
class Encoder
{
public:
   static BytesVector encode(const BytesVector &message, GenerateKeystream getKeystream)
   {
      const uint64_t output_size = message.size();
      BytesVector output;
      output.reserve(output_size);

      const uint8_t type_size = sizeof(typename KeystreamType::value_type);
      for(uint64_t i = 0; i < output_size; i += KeystreamSize)
      {
         const UInt32Vector keystream = getKeystream();
         for(uint64_t j = 0; j < KeystreamSize; j += type_size)
         {
            const BytesVector key_bytes = EndianType::toBytesVector(keystream[j / type_size]);
            for(uint8_t k = 0; k < type_size; ++k)
            {
               const uint64_t current_len = i+j+k;
               if(current_len == output_size)
               {
                  return output;
               }
               output.push_back(message[current_len] ^ key_bytes[k]);
            }
         }
      }

      return output;
   }
};

template <class EndianType, uint32_t KeystreamSize, typename GenerateKeystream>
class Encoder<BytesVector, EndianType, KeystreamSize, GenerateKeystream>
{
public:
   static BytesVector encode(const BytesVector &message, GenerateKeystream getKeystream)
   {
      const uint64_t output_size = message.size();
      BytesVector output;
      output.reserve(output_size);

      for(uint64_t j = 0; j < output_size; j += KeystreamSize)
      {
         const BytesVector keystream = getKeystream();
         for(uint16_t i = 0; i < KeystreamSize; ++i)
         {
            if(output_size == (i+j))
            {
               return output;
            }
            output.push_back(message[i + j] ^ keystream[i]);
         }
      }

      return output;
   }
};

template <class EndianType, typename GenerateKeystream>
class Encoder<BytesVector, EndianType, 1, GenerateKeystream>
{
public:
   static BytesVector encode(const BytesVector &message, GenerateKeystream getKeystream)
   {
      BytesVector output;
      const uint64_t output_size = message.size();
      output.reserve(output_size);

      const BytesVector keystream = getKeystream();
      for(uint64_t i = 0; i < output_size; ++i)
      {
         output.push_back(message[i] ^ keystream[i]);
      }

      return output;
   }
};*/

template <class KeystreamType, class EndianType, uint32_t KeystreamSize>
class SynchronousStreamCipher : public StreamCipher<KeystreamType>
{
public:
   virtual const BytesVector encode(const BytesVector &message) final
   {
      const uint64_t output_size = message.size();
      BytesVector output;
      output.reserve(output_size);

      const uint8_t type_size = sizeof(typename KeystreamType::value_type);
      for(uint64_t i = 0; i < output_size; i += KeystreamSize)
      {
         const KeystreamType keystream = generateKeystream();
         for(uint64_t j = 0; j < KeystreamSize; j += type_size)
         {
            const BytesVector key_bytes = EndianType::toBytesVector(keystream[j / type_size]);
            for(uint8_t k = 0; k < type_size; ++k)
            {
               const uint64_t current_len = i+j+k;
               if(current_len == output_size)
               {
                  return output;
               }
               output.push_back(message[current_len] ^ key_bytes[k]);
            }
         }
      }
       
      return output;
   }
   
protected:         
   virtual ~SynchronousStreamCipher() {}
   
   virtual KeystreamType generateKeystream() = 0;
   
   /* Set the key and check if the key has a correct length. */
   virtual void setKey(const BytesVector &) = 0;
   
   virtual void keySetup() = 0;
};

#endif