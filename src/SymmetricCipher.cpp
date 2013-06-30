#include "SymmetricCipher.hpp"

#include <sstream>
#include <fstream>

// FILE old style is faster than the c++ STL ofstream to write bytes in a file.

void SymmetricCipher::save(const std::string &filename, const BytesContainer &data)
{
   FILE *pFile;
   pFile = fopen(filename.c_str(), "wb");
   size_t data_size = data.size();

   fwrite(&data.front(), 1, data_size, pFile);
   fclose(pFile);
}

const SymmetricCipher::BytesContainer
SymmetricCipher::load(const std::string &filename)
{
   BytesContainer bytes;
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