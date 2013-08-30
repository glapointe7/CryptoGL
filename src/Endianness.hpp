
#ifndef ENDIANNESS_HPP
#define ENDIANNESS_HPP

#include <vector>

template <class UInt>
class Endianness
{
protected:
   typedef std::vector<uint8_t> BytesContainer;
   
   Endianness() { bytes.resize(sizeof(UInt)); value = 0u; }
   virtual ~Endianness() {}
   
   //virtual void toBytes(const UInt &word) = 0;
   //virtual void toInteger(const BytesContainer &bytes) = 0;

   UInt value = 0u;
   BytesContainer bytes;
   const uint8_t int_size = sizeof(UInt);
   
public:   
   /* Transform endianness of an integer (16, 32 or 64 bits). */
   void transform(const UInt &word)
   {
      for(uint8_t i = 0; i < int_size; ++i)
      {
         value |= ((word >> (i << 3)) & 0xFF) << ((int_size - i - 1) << 3);
      }
   }
   
   /* Transform endianness of a bytes vector. */
   void transform(const BytesContainer &old_bytes)
   {
      bytes = BytesContainer(old_bytes.rbegin(), old_bytes.rend());
   }
   
   const BytesContainer getBytes() const
   {
      return bytes;
   }

   UInt getValue() const
   {
      return value;
   }
   
   uint8_t getIntSize() const
   {
      return int_size;
   }
   
   void clearBytes()
   {
      bytes.resize(int_size);
   }

   void resetValue()
   {
      value = 0u;
   }
};

#endif