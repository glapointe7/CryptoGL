#include "Isaac.hpp"

constexpr std::array<uint8_t, 8> Isaac::shift;

uint32_t Isaac::applyRound(const uint32_t accum_shifted, const uint16_t index)
{
   const uint32_t x = rand_mem[index];
   accumulator ^= accum_shifted + rand_mem[(index + 128) & 0xFF];
   rand_mem[index] = accumulator + previous + rand_mem[(x >> 2) & 0xFF];
   const uint32_t result = x + rand_mem[(rand_mem[index] >> 10) & 0xFF];
   previous = result;
   
   return result;
}

const UInt32Vector Isaac::generate()
{
   counter++;
   previous += counter;
   UInt32Vector result = keystream;
   
   for(uint16_t i = 0; i < 256; i += 4)
   {
      result[i] = applyRound(accumulator << 13, i);
      result[i+1] = applyRound(accumulator >> 6, i + 1);
      result[i+2] = applyRound(accumulator << 2, i + 2);
      result[i+3] = applyRound(accumulator >> 16, i + 3);
   }
   
   return result;
}

void Isaac::shuffle(std::array<uint32_t, 8> &tmp)
{     
   for(uint8_t i = 0; i < 8; i += 2)
   {
      tmp[i] ^= tmp[i+1] << shift[i]; 
      tmp[(i+3) & 7] += tmp[i]; 
      tmp[i+1] += tmp[(i+2) & 7];
      
      tmp[i+1] ^= tmp[(i+2) & 7] >> shift[i+1];  
      tmp[(i+4) & 7] += tmp[i+1]; 
      tmp[(i+2) & 7] += tmp[(i+3) & 7];
   }
}

void Isaac::initialize()
{
   std::array<uint32_t, 8> tmp = {{0x9e3779b9, 0x9e3779b9, 0x9e3779b9, 0x9e3779b9, 
      0x9e3779b9, 0x9e3779b9, 0x9e3779b9, 0x9e3779b9}
   };
   
   for(uint8_t i = 0; i < 4; ++i)
   {
      shuffle(tmp);
   }
   
   keystream.reserve(seed.size());
   if(!seed.empty())
   {
      randFill(tmp, keystream);
      randFill(tmp, rand_mem);
   }
   else
   {
      rand_mem.resize(256);
      for(uint16_t i = 0; i < 256; i += 8)
      {
         shuffle(tmp);
         for (uint8_t j = 0; j < 8; ++j)
         {
            rand_mem[i + j] += tmp[j];
         }         
      }
   }
   
   keystream = generate();
}

void Isaac::randFill(std::array<uint32_t, 8> &tmp, const UInt32Vector &rnd)
{
   for(uint16_t i = 0; i < 256; i += 8)
   {
      for(uint8_t j = 0; j < 8; ++j)
      {
         tmp[j] += rnd[i + j];
      }
      
      shuffle(tmp);
      
      for(uint8_t j = 0; j < 8; ++j)
      {
         rand_mem[i + j] += tmp[j];
      }
   }
}