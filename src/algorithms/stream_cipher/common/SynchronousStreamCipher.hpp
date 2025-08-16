#pragma once

#include "StreamCipher.hpp"
#include "../../../core/Utils/Endian.hpp"

namespace CryptoGL
{
    template <typename KeystreamType, typename EndianType, uint32_t KeystreamSize, typename MessageType>
    class SynchronousStreamCipher : public StreamCipher<KeystreamType, MessageType>
    {
    public:
        BytesVector encode(const BytesVector &message) override
        {
            const uint64_t output_size = message.size();
            BytesVector output;
            output.reserve(output_size);

            const uint8_t type_size = sizeof(typename KeystreamType::value_type);
            for (uint64_t i = 0; i < output_size; i += KeystreamSize)
            {
                const KeystreamType keystream = generateKeystream();
                for (uint64_t j = 0; j < KeystreamSize; j += type_size)
                {
                    const BytesVector key_bytes = EndianType::toBytesVector(keystream[j / type_size]);
                    for (uint8_t k = 0; k < type_size; ++k)
                    {
                        const uint64_t current_len = i + j + k;
                        if (current_len == output_size)
                        {
                            return output;
                        }
                        output.push_back(message[current_len] ^ key_bytes[k]);
                    }
                }
            }

            return output;
        }

    protected:       
        virtual KeystreamType generateKeystream() override = 0;

        virtual void keySetup() override = 0;
    };
}