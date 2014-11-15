
#ifndef CIPHERGRID_HPP
#define CIPHERGRID_HPP

#include "Types.hpp"

namespace CryptoGL
{
    using Coordinates = std::pair<uint8_t, uint8_t>;
    using Grid = std::vector<ClassicalType>;

    class CipherGrid
    {
    public:
       CipherGrid() {} 

       CipherGrid(ClassicalType key, const ClassicalType &alpha);

       explicit CipherGrid(const ClassicalType &alpha) : CipherGrid("", alpha) {}

       virtual ~CipherGrid() {}

       /* Build the grid with the string 'chars' and the dimension set given. */
       void setGrid(ClassicalType chars);

       /* Return character coordinates from the cipher grid. */
       Coordinates getCharCoordinates(const char c) const;

       /* With an alphabet given with perfect square length, it sets the dimension of the grid. */
       void setDimension(const ClassicalType &alpha);

       /* Return the grid dimension. */
       uint8_t getDimension() const { return dim; }

       /* Return the value of grid[x][y]. */
       uint8_t at(const uint8_t x, const uint8_t y) const { return grid[x][y]; }

    private:
       // Dimension of the square grid.
       uint8_t dim;

       /* The square grid of dimension 'dim' X 'dim'. */
       Grid grid;
    };
}

#endif