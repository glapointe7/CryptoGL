/*
 * Réseau de Feistel : DES, Blowfish sont des Feistel.
 * Précisons que la fonction F n'est PAS nécessairement inversible.
 * 
 * Soit un bloc de données de 64 bits Data. On le sépare en deux parties égales : L0 et R0.
 * L_n = R_{n-1}
 * R_n = L_{n-1} XOR F(R_{n-1}, sous-clé_n)
 * swap(L, R)
 * On répète le processus sur 16 rounds.
 */
#ifndef FEISTEL_HPP
#define	FEISTEL_HPP

#include "BlockCipher.hpp"

class Feistel : public BlockCipher
{
protected:
   explicit Feistel(const uint8_t round) : BlockCipher(OperationModes::ECB), rounds(round) {}
   Feistel(const OperationModes mode, const uint8_t round)  : BlockCipher(mode), rounds(round) {}
   virtual ~Feistel() {}
   
   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;
   
   virtual void setKey(const BytesContainer &) = 0;
   virtual void generateSubkeys() = 0;
   virtual const BytesContainer getOutputBlock(const BytesContainer &data, const bool to_encode) = 0;
   
   virtual uint64_t F(const uint64_t &data, const uint64_t &) const = 0;
   virtual void encodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const = 0;
   virtual void decodeFeistelRounds(uint64_t &L, uint64_t &R, const uint8_t) const = 0;
   
   const uint8_t rounds;
};

#endif