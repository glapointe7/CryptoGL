/*
 * Source : FIPS197 from NIST and http://en.wikipedia.org/wiki/Rijndael_mix_columns
 */
#ifndef AES_HPP
#define	AES_HPP

#include "BlockCipher.hpp"

#include <vector>

class AES : public BlockCipher
{
public:
   explicit AES(const BytesContainer &key) : BlockCipher(OperationModes::ECB, 16) { setKey(key); }
   AES(const BytesContainer &key, const OperationModes mode) : BlockCipher(mode, 16) { setKey(key); }
   
   virtual const BytesContainer encode(const BytesContainer &clear_text) final;
   virtual const BytesContainer decode(const BytesContainer &cipher_text) final;
   
private:      
   virtual void setKey(const BytesContainer &key) final;
   virtual void generateSubkeys() final;
   virtual const BytesContainer getOutputBlock(const BytesContainer &block, const bool to_encode) final;
   
   static void subBytes(UInt32Container &state, const uint8_t *box);
         
   uint32_t subWord(const uint32_t word);
   void addRoundKey(UInt32Container &state, const uint8_t round);
   
   static void shiftRows(UInt32Container &state);
   static void inverseShiftRows(UInt32Container &state);
   
   /* Function using Galois field GF(256) and 4 functions used to calculate each of the 4 rows. */
   void mixColumns(UInt32Container &state);
   void inverseMixColumns(UInt32Container &state);
   
   uint32_t rounds;
   
   /* sbox is pre-computed multiplicative inverse in GF(2^8) used in subBytes and keyExpansion. */
   static const uint8_t sbox[256];
   static const uint8_t inverse_sbox[256];
   static const uint8_t shift_indexes[16];
   static const uint8_t inverse_shift_indexes[16];
   static const uint32_t round_constants[15];
   
   /* Lookup tables for Galois field multiplication by 2, 3, 9, 11, 13 and 14. */
   static const uint8_t table_2[256];
   static const uint8_t table_3[256];
   static const uint8_t table_9[256];
   static const uint8_t table_11[256];
   static const uint8_t table_13[256];
   static const uint8_t table_14[256];
};

#endif