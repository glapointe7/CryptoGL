#include "Playfair.hpp"

#include "MathematicalTools.hpp"

using namespace CryptoGL;

ClassicalType Playfair::encode(const ClassicalType &clear_text)
{
    ClassicalType full_text = appendChars(clear_text, 2, 'X');
    uint32_t clear_len = full_text.length();
    ClassicalType crypted(clear_len);
    const uint8_t dim = grid.getDimension();

    for (uint32_t i = 0; i < clear_len; i += 2)
    {
        // Get the coordinates (x,y) and (a,b) of the digram's letters from the cipher grid.
        const auto X = grid.getCharCoordinates(full_text[i]);
        const auto A = grid.getCharCoordinates(full_text[i + 1]);

        // Let X = (x,y) and A = (a,b).
        // Rule 1 : If x != a AND y != b, then X = (x,b) and A = (a,y).
        if (X.x != A.x && A.y != X.y)
        {
            crypted.push_back(grid.at(X.y, A.x));
            crypted.push_back(grid.at(A.y, X.x));
        }
            // Rule 2 : If x = a AND y != b, then X = (x+1,y) and A = (a+1,b).
        else if (X.x == A.x && X.y != A.y)
        {
            const uint8_t x = (X.y + 1) % dim;
            const uint8_t y = (A.y + 1) % dim;
            crypted.push_back(grid.at(x, X.x));
            crypted.push_back(grid.at(y, A.x));
        }
            // Rule 3 : If x != a AND y = b, then X = (x,y+1) and A = (a,b+1).
        else if (X.x != A.x && A.y == X.y)
        {
            const uint8_t x = (X.x + 1) % dim;
            const uint8_t y = (A.x + 1) % dim;
            crypted.push_back(grid.at(X.y, x));
            crypted.push_back(grid.at(A.y, y));
        }
            // Rule 4 : If x = a AND y = b, then X = (x,y) and A = NULL = X.
        else
        {
            full_text.insert(full_text.begin() + i + 1, 'X');
            i -= 2;
            clear_len++;
        }
    }

    return crypted;
}

ClassicalType Playfair::decode(const ClassicalType &cipher_text)
{
    const uint32_t cipher_len = cipher_text.length();
    ClassicalType decrypted(cipher_len);
    const uint8_t dim = grid.getDimension();

    for (uint32_t i = 0; i < cipher_len; i += 2)
    {
        // Get the coordinates (x,y) and (a,b) of the digram letters from the cipher grid.
        const auto X = grid.getCharCoordinates(cipher_text[i]);
        const auto A = grid.getCharCoordinates(cipher_text[i + 1]);

        // Let A = (x,y) and B = (a,b)
        // Rule 1 : If x != a AND y != b, then A = (x,b) and B = (a,y).
        if (X.x != A.x && X.y != A.y)
        {
            decrypted.push_back(grid.at(X.y, A.x));
            decrypted.push_back(grid.at(A.y, X.x));
        }
            // Rule 2 : If x = a AND y != b, then A = (x+1,y) and B = (a+1,b).
        else if (X.x == A.x && X.y != A.y)
        {
            decrypted.push_back(grid.at((X.y + dim - 1) % dim, X.x));
            decrypted.push_back(grid.at((A.y + dim - 1) % dim, A.x));
        }
            // Rule 3 : If x != a AND y = b, then A = (x,y+1) and B = (a,b+1).
        else if (X.x != A.x && X.y == A.y)
        {
            decrypted.push_back(grid.at(X.y, (X.x + dim - 1) % dim));
            decrypted.push_back(grid.at(A.y, (A.x + dim - 1) % dim));
        }
    }

    return decrypted;
}