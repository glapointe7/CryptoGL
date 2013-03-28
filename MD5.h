/* 
 * File:   MD5.h
 * Author: gabriel
 *
 * Created on March 27, 2013, 8:41 PM
 */

#ifndef MD5_H
#define	MD5_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class MD5
{
public:
   MD5();
   MD5(const string& text);

   void update(const unsigned char *buf, uint32_t length);
   MD5& finalize();
   string hexdigest() const;

private:
   void transform(const uint8_t block[64]);
   //static void decode(vector<uint32_t> &output, const uint8_t input[], uint32_t len);
   static void encode(uint8_t output[], const vector<uint32_t> &input, uint32_t len);

   bool finalized;
   uint8_t buffer[64]; // bytes that didn't fit in last 64 byte chunk
   vector<uint32_t> count = {0, 0};
   vector<uint32_t> state = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
   uint8_t digest[16]; // the result

   // low level logic operations
   static inline uint32_t F(uint32_t x, uint32_t y, uint32_t z);
   static inline uint32_t G(uint32_t x, uint32_t y, uint32_t z);
   static inline uint32_t H(uint32_t x, uint32_t y, uint32_t z);
   static inline uint32_t I(uint32_t x, uint32_t y, uint32_t z);
   static inline uint32_t rotate_left(uint32_t x, int n);
   static inline void FF(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
   static inline void GG(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
   static inline void HH(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
   static inline void II(uint32_t &a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac);
};
#endif	/* MD5_H */

