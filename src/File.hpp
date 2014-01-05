
#ifndef FILE_HPP
#define FILE_HPP

#include "Types.hpp"

#include <fstream>
#include <iostream>

namespace File
{
   /* Get the file size. */
   std::ifstream::pos_type getSize(std::ifstream in)
   {
      in.seekg(0, std::ios::end);
      return in.tellg();
   }
   
   /* Save data in a file named by 'filename'. */
   template <class DataType>
   void save(const std::string &filename, const DataType &data)
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
   template <class DataType>
   DataType load(const std::string &filename)
   {
      DataType bytes;
      std::ifstream in;
      in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      try
      {
         in.open(filename.c_str(), std::ios::binary);
         bytes.reserve(getSize(in));
         in.read(reinterpret_cast<char *> (&bytes.front()), bytes.size());
      }
      catch (const std::ifstream::failure &e)
      {
         std::cerr << "Cannot open / read the file.";
      }
      in.close();

      return bytes;
   }
}

#endif