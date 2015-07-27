#include "Base64.hpp"

#include "exceptions/BadChar.hpp"

using namespace CryptoGL;

ClassicalType Base64::encode(const BytesVector &clear_data)
{
    const uint32_t clear_len = clear_data.size();
    ClassicalType crypted;
    crypted.reserve(((clear_len / 3) + (clear_len % 3 > 0)) * 4);
    
    for (uint32_t i = 0; i < clear_len - 2; i += 3)
    {
        uint32_t temp = 0;
        for(int8_t j = 2; j >= 0; --j)
        {
            temp += clear_data[i + 2 - j] << (8 * j);
        }

        crypted.push_back(ClassicalType::BASE64_ALPHABET[(temp & 0xFC0000) >> 18]);
        crypted.push_back(ClassicalType::BASE64_ALPHABET[(temp & 0x03F000) >> 12]);
        crypted.push_back(ClassicalType::BASE64_ALPHABET[(temp & 0x000FC0) >> 6]);
        crypted.push_back(ClassicalType::BASE64_ALPHABET[(temp & 0x00003F)]);
    }
    
    uint32_t temp = 0;
    switch (clear_len % 3)
    {
        case 1:
            temp = clear_data.back() << 16;
            crypted.push_back(ClassicalType::BASE64_ALPHABET[(temp & 0xFC0000) >> 18]);
            crypted.push_back(ClassicalType::BASE64_ALPHABET[(temp & 0x03F000) >> 12]);
            crypted.append(2, PADDING_CHARACTER);
            break;

        case 2:
            temp = clear_data[clear_len - 2] << 16;
            temp += clear_data.back() << 8;
            crypted.push_back(ClassicalType::BASE64_ALPHABET[(temp & 0xFC0000) >> 18]);
            crypted.push_back(ClassicalType::BASE64_ALPHABET[(temp & 0x03F000) >> 12]);
            crypted.push_back(ClassicalType::BASE64_ALPHABET[(temp & 0x000FC0) >> 6]);
            crypted.push_back(PADDING_CHARACTER);
            break;
    }

    return crypted;
}

BytesVector Base64::decode(const ClassicalType &cipher_data)
{
    const uint32_t cipher_len = cipher_data.length();
    if (cipher_len % 4)
    {
        throw Exception("The cipher must be a multiple of 4 to be a valid base64.");
    }
        
    BytesVector decrypted;
    decrypted.reserve(((cipher_len / 4) * 3) - countPaddingCharacters(cipher_data));

    uint32_t temp = 0;
    for(uint32_t j = 0; j < cipher_len; j += 4)
    {
        for (uint8_t i = 0; i < 4; ++i)
        {
            const char byte = cipher_data[i + j];
            temp <<= 6;
            if (byte >= 0x41 && byte <= 0x5A) 
            {
                temp |= byte - 0x41;
            }
            else if (byte >= 0x61 && byte <= 0x7A)
            {
                temp |= byte - 0x47;
            }
            else if (byte >= 0x30 && byte <= 0x39)
            {
                temp |= byte + 0x04;
            }
            else
            {
                switch (byte)
                {
                    case 0x2B:
                        temp |= 0x3E; //change to 0x2D for URL alphabet
                        break;

                    case 0x2F:
                        temp |= 0x3F; //change to 0x5F for URL alphabet
                        break;

                    case PADDING_CHARACTER:
                    {
                        switch(cipher_len - i - j)
                        {
                            case 1:
                                decrypted.push_back((temp >> 16) & 0xFF);
                                decrypted.push_back((temp >> 8) & 0xFF);
                                return decrypted;

                            case 2:
                                decrypted.push_back((temp >> 10) & 0xFF);
                                return decrypted;

                            default:
                                throw Exception("Invalid padding in base 64.");
                        }
                    }

                    default:
                        throw BadChar("Invalid character in base 64.", byte);
                }
            }
        }
        decrypted.push_back((temp >> 16) & 0xFF);
        decrypted.push_back((temp >> 8) & 0xFF);
        decrypted.push_back((temp) & 0xFF);
    }
    
    return decrypted;
}

uint8_t Base64::countPaddingCharacters(const ClassicalType &cipher_data)
{
    uint8_t padding = 0;
    if (!cipher_data.empty())
    {
        const uint32_t cipher_len = cipher_data.size();
        if (cipher_data[cipher_len - 1] == PADDING_CHARACTER)
        {
            padding++;
        }
        
        if (cipher_data[cipher_len - 2] == PADDING_CHARACTER)
        {
            padding++;
        }
    }
    
    return padding;
}