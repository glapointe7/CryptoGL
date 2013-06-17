
#ifndef LITTLEENDIAN_HPP
#define LITTLEENDIAN_HPP

template <class T>
class LittleEndian
{
public:
   virtual ~LittleEndian() {}
   
   void add(const uint8_t byte, const uint32_t offset)
   {
      value |= byte << (8 * (offset % mod));
   }
   
   /* Transform an integer (32 of 64 bits) from big endian to little endian. */
   void transform(const T word)
   {
      for(uint8_t i = 0; i < mod; ++i)
      {
         value |= ((word >> (i << 3)) & 0xFF) << ((mod - i - 1) << 3);
      }
   }

   T getValue() const
   {
      return value;
   }

   void reset()
   {
      value = 0u;
   }

private:
   T value = 0u;
   const uint8_t mod = sizeof(T);
};

using LittleEndian4Bytes = LittleEndian<uint32_t>;
using LittleEndian8Bytes = LittleEndian<uint64_t>;

#endif