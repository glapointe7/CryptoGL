#include "Morse.hpp"

#include <algorithm>

using namespace CryptoGL;

const std::array<ClassicalType, 36> Morse::MORSE_CODES = {
    {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
        ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
        "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----",
        ".----", "..---", "...--", "....-", ".....", "-....", "--...",
        "---..", "----."}
};

Morse::Morse()
{
    setAlpha(ClassicalType::UPPERCASE_DIGITS);
}

ClassicalType Morse::encode(const ClassicalType &clear_text)
{
    ClassicalType crypted(8 * clear_text.length());
    for (const auto c : clear_text)
    {
        const uint32_t found = alpha.find(c);
        crypted.append(MORSE_CODES[found]);
        crypted.push_back(' ');
    }

    return crypted;
}

ClassicalType Morse::decode(const ClassicalType &cipher_text)
{
    ClassicalType decrypted(cipher_text.length() / 2);
    const Vector<ClassicalType> cipher_word = cipher_text.split(' ');

    for (const auto &str : cipher_word)
    {
        const uint32_t pos = std::distance(MORSE_CODES.begin(), std::find(MORSE_CODES.begin(), MORSE_CODES.end(), str));
        decrypted.push_back(alpha[pos]);
    }

    return decrypted;
}