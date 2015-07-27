#include "Transposition.hpp"

#include <set>

using namespace CryptoGL;

Transposition::Table Transposition::createTable(ClassicalType data)
{
    const uint32_t key_len = key.size();
    data = appendChars(data, key_len, 'X');
    const uint32_t data_len = data.length();
    Table table;
    table.reserve(data_len / key_len);

    // We fill the grid which will be read by column. 
    for (uint32_t i = 0; i < data_len; i += key_len)
    {
        table.push_back(data.substr(i, key_len));
    }

    return table;
}

Transposition::Table Transposition::createIncompleteTable(const ClassicalType &data)
{
    const uint32_t key_len = key.size();
    const uint32_t data_len = data.length();
    const uint32_t rows = data_len / key_len;
    Table table;
    table.reserve(rows);

    for (uint32_t i = 0; i < data_len - key_len; i += key_len)
    {
        table.push_back(data.substr(i, key_len));
    }
    table.push_back(data.substr(rows * key_len));

    return table;
}

ClassicalType Transposition::readPermutedTable(const Table &table)
{
    ClassicalType data;
    data.reserve(table.size() * key.size());
    for (const auto &str : table)
    {
        data.append(str);
    }

    return data;
}

ClassicalType TranspositionCompleteRows::readPermutedTable(const Table &table)
{
    ClassicalType data;
    data.reserve(table.size() * key.size());
    for (const auto &str : table)
    {
        for (const auto k : key)
        {
            data.push_back(str[k]);
        }
    }

    return data;
}

ClassicalType TranspositionIncompleteRows::readPermutedTable(const Table &table)
{
    const uint32_t key_len = key.size();
    const uint32_t rows = table.size();
    ClassicalType data;
    data.reserve(rows * key_len);

    for (uint32_t i = 0; i < rows - 1; ++i)
    {
        for (const auto k : key)
        {
            data.push_back(table[i][k]);
        }
    }

    const uint32_t rest = table[rows - 1].length();
    const std::set<uint32_t> last_row_sorted(key.begin(), key.begin() + rest);
    for (uint32_t j = 0; j < rest; ++j)
    {
        const auto it = std::next(last_row_sorted.begin(), key[j]);
        const auto pos = std::distance(key.begin(), std::find(key.begin(), key.end(), *it));
        data.push_back(table[rows - 1][pos]);
    }

    return data;
}

/* Implementation of TranspositionColumns */

Transposition::Table TranspositionCompleteColumns::createTable(ClassicalType data)
{
    const uint32_t key_len = key.size();
    const uint32_t data_len = data.length();
    const uint32_t rows = data_len / key_len;
    Table table(rows, ClassicalType(key_len, '.'));

    uint32_t k = 0;
    for (uint32_t i = 0; i < rows; ++i)
    {
        for (uint32_t j = i; j < data_len; j += rows)
        {
            const auto pos = std::distance(key.begin(), std::find(key.begin(), key.end(), k));
            table[i][pos] = data[j];
            k = (k + 1) % key_len;
        }
    }

    return table;
}

ClassicalType TranspositionCompleteColumns::readPermutedTable(const Table &table)
{
    const uint32_t key_len = key.size();
    const uint32_t rows = table.size();
    ClassicalType data;
    data.reserve(rows * key_len);

    for (uint32_t i = 0; i < key_len; ++i)
    {
        const auto pos = std::distance(key.begin(), std::find(key.begin(), key.end(), i));
        for (uint32_t j = 0; j < rows; ++j)
        {
            data.push_back(table[j][pos]);
        }
    }

    return data;
}

/* Implementation of TranspositionIncompleteColumns */

Transposition::Table
TranspositionIncompleteColumns::createIncompleteTable(const ClassicalType &data)
{
    const uint32_t key_len = key.size();
    const uint32_t data_len = data.length();
    const uint32_t rows = (data_len / key_len) + 1;
    const uint32_t rest = data_len % key_len;
    Table table(rows - 1, ClassicalType(key_len, '.'));
    table.push_back(ClassicalType(rest, '.'));

    uint32_t k = 0;
    for (uint32_t i = 0; i < key_len; ++i)
    {
        const auto pos = std::distance(key.begin(), std::find(key.begin(), key.end(), i));
        uint32_t total = rows;
        if (pos >= rest)
        {
            total--;
        }
        for (uint32_t j = 0; j < total; ++j, ++k)
        {
            table[j][pos] = data[k];
        }
    }

    return table;
}

ClassicalType
TranspositionIncompleteColumns::readPermutedTable(const Table &table)
{
    const uint32_t key_len = key.size();
    const uint32_t rows = table.size();
    const uint32_t last_row = table[rows - 1].length();
    ClassicalType data;
    data.reserve(rows * key_len);

    for (uint32_t i = 0; i < key_len; ++i)
    {
        uint32_t total = rows;
        const auto pos = std::distance(key.begin(), std::find(key.begin(), key.end(), i));
        if (pos >= last_row)
        {
            total = rows - 1;
        }

        for (uint32_t j = 0; j < total; ++j)
        {
            data.push_back(table[j][pos]);
        }
    }

    return data;
}