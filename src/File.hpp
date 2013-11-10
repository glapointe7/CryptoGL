
#ifndef FILE_HPP
#define FILE_HPP

#include "Types.hpp"

#include <string>
#include <fstream>

namespace File
{
   /* Save a vector of bytes in a binary file. */
   void saveBytes(const std::string &filename, const BytesVector &data)
   {
      FILE *pFile;
      pFile = fopen(filename.c_str(), "wb");
      size_t data_size = data.size();

      fwrite(&data.front(), 1, data_size, pFile);
      fclose(pFile);
   }
   
   /* Load a binary file and get the bytes in a vector. */
   BytesVector loadBytes(const std::string &filename)
   {
      BytesVector bytes;
      std::ifstream in(filename.c_str(), std::ios::binary);

      try
      {
         in.seekg(0, std::ios::end);
         bytes.resize(in.tellg());
         in.seekg(0, std::ios::beg);
         in.read(reinterpret_cast<char *> (&bytes.front()), bytes.size());
         in.close();
      }
      catch (const std::exception &e)
      {
         throw e.what();
      }

      return bytes;
   }
   
   /* Save a string in a file named by 'filename'. */
   void saveString(const std::string &filename, const std::string &data)
   {
      try
      {
         std::ofstream out(filename.c_str());
         out << data;
         out.close();
      }
      catch (const std::exception &e)
      {
         throw e.what();
      }
   }

   /* Load a file in a string. */
   std::string loadString(const std::string &filename)
   {
      std::string contents;
      std::ifstream in(filename.c_str(), std::ios::binary);

      in.seekg(0, std::ios::end);
      const std::ifstream::pos_type file_size = in.tellg();
      contents.reserve(file_size);
      contents.resize(file_size);
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], file_size);
      in.close();

      return contents;
   }
}

#endif