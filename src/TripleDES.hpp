#pragma once

#include "SymmetricCipher.hpp"
#include "BlockCipherModes.hpp"

namespace CryptoGL
{
    class TripleDES : public SymmetricCipher
    {
    public:
        TripleDES(const BytesVector &key1, const BytesVector &key2, const BytesVector &key3);

        BytesVector encode(const BytesVector &clear_text) override;
        BytesVector decode(const BytesVector &cipher_text) override;

        void setKey(const BytesVector &key) override;

        void setOperationMode(const OperationModes mode);

    private:
        BytesVector key2, key3;
        OperationModes mode = OperationModes::ECB;
    };
}