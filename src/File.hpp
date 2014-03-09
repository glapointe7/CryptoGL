
#ifndef FILE_HPP
#define FILE_HPP

#include "Types.hpp"

#include <fstream>
#include <iostream>

template <class DataType>
class File
{
public:
   explicit File(const std::string &filename) : filename(filename) {}
   
   /* Get the file size. */
   static std::ifstream::pos_type size(std::ifstream in)
   {
      in.seekg(0, std::ios::end);
      return in.tellg();
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
      catch (const std::ofstream::failure &e)
      {
         std::cerr << "Cannot open / write the file.";
      }
      out.close();
   }
   
   /* Load a binary file and get the bytes in a vector. */
   DataType load() const
   {
      DataType bytes;
      std::ifstream in;
      in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      try
      {
         in.open(filename.c_str(), std::ios::binary);
         bytes.reserve(size(in));
         in.read(reinterpret_cast<char *> (&bytes.front()), bytes.size());
      }
      catch (const std::ifstream::failure &e)
      {
         std::cerr << "Cannot open / read the file.";
      }
      in.close();

      return bytes;
   }
   
private:
   const std::string filename;
};

#endif