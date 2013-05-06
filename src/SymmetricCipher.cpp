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

const std::string SymmetricCipher::hexDigest(const BytesContainer &bytes)
{
   std::stringstream ss;

   for (const auto byte : bytes)
   {
      ss.setf(std::ios::hex, std::ios::basefield);
      ss.fill('0');
      ss.width(2);
      ss << std::uppercase << static_cast<uint16_t> (byte);
   }

   return ss.str();
}

const SymmetricCipher::BytesContainer
SymmetricCipher::getBytesFromString(const std::string &str)
{
   return BytesContainer(str.begin(), str.end());
}

const std::string SymmetricCipher::getStringFromBytes(const BytesContainer &bytes)
{
   return std::string(bytes.begin(), bytes.end());
}

// Extract bytes from the hex string.

const SymmetricCipher::BytesContainer
SymmetricCipher::getBytesFromHexDigest(const std::string &hex_str)
{
   const uint32_t hex_len = hex_str.length();
   BytesContainer bytes;
   bytes.reserve(hex_len >> 1);

   for (uint32_t i = 0; i < hex_len; i += 2)
   {
      const std::string hexa = hex_str.substr(i, 2);
      std::stringstream ss(hexa);
      ss.setf(std::ios::hex, std::ios::basefield);
      uint16_t x;
      ss >> x;
      bytes.push_back(static_cast<uint8_t> (x));
   }

   return bytes;
}