/*
 * HMAC source : http://tools.ietf.org/html/rfc2104
 */
#ifndef HASHFUNCTION_HPP
#define HASHFUNCTION_HPP

#include "Types.hpp"

#include "Endian.hpp"
#include "Padding.hpp"

namespace CryptoGL
{
    /* 
     * DataType : An unsigned integer for the IV and integer input block.
     * EndianType : An endian class type : BigEndian<DataType> or LittleEndian<DataType>.
     */
    template <typename DataType, class EndianType>
    class HashFunction
    {
        static_assert(std::is_integral<DataType>::value, "HashFunction: The type 'DataType' must be an integral type.");

    protected:
        using DataTypeVector = Vector<DataType>;
        
        HashFunction(const uint8_t input_block_size, const uint8_t output_size, const DataTypeVector &IV)
            : output_size(output_size), input_block_size(input_block_size), IV(IV) { }
        
        virtual ~HashFunction() { }

        /* Hash the initial message. */
        virtual BytesVector encode(const BytesVector &message) = 0;

        /* Take the current block of data and an initial state and return the hashed block (not final) in 'state'. */
        virtual void compress(DataTypeVector &state) = 0;

        /* Output hash size in bytes. */
        const uint8_t output_size;

        /* Size of the input block to be hashed. */
        const uint8_t input_block_size;

        /* Current block to process the encoding algorithm. */
        DataTypeVector current_block;
        
        /* Append length in bits of the initial message to the padded message. */
        template <class EndianLengthType>
        static BytesVector appendLength(BytesVector bytes, const uint64_t &length)
        {
            bytes.extend(EndianLengthType::toBytesVector(length));

            return bytes;
        }
        
        /* Pad the initial message. */
        virtual BytesVector pad(BytesVector data) const
        {
            //return Padding::_10Star(data, input_block_size);
            const uint64_t data_len = data.size();
            const uint8_t rest = 2 * sizeof (DataType);
            const uint16_t block_size = input_block_size * 2;
            data.reserve(data_len + block_size);

            /* Append '1' bit to the message. */
            data.push_back(0x80);

            // Pad with '0' bits at the end of bits_pad until the length is 448 (mod 512).
            const uint8_t bytes_pad_len = (block_size - rest - ((data_len + 1)
                    % input_block_size)) % input_block_size;
            data.insert(data.end(), bytes_pad_len + rest - 8, 0);

            return data;
        }
        
        /* Transform the input bytes to input block of integers. */
        DataTypeVector convertToIntegersBlock(const BytesVector &bytes, const uint64_t &block_index) const
        {
            return EndianType::toIntegersVector(bytes, block_index, input_block_size);
        }
        
        /* Get the output hash with a specific size. */
        virtual BytesVector getOutput(const DataTypeVector &hash) const
        {
            return EndianType::toBytesVector(hash, output_size / sizeof (DataType));
        }

    public:
        /* Get and set for the IV. */
        DataTypeVector getIV() const
        {
            return IV;
        }
        
        void setIV(const DataTypeVector &IV)
        {
            this->IV = IV;
        }
        
        /* Getter to get the current block (will be used by SEAL keysetup and Gamma methods. */
        DataTypeVector getCurrentBlock() const
        {
            return current_block;
        }
        
        /* Setter to get the current block (will be used by SEAL keysetup and Gamma methods. */
        void setCurrentBlock(const DataTypeVector &current_block)
        {
            this->current_block = current_block;
        }
        
        /* Encode a message with a key and return the HMAC. */
        BytesVector hmacEncode(BytesVector hmac_key, const BytesVector &message)
        {
            if (hmac_key.size() > input_block_size)
            {
                hmac_key = encode(hmac_key);
            }

            BytesVector out_key_pad(input_block_size, 0x5C);
            BytesVector in_key_pad(input_block_size, 0x36);

            const uint8_t key_length = hmac_key.size();
            for (uint8_t i = 0; i < key_length; ++i)
            {
                out_key_pad[i] ^= hmac_key[i];
                in_key_pad[i] ^= hmac_key[i];
            }

            in_key_pad.extend(message);
            out_key_pad.extend(encode(in_key_pad));

            return encode(out_key_pad);
        }

    private:
        /* Initial vector. */
        DataTypeVector IV;
    };
}

#endif