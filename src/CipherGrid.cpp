#include "CipherGrid.hpp"

#include "String.hpp"
#include "MathematicalTools.hpp"

#include "exceptions/BadAlphaLength.hpp"

using namespace CryptoGL;

CipherGrid::CipherGrid(ClassicalType key, const ClassicalType &alpha)
{
    setDimension(alpha);
    setGrid(ClassicalType(key.append(alpha)));
}

void CipherGrid::setGrid(ClassicalType chars)
{
    chars.removeDuplicates();
    grid.reserve(dim);

    // Split the string to set a square grid of dimension (dim X dim) of chars.
    for (uint8_t i = 0; i < dim; ++i)
    {
        grid.push_back(ClassicalType(chars.substr(i * dim, dim)));
    }
}

Coordinates CipherGrid::getCharCoordinates(const char c) const
{
    Coordinates coords;
    for (const auto &str : grid)
    {
        const auto found = str.find(c);
        if (found != String::npos)
        {
            coords.x = found;
            return coords;
        }
        coords.y++;
    }

    return coords;
}

void CipherGrid::setDimension(const ClassicalType &alpha)
{
    const uint8_t alpha_size = alpha.length();
    if (!Maths::isPerfectSquare(alpha_size))
    {
        throw BadAlphaLength("CipherGrid (setDimension): The length of your alphabet should be a perfect square.", alpha_size);
    }

    // get grid dimension from alphabet assuming length is a perfect square by the previous IF.
    dim = Maths::getByteSqrt(alpha_size);
}