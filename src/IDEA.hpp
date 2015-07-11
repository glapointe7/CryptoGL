
#ifndef IDEA_HPP
#define IDEA_HPP

#include "BlockCipher.hpp"

namespace CryptoGL
{
    class IDEA : public BlockCipher<uint16_t, UInt16Vector, 8, BigEndian16>
    {
    public:
        /* Constructor with an IV needed : Only CBC, CFB and OFB modes are accepted. */
        IDEA(const BytesVector &key, const OperationModes mode, const BytesVector &IV)
            : BlockCipher(mode, 8, IV)
        {
            setKey(key);
        }
        
        explicit IDEA(const BytesVector &key)
            : IDEA(key, OperationModes::ECB, {}) { }
        
        IDEA(const BytesVector &key, const OperationModes mode)
            : IDEA(key, mode, {}) { }

    private:
        void generateSubkeys() override;
        void generateInverseSubkeys() override;
        
        void processEncodingCurrentBlock() override;
        void processDecodingCurrentBlock() override;

        /* Decryption subkeys. */
        UInt16Vector decoded_subkeys;
    };
}

#endif