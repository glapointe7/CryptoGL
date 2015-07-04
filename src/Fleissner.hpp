
#ifndef FLEISSNER_HPP
#define FLEISSNER_HPP

#include "StringCipher.hpp"

#include "exceptions/BadKey.hpp"

#include <set>

namespace CryptoGL
{

    class Fleissner : public StringCipher
    {
        using Cell = std::pair<uint32_t, uint32_t>;
        using Coordinates = Vector<Cell>;
        using Grid = Vector<ClassicalType>;
        using KeyCellNotUnique = BadKey;

    public:
        Fleissner(const Coordinates &key, const uint32_t grid_dim, const bool clockwise);

        ClassicalType encode(const ClassicalType &clear_text) override;
        ClassicalType decode(const ClassicalType &cipher_text) override;

        void setGridDimension(const uint32_t dim);

    private:
        static void rotationExists(std::set<Cell> &cmp, const uint32_t x, const uint32_t y);

        // Dimension of the cipher grid.
        uint32_t grid_dim;

        Coordinates key;
    };
}

#endif