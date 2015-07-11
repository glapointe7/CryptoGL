/*
 * Abstract class for Stream ciphers.
 */
#ifndef STREAMCIPHER_HPP
#define STREAMCIPHER_HPP

#include "SymmetricCipher.hpp"

namespace CryptoGL
{
    template <class KeystreamType>
    class StreamCipher : public SymmetricCipher
    {
    public:
        /* Since encoding is his own inverse, then decoding is simply a call to encode. */
        BytesVector decode(const BytesVector &message) override
        {
            return encode(message);
        }

    protected:
        /* Initial Vector */
        BytesVector IV;

        virtual BytesVector encode(const BytesVector &) override = 0;

        virtual KeystreamType generateKeystream() = 0;

        virtual void keySetup() = 0;
        
        virtual void setIV(const BytesVector &IV)
        {
            const uint8_t iv_size = IV.size();
            if (iv_size != 8)
            {
                throw BadIVLength("Your IV length has to be 8 bytes.", iv_size);
            }

            this->IV = IV;
        }
    };
}

#endif