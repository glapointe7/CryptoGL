
#ifndef FLEISSNER_HPP
#define FLEISSNER_HPP

#include "StringCipher.hpp"
#include "CipherGrid.hpp"

#include "exceptions/BadKey.hpp"

#include <set>

namespace CryptoGL
{
    class Fleissner : public StringCipher
    {
        using CoordinatesSet = std::set<Coordinates>;
        using CoordinatesVector = Vector<Coordinates>;
        using Grid = Vector<ClassicalType>;
        using KeyCellNotUnique = BadKey;

    public:
        Fleissner(const CoordinatesVector &key, const uint32_t grid_dim, const bool clockwise);

        ClassicalType encode(const ClassicalType &clear_text) override;
        ClassicalType decode(const ClassicalType &cipher_text) override;

    private:
        static void rotationExists(CoordinatesSet &cmp, const uint32_t x, const uint32_t y);

        // Dimension of the cipher grid.
        const uint32_t grid_dim;

        CoordinatesVector key;
    };
}

#endif