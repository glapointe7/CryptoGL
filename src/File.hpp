
#ifndef FILE_HPP
#define FILE_HPP

#include "Types.hpp"

#include <fstream>

template <class DataType>
class File
{
public:

    File(const String &filename) : filename(filename) { }

    /* Get the file size. */
    static std::ifstream::pos_type size(std::ifstream &in)
    {
        in.seekg(0, std::ios::end);
        const std::ifstream::pos_type file_size = in.tellg();
        in.seekg(0, std::ios::beg);

        return file_size;
    }

    /* Save data in a file named by 'filename'. */
    void save(const DataType &data) const
    {
        std::ofstream out;
        out.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        try
        {
            out.open(filename.c_str(), std::ios::binary);
            out.write(&data.front(), data.size());
        }
        catch (...)
        {
            throw "Cannot open / write the file.";
        }
        out.close();
    }

    /* Load a binary file and get the bytes in a vector. */
    Vector<DataType> load() const
    {
        Vector<DataType> bytes;
        std::ifstream in(filename.c_str());
        if (!in)
        {
            throw Exception("Check if your file exists in the directory you gave.");
        }

        const uint32_t total_size = size(in);
        bytes.reserve(size(in));
        DataType value;
        while (in >> value)
        {
            bytes.push_back(value);
        }
        in.close();

        return bytes;
    }

private:
    const String filename;
};

#endif