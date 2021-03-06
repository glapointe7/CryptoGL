
#include "Fleissner.hpp"

#include "exceptions/BadKeyLength.hpp"

#include <time.h>

using namespace CryptoGL;

// The key is represented by the coordinates of each 'hole' in the grid's mask.

Fleissner::Fleissner(const Coordinates &key, const uint32_t grid_dim, const bool clockwise)
{
    Coordinates coords(key);
    setGridDimension(grid_dim);
    const uint32_t key_size = key.size();
    const uint32_t mask_size_approuved = key_size * 4;

    if (mask_size_approuved != (grid_dim * grid_dim))
    {
        throw BadKeyLength("Your key have to be square following the dimension you provided.", mask_size_approuved);
    }

    // If the rotation coordinates exist, then the mask is not valid. 
    // Mask rotations 270, 180, 90 degrees to check.
    coords.reserve(mask_size_approuved);
    std::set<Cell> rotation;

    if (clockwise)
    {
        for (const auto &xy : key)
        {
            rotationExists(rotation, xy.second, grid_dim - 1 - xy.first);
        }
    }
    else
    {
        for (const auto &xy : key)
        {
            rotationExists(rotation, grid_dim - 1 - xy.second, xy.first);
        }
    }
    coords.insert(coords.end(), rotation.begin(), rotation.end());
    rotation.clear();

    for (const auto &xy : key)
    {
        rotationExists(rotation, grid_dim - 1 - xy.first, grid_dim - 1 - xy.second);
    }
    coords.insert(coords.end(), rotation.begin(), rotation.end());
    rotation.clear();

    if (clockwise)
    {
        for (const auto &xy : key)
        {
            rotationExists(rotation, grid_dim - 1 - xy.second, xy.first);
        }
    }
    else
    {
        for (const auto &xy : key)
        {
            rotationExists(rotation, xy.second, grid_dim - 1 - xy.first);
        }
    }
    coords.insert(coords.end(), rotation.begin(), rotation.end());

    this->key = coords;
}

void Fleissner::setGridDimension(const uint32_t dim)
{
    grid_dim = dim;
}

void Fleissner::rotationExists(std::set<Cell> &rotation, const uint32_t x, const uint32_t y)
{
    const auto is_unique = rotation.insert(std::make_pair(x, y));
    if (is_unique.second == false)
    {
        throw KeyCellNotUnique("The key you provided contains cell that is not unique when rotated.");
    }
}

ClassicalType Fleissner::encode(const ClassicalType &clear_text)
{
    const uint32_t dim = grid_dim * grid_dim;
    ClassicalType full_text(clear_text);
    full_text.reserve(full_text.length() + dim);
    full_text.append(dim, 'A');
    Grid grid(grid_dim, ClassicalType(grid_dim, '.'));

    // If the grid is filled, we clear it and we start the process again.
    const uint32_t clear_len = full_text.length();
    const uint32_t max_grid = clear_len / dim;

    ClassicalType crypted(clear_len);

    uint32_t k = 0;
    for (uint32_t i = 0; i < max_grid; ++i)
    {
        for (uint32_t j = 0; j < dim; ++j, ++k)
        {
            grid[key[j].first][key[j].second] = full_text[k];
        }

        for (const auto &str : grid)
        {
            crypted.append(str);
        }
        grid.clear();
    }

    return crypted;
}

ClassicalType Fleissner::decode(const ClassicalType &cipher_text)
{
    const uint32_t cipher_len = cipher_text.length();
    const uint32_t dim = grid_dim * grid_dim;
    const uint32_t max_grid = cipher_len / dim;

    Grid grid;
    grid.reserve(grid_dim);
    ClassicalType decrypted(cipher_len);
    uint32_t k = 0;
    for (uint32_t i = 0; i < max_grid; ++i)
    {
        // Fill the grid with the cipher text.
        for (uint32_t j = 0; j < grid_dim; ++j, k += grid_dim)
        {
            grid.push_back(ClassicalType(cipher_text.substr(k, grid_dim)));
        }

        for (uint32_t c = 0; c < dim; ++c)
        {
            decrypted.push_back(grid[key[c].first][key[c].second]);
        }
    }

    return decrypted;
}