
#ifndef CIPHERGRID_HPP
#define CIPHERGRID_HPP

#include "Types.hpp"

namespace CryptoGL
{
    using Grid = Vector<ClassicalType>;
    
    /* 2D Coordinates (x, y) used for cipher grids, where x,y < 256 are positive integers. */
    struct Coordinates 
    {
        Coordinates(const uint8_t x, const uint8_t y)
            : x(x), y(y) { }
            
        Coordinates()
            : Coordinates(0, 0) { }
        
        uint8_t x = 0; 
        uint8_t y = 0;
    };
    
    
    /* Class for the creation of a square grid where each cell contains a character. */
    class CipherGrid
    {
    public:
        CipherGrid() { }

        CipherGrid(ClassicalType key, const ClassicalType &alpha);
        explicit CipherGrid(const ClassicalType &alpha) : CipherGrid("", alpha) { }

        /* Build the grid with the string 'chars' and the dimension set given. */
        void setGrid(ClassicalType chars);

        /* Return character coordinates from the cipher grid. */
        Coordinates getCharCoordinates(const char c) const;

        /* With an alphabet given with perfect square length, it sets the dimension of the grid. */
        void setDimension(const ClassicalType &alpha);
        
        /* Return the grid dimension. */
        uint8_t getDimension() const
        {
            return dim;
        }
        
        /* Return the value of grid[x][y]. */
        uint8_t at(const uint8_t x, const uint8_t y) const
        {
            return grid[x][y];
        }

    private:
        // Dimension of the square grid.
        uint8_t dim;

        /* The square grid of dimension 'dim' X 'dim'. */
        Grid grid;
    };
}

#endif