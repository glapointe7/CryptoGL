/*
 * Message Authentification Code.
 */
#ifndef MAC_HPP
#define MAC_HPP

#include "Types.hpp"
#include "exceptions/Exception.hpp"
#include "exceptions/BadKeyLength.hpp"
#include <functional>

#include "Bits.hpp"
#include "Vector.hpp"
#include "Padding.hpp"
#include "MathematicalTools.hpp"

/* Include all block ciphers. */
#include "AES.hpp"
#include "DES.hpp"
#include "Blowfish.hpp"
#include "CAST128.hpp"
#include "CAST256.hpp"
#include "Camellia.hpp"
#include "IDEA.hpp"
#include "Mysty1.hpp"
#include "Noekeon.hpp"
#include "RC2.hpp"
#include "RC5.hpp"
#include "RC6.hpp"
#include "Skipjack.hpp"
#include "Twofish.hpp"
#include "XTEA.hpp"

/*
 * Constants used for L . x^n and L . x^-1.
 */
template <uint8_t BlockSize>
struct Constant
{
   static const BytesVector msb_value;
   static const BytesVector lsb_value;
};

/*
 * Calculate L.u^n for any n > 0.
 */
template <uint8_t BlockSize, int64_t n>
struct Lu
{
   static BytesVector getValue(BytesVector L)
   {
      if(Bits::msb(L) == 0)
         return Lu<BlockSize, n - 1>::getValue(Vector::leftShift(L, 1));
      
      return Vector::Xor(Lu<BlockSize, n - 1>::getValue(Vector::leftShift(L, 1)), Constant<BlockSize>::msb_value);
   }
};

/*
 * Calculate L.u^0 = L.
 */
template <uint8_t BlockSize>
struct Lu<BlockSize, 0>
{
   static BytesVector getValue(BytesVector L)
   {
      return L;
   }
};

/*
 * Calculate L.u^-1.
 */
template <uint8_t BlockSize>
struct Lu<BlockSize, -1>
{
   static BytesVector getValue(BytesVector L)
   {
      if(Bits::lsb(L) == 0)
      {
         return Vector::rightShift(L, 1);
      }
      
      return Vector::Xor(Vector::rightShift(L, 1), Constant<BlockSize>::lsb_value);
   }
};

using BlockCipherBase = BlockCipher<BytesVector, uint64_t, 16, BigEndian64>;

/*
 * MAC Interface.
 */
class MAC
{
protected:
   virtual ~MAC() {}
   virtual BytesVector encode(const BytesVector &message) = 0;
};

/*
 * CBC-MAC mode.
 * Source : http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/xcbc-mac/xcbc-mac-spec.pdf
 */
template <class BlockCipherType>
class CBC_MAC : public MAC
{
   static_assert(!std::is_base_of<BlockCipherBase, BlockCipherType>::value, 
           "'BlockCipherType' has to be a BlockCipher.");
   
public:
   explicit CBC_MAC(const BytesVector &key) : key(key) {}
   
   BytesVector encode(const BytesVector &message)
   {
      constexpr uint8_t BlockSize = BlockCipherType::getBlockSize();
      BlockCipherType Block(key, OperationModes::CBC, BytesVector(BlockSize, 0));
      
      const uint64_t message_size = message.size();
      BytesVector padded_message(message);
      if(message_size == 0 || message_size % BlockSize)
      {
         padded_message = std::move(Padding::_10Star(message, BlockSize));
      }
      
      // We encode in CBC mode the padded message.
      const BytesVector C(Block.encode(padded_message));
      
      // We keep only the last block of the encryption.
      return BytesVector(C.begin() + C.size() - BlockSize, C.end());
   }
   
private:
   BytesVector key;
};

/*
 * AES-XCBC-MAC mode.
 * Source : http://www.ietf.org/rfc/rfc3566.txt
 */
class AES_XCBC_MAC : public MAC
{   
public:
   explicit AES_XCBC_MAC(const BytesVector &key) : key(key) {}
   
   virtual BytesVector encode(const BytesVector &message) final
   {
      AES Block(key);
      const BytesVector K1(Block.encode(BytesVector(16, 0x01)));
      const BytesVector K2(Block.processEncodeBlock(BytesVector(16, 0x02)));
      const BytesVector K3(Block.processEncodeBlock(BytesVector(16, 0x03)));
      const uint64_t message_size = message.size();

      BytesVector padded_message(message);
      BytesVector K;
      if(message_size % 16 || message_size == 0)
      {
         padded_message = std::move(Padding::_10Star(message, 16));
         K = K3;
      }
      else
      {
         K = K2;
      }
      
      const BytesMatrix M = Vector::chunk(padded_message, 16);
      BytesVector previous_cipher_block(16, 0);
      BytesVector current_cipher_block;
      const int64_t M_size = M.size();
      AES Block_K1(K1);
      for(int64_t i = 0; i < M_size - 1; ++i)
      {
         current_cipher_block = Block_K1.encode(Vector::Xor(M[i], previous_cipher_block));
         previous_cipher_block = current_cipher_block;
      }

      return Block_K1.encode(Vector::Xor(Vector::Xor(M[M_size - 1], previous_cipher_block), K));
   }
   
private:
   BytesVector key;
};


/*
 * CMAC mode.
 * Source : http://www.nuee.nagoya-u.ac.jp/labs/tiwata/omac/omac.html 
 */
template <class BlockCipherType>
class CMAC : public MAC
{
   static_assert(!std::is_base_of<BlockCipherBase, BlockCipherType>::value, 
           "'BlockCipherType' has to be a BlockCipher.");
   
public:
   explicit CMAC(const BytesVector &key) : key(key) {}
   
   virtual BytesVector encode(const BytesVector &message) final
   {
      BlockCipherType Block(key);
      constexpr uint8_t BlockSize = BlockCipherType::getBlockSize();
      BytesVector L = Block.encode(BytesVector(BlockSize, 0));
      BytesVector L_u = Lu<BlockSize, 1>::getValue(L);
            
      const uint64_t message_size = message.size();
      BytesVector padded_message(message);
      if(message_size == 0 || message_size % BlockSize)
      {
         padded_message = std::move(Padding::_10Star(message, BlockSize));
         L_u = Lu<BlockSize, 1>::getValue(L_u);
      }
      
      const BytesMatrix M = Vector::chunk(padded_message, BlockSize);
      BytesVector previous_cipher_block(BlockSize, 0);
      BytesVector current_cipher_block;
      const int64_t M_size = M.size();
      for(int64_t i = 0; i < M_size - 1; ++i)
      {
         current_cipher_block = Block.processEncodeBlock(Vector::Xor(M[i], previous_cipher_block));
         previous_cipher_block = current_cipher_block;
      }
      
      return Block.processEncodeBlock(Vector::Xor(Vector::Xor(M[M_size - 1], previous_cipher_block), L_u));
   }
   
private:
   BytesVector key;
};

/*
 * OMAC mode.
 * Source : http://www.nuee.nagoya-u.ac.jp/labs/tiwata/omac/omac.html
 */
template <class BlockCipherType>
class OMAC : public MAC
{
   static_assert(!std::is_base_of<BlockCipherBase, BlockCipherType>::value, 
           "'BlockCipherType' has to be a BlockCipher.");
   
public:
   explicit OMAC(const BytesVector &key) : key(key) {}
   
   virtual BytesVector encode(const BytesVector &message) final
   {
      BlockCipherType Block(key);
      constexpr uint8_t BlockSize = BlockCipherType::getBlockSize();
      BytesVector L = Block.encode(BytesVector(BlockSize, 0));
      BytesVector L_u = Lu<BlockSize, 1>::getValue(L);
      
      // If the message is empty or not a multiple of 'BlockSize', the message 
      // is padded to form a complete block.
      const uint64_t message_size = message.size();
      BytesVector padded_message(message);
      if(message_size == 0 || message_size % BlockSize)
      {
         padded_message = std::move(Padding::_10Star(message, BlockSize));
         L_u = Lu<BlockSize, -1>::getValue(L);       
      }
      
      const BytesMatrix M = Vector::chunk(padded_message, BlockSize);      
      BytesVector previous_cipher_block(BlockSize, 0);
      BytesVector current_cipher_block;
      const int64_t M_size = M.size();
      for(int64_t i = 0; i < M_size - 1; ++i)
      {
         current_cipher_block = Block.processEncodeBlock(Vector::Xor(M[i], previous_cipher_block));
         previous_cipher_block = current_cipher_block;
      }
    
      return Block.processEncodeBlock(Vector::Xor(Vector::Xor(M[M_size - 1], previous_cipher_block), L_u));
   }
   
      
private:   
   BytesVector key;
};

/*
 * PMAC class.
 * Source : http://www.cs.ucdavis.edu/~rogaway/ocb/pmac-bak.htm
 */
template <class BlockCipherType>
class PMAC : public MAC
{
   static_assert(!std::is_base_of<BlockCipherBase, BlockCipherType>::value, 
           "'BlockCipherType' has to be a BlockCipher.");
   
public:
   explicit PMAC(const BytesVector &key) : key(key) {}
 
   virtual BytesVector encode(const BytesVector &message) final
   {
      //const uint64_t message_size = message.size();
      BlockCipherType Block(key);
      constexpr uint8_t BlockSize = BlockCipherType::getBlockSize();
         
      // Pad the message if it's not a multiple of BlockSize.
      BytesVector padded_message;
      bool need_padding = false;
      const uint64_t message_size = message.size();
      if(message_size % BlockSize || message_size == 0)
      {
         need_padding = true;
         padded_message = std::move(Padding::_10Star(message, BlockSize));
      }
      else
      {
         padded_message = message;
      }
      
      const BytesMatrix M = Vector::chunk(padded_message, BlockSize);
      BytesVector previous_cipher_block(BlockSize, 0);
      BytesVector sigma(previous_cipher_block);
      BytesVector L = Block.encode(BytesVector(BlockSize, 0));
      const int64_t M_size = M.size();
      for(int64_t i = 0; i < M_size - 1; ++i)
      {
         sigma = Vector::Xor(Block.processEncodeBlock(Vector::Xor(M[i], getLuNValue(L, i))), sigma);
      }
      sigma = Vector::Xor(M[M_size - 1], sigma);
      if(!need_padding)
      {
         sigma = Vector::Xor(Lu<BlockSize, -1>::getValue(L), sigma);
      }

      return Block.processEncodeBlock(sigma);
   }
   
private:
   static BytesVector getLuNValue(BytesVector L, const uint64_t &n)
   {
      if(Bits::msb(L) == 0)
         return getLuNValue(Vector::leftShift(L, 1), n-1);
      
      return Vector::Xor(getLuNValue(Vector::leftShift(L, 1), n-1), 
              Constant<BlockCipherType::getBlockSize()>::msb_value);
   }
   
   BytesVector key;
};

/*
 * TMAC class.
 * Source : http://csrc.nist.gov/groups/ST/toolkit/BCM/documents/proposedmodes/tmac/tmac-spec.pdf
 */
template <class BlockCipherType>
class TMAC : public MAC
{
   static_assert(!std::is_base_of<BlockCipherBase, BlockCipherType>::value, 
           "'BlockCipherType' has to be a BlockCipher.");
   
public:
   TMAC(const BytesVector &key, const BytesVector &key2) : key(key) { setKey(key2); }
   
   virtual BytesVector encode(const BytesVector &message) final
   {
      const uint64_t message_size = message.size();
      BlockCipherType Block(key);
      constexpr uint8_t BlockSize = BlockCipherType::getBlockSize();
      
      BytesVector K;
      BytesVector padded_message;
      if(message_size % BlockSize || message_size == 0)
      {
         K = key2;
         padded_message = Padding::_10Star(message, BlockSize);
      }
      else
      {
         K = Lu<BlockSize, 1>::getValue(key2);
         padded_message = message;
      }

      const BytesMatrix M = Vector::chunk(padded_message, BlockSize);
      BytesVector previous_cipher_block(BlockSize, 0);
      BytesVector current_cipher_block = Block.encode({});
      const int64_t M_size = M.size();
      for(int64_t i = 0; i < M_size - 1; ++i)
      {
         current_cipher_block = Block.processEncodeBlock(Vector::Xor(M[i], previous_cipher_block));
         previous_cipher_block = current_cipher_block;
      }

      return Block.processEncodeBlock(Vector::Xor(Vector::Xor(M[M_size - 1], previous_cipher_block), K));
   }
   
   void setKey(const BytesVector &key2)
   {
      if(key2.empty() || key2.size() != BlockCipherType::getBlockSize())
      {
         throw BadKeyLength("Your 'key2' has to be the same size as the block cipher uses.", key2.size());
      }
      
      this->key2 = key2;
   }
   
private:
   BytesVector key, key2;
};

#endif