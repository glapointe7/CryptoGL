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
public:
   virtual const BytesContainer encode(const BytesContainer &) = 0;
   virtual const BytesContainer decode(const BytesContainer &) = 0;
   virtual void setKey(const BytesContainer &) = 0;

protected:
   virtual const UInt64Container getKeySchedule() = 0;
   virtual uint64_t F(const uint64_t &data, const uint64_t &subkey) const = 0;
   
   
};

#endif