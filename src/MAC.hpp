/*
 * Message Authentication Code.
 */
#ifndef MAC_HPP
#define MAC_HPP

#include "Types.hpp"
#include "exceptions/BadKeyLength.hpp"

#include "Padding.hpp"

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

namespace CryptoGL
{
    /*
     * Constants used for L . x^n (msb_value) and L . x^-1 (lsb_value) for n > 0.
     */
    template <uint8_t BlockSize>
    struct Constant
    {
        static_assert(BlockSize == 8 || BlockSize == 16, "'BlockSize' has to be 8 or 16.");
        static const BytesVector msb_value;
        static const BytesVector lsb_value;
    };

    template<>
    const BytesVector Constant<8>::msb_value = {0, 0, 0, 0, 0, 0, 0, 0x1B};

    template <>
    const BytesVector Constant<16>::msb_value = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x87};

    template<>
    const BytesVector Constant<8>::lsb_value = {0x80, 0, 0, 0, 0, 0, 0, 0x0D};

    template <>
    const BytesVector Constant<16>::lsb_value = {0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x43};

    /*
     * Calculate L.u^n for any n > 0.
     */
    template <uint8_t BlockSize, int64_t n>
    struct Lu
    {
        static BytesVector getValue(const BytesVector &L)
        {
            if (L.msb() == 0)
                return Lu <BlockSize, n - 1>::getValue(L.leftShift(1));

            return Lu<BlockSize, n - 1>::getValue(L.leftShift(1)).Xor(Constant<BlockSize>::msb_value);
        }
    };

    /*
     * Calculate L.u^0 = L.
     */
    template <uint8_t BlockSize>
    struct Lu<BlockSize, 0>
    {
        static BytesVector getValue(const BytesVector &L)
        {
            return L;
        }
    };

    /*
     * Calculate L.u^-1.
     */
    template <uint8_t BlockSize>
    struct Lu<BlockSize, -1 >
    {
        static BytesVector getValue(const BytesVector &L)
        {
            if (L.lsb() == 0)
            {
                return L.rightShift(1);
            }

            return L.rightShift(1).Xor(Constant<BlockSize>::lsb_value);
        }
    };

    /* Used only for checking parent / child routine. */
    using BlockCipherBase = BlockCipher<BytesVector, uint64_t, 16, BigEndian64>;

    /*
     * MAC abstract class.
     */
    class MAC
    {
    protected:
        explicit MAC(const BytesVector &key) : key(key) { }
        virtual ~MAC() { }
        virtual BytesVector encode(BytesVector message) = 0;

        /* The main key for encryption. 
         * This key is already checked in block cipher algorithm's constructor. 
         */
        const BytesVector key;
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
        explicit CBC_MAC(const BytesVector &key) : MAC(key) { }
        
        BytesVector encode(BytesVector message) override
        {
            /* Prepare the encryption to be in CBC mode with an IV = 0^{BlockSize}. */
            constexpr uint8_t BlockSize = BlockCipherType::getBlockSize();
            BlockCipherType Block(key, OperationModes::CBC, BytesVector(BlockSize, 0));

            /* Pad the message if empty or not a multiple of BlockSize. */
            const uint64_t message_size = message.size();
            if (message_size == 0 || message_size % BlockSize)
            {
                message = Padding::_10Star(message, BlockSize);
            }

            // We encode in CBC mode the padded message.
            const BytesVector C(Block.encode(message));

            // We keep only the last block of the encryption.
            return C.range(C.size() - BlockSize);
        }
    };

    /*
     * AES-XCBC-MAC mode.
     * Source : http://www.ietf.org/rfc/rfc3566.txt
     */
    class AES_XCBC_MAC : public MAC
    {
    public:
        explicit AES_XCBC_MAC(const BytesVector &key) : MAC(key) { }
        
        BytesVector encode(BytesVector message) override
        {
            AES Block(key);

            /* Encode 3 keys derived from the main key with specific constants for each one. */
            const BytesVector K1 = Block.encodeCurrentBlock(BytesVector(16, 1));
            const BytesVector K2 = Block.encodeCurrentBlock(BytesVector(16, 2));
            const BytesVector K3 = Block.encodeCurrentBlock(BytesVector(16, 3));
            const uint64_t message_size = message.size();

            /* Pad the message if empty or not multiple of 16 since AES works with block of 16 bytes. */
            BytesVector K;
            if (message_size % 16 || message_size == 0)
            {
                message = Padding::_10Star(message, 16);
                K = K3;
            }
            else
            {
                K = K2;
            }

            /* Split the message in sub messages of 16 bytes length.
             * Apply the formula : C[i+1] = E(M[i] XOR C[i]) where C[0] = 0^16. */
            const BytesMatrix M = message.chunk(16);
            BytesVector previous_cipher_block(16, 0);
            BytesVector current_cipher_block;
            const int64_t M_size = M.size();
            AES Block_K1(K1);
            for (int64_t i = 0; i < M_size - 1; ++i)
            {
                current_cipher_block = Block_K1.encodeCurrentBlock(M[i].Xor(previous_cipher_block));
                previous_cipher_block = current_cipher_block;
            }

            return Block_K1.encodeCurrentBlock(M[M_size - 1].Xor(previous_cipher_block).Xor(K));
        }
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
        explicit CMAC(const BytesVector &key) : MAC(key) { }
        
        BytesVector encode(BytesVector message) override
        {
            BlockCipherType Block(key);
            constexpr uint8_t BlockSize = BlockCipherType::getBlockSize();
            BytesVector L = Block.encodeCurrentBlock(BytesVector(BlockSize, 0));
            BytesVector L_u = Lu<BlockSize, 1>::getValue(L);

            const uint64_t message_size = message.size();
            if (message_size == 0 || message_size % BlockSize)
            {
                message = Padding::_10Star(message, BlockSize);
                L_u = Lu<BlockSize, 1>::getValue(L_u);
            }

            const BytesMatrix M = message.chunk(BlockSize);
            BytesVector previous_cipher_block(BlockSize, 0);
            BytesVector current_cipher_block;
            const int64_t M_size = M.size();
            for (int64_t i = 0; i < M_size - 1; ++i)
            {
                current_cipher_block = Block.encodeCurrentBlock(M[i].Xor(previous_cipher_block));
                previous_cipher_block = current_cipher_block;
            }

            return Block.encodeCurrentBlock(M[M_size - 1].Xor(previous_cipher_block).Xor(L_u));
        }
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
        explicit OMAC(const BytesVector &key) : MAC(key) { }
        
        BytesVector encode(BytesVector message) override
        {
            BlockCipherType Block(key);
            constexpr uint8_t BlockSize = BlockCipherType::getBlockSize();
            BytesVector L = Block.encodeCurrentBlock(BytesVector(BlockSize, 0));
            BytesVector L_u = Lu<BlockSize, 1>::getValue(L);

            // If the message is empty or not a multiple of 'BlockSize', the message 
            // is padded to form a complete block.
            const uint64_t message_size = message.size();
            if (message_size == 0 || message_size % BlockSize)
            {
                message = Padding::_10Star(message, BlockSize);
                L_u = Lu<BlockSize, -1>::getValue(L);
            }

            const BytesMatrix M = message.chunk(BlockSize);
            BytesVector previous_cipher_block(BlockSize, 0);
            BytesVector current_cipher_block;
            const int64_t M_size = M.size();
            for (int64_t i = 0; i < M_size - 1; ++i)
            {
                current_cipher_block = Block.encodeCurrentBlock(M[i].Xor(previous_cipher_block));
                previous_cipher_block = current_cipher_block;
            }

            return Block.encodeCurrentBlock(M[M_size - 1].Xor(previous_cipher_block).Xor(L_u));
        }
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
        explicit PMAC(const BytesVector &key) : MAC(key) { }
        
        BytesVector encode(BytesVector message) override
        {
            BlockCipherType Block(key);
            constexpr uint8_t BlockSize = BlockCipherType::getBlockSize();

            // Pad the message if it's not a multiple of BlockSize.
            bool need_padding = false;
            const uint64_t message_size = message.size();
            if (message_size % BlockSize || message_size == 0)
            {
                need_padding = true;
                message = Padding::_10Star(message, BlockSize);
            }

            const BytesMatrix M = message.chunk(BlockSize);
            BytesVector sigma(BlockSize, 0);
            BytesVector L = Block.encodeCurrentBlock(BytesVector(BlockSize, 0));
            const int64_t M_size = M.size();
            for (int64_t i = 0; i < M_size - 1; ++i)
            {
                sigma = Block.encodeCurrentBlock(M[i].Xor(getLuNValue(L, i))).Xor(sigma);
            }
            sigma = M[M_size - 1].Xor(sigma);
            if (!need_padding)
            {
                sigma = Lu<BlockSize, -1 > ::getValue(L).Xor(sigma);
            }

            return Block.encodeCurrentBlock(sigma);
        }

    private:
        /* Get L.u^n from the specs. */
        static BytesVector getLuNValue(const BytesVector &L, const uint64_t &n)
        {
            if (L.msb() == 0)
                return getLuNValue(L.leftShift(1), n - 1);

            return getLuNValue(L.leftShift(1), n - 1).Xor(Constant <BlockCipherType::getBlockSize()>::msb_value);
        }
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
        TMAC(const BytesVector &key, const BytesVector &key2) : MAC(key)
        {
            setKey(key2);
        }
        
        BytesVector encode(BytesVector message) override
        {
            const uint64_t message_size = message.size();
            BlockCipherType Block(key);
            constexpr uint8_t BlockSize = BlockCipherType::getBlockSize();

            BytesVector K;
            if (message_size % BlockSize || message_size == 0)
            {
                K = key2;
                message = Padding::_10Star(message, BlockSize);
            }
            else
            {
                K = Lu<BlockSize, 1>::getValue(key2);
            }

            const BytesMatrix M = message.chunk(BlockSize);
            BytesVector previous_cipher_block(BlockSize, 0);
            BytesVector current_cipher_block;
            const int64_t M_size = M.size();
            for (int64_t i = 0; i < M_size - 1; ++i)
            {
                current_cipher_block = Block.encodeCurrentBlock(M[i].Xor(previous_cipher_block));
                previous_cipher_block = current_cipher_block;
            }

            return Block.encodeCurrentBlock(M[M_size - 1].Xor(previous_cipher_block).Xor(K));
        }
        
        void setKey(const BytesVector &key2)
        {
            const uint8_t key2_length = key2.size();
            if (key2.empty() || key2_length != BlockCipherType::getBlockSize())
            {
                throw BadKeyLength("Your 'key2' has to be the same size as the block cipher uses.", key2_length);
            }

            this->key2 = key2;
        }

    private:
        BytesVector key2;
    };
}
#endif