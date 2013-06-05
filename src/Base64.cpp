#include "Base64.hpp"

const uint8_t Base64::pad_character = '=';
const std::string Base64::alpha = String::base64_alphabet;

const std::string Base64::encode(const BytesContainer &clear_data)
{
   const uint32_t clear_len = clear_data.size();
   std::string crypted;
   crypted.reserve(((clear_len / 3) + (clear_len % 3 > 0)) * 4);

   uint32_t temp;
   BytesContainer::const_iterator cursor = clear_data.begin();
   for (uint32_t idx = 0; idx < clear_len / 3; idx++)
   {
      temp = (*cursor++) << 16; //Convert to big endian
      temp += (*cursor++) << 8;
      temp += (*cursor++);
      crypted.append(1, alpha[(temp & 0x00FC0000) >> 18]);
      crypted.append(1, alpha[(temp & 0x0003F000) >> 12]);
      crypted.append(1, alpha[(temp & 0x00000FC0) >> 6 ]);
      crypted.append(1, alpha[(temp & 0x0000003F) ]);
   }
   switch (clear_data.size() % 3)
   {
      case 1:
         temp = (*cursor++) << 16; //Convert to big endian
         crypted.append(1, alpha[(temp & 0x00FC0000) >> 18]);
         crypted.append(1, alpha[(temp & 0x0003F000) >> 12]);
         crypted.append(2, pad_character);
         break;
      case 2:
         temp = (*cursor++) << 16; //Convert to big endian
         temp += (*cursor++) << 8;
         crypted.append(1, alpha[(temp & 0x00FC0000) >> 18]);
         crypted.append(1, alpha[(temp & 0x0003F000) >> 12]);
         crypted.append(1, alpha[(temp & 0x00000FC0) >> 6 ]);
         crypted.append(1, pad_character);
         break;
   }

   return crypted;
}

const Base64::BytesContainer Base64::decode(const std::string &cipher_data)
{
   const uint32_t cipher_len = cipher_data.length();
   if (cipher_len % 4) //Sanity check
      throw "Non-Valid base64!";
   size_t padding = 0;
   if (cipher_len)
   {
      if (cipher_data[cipher_len - 1] == pad_character)
         padding++;
      if (cipher_data[cipher_len - 2] == pad_character)
         padding++;
   }
   //Setup a vector to hold the result
   BytesContainer decrypted;
   decrypted.reserve(((cipher_len / 4)*3) - padding);
   uint32_t temp = 0; //Holds decoded quanta
   std::string::const_iterator cursor = cipher_data.begin();
   while (cursor < cipher_data.end())
   {
      for (size_t quantumPosition = 0; quantumPosition < 4; quantumPosition++)
      {
         temp <<= 6;
         if (*cursor >= 0x41 && *cursor <= 0x5A) // This area will need tweaking if
            temp |= *cursor - 0x41; // you are using an alternate alphabet
         else if (*cursor >= 0x61 && *cursor <= 0x7A)
            temp |= *cursor - 0x47;
         else if (*cursor >= 0x30 && *cursor <= 0x39)
            temp |= *cursor + 0x04;
         else if (*cursor == 0x2B)
            temp |= 0x3E; //change to 0x2D for URL alphabet
         else if (*cursor == 0x2F)
            temp |= 0x3F; //change to 0x5F for URL alphabet
         else if (*cursor == pad_character) //pad
         {
            switch (cipher_data.end() - cursor)
            {
               case 1: //One pad character
                  decrypted.push_back((temp >> 16) & 0x000000FF);
                  decrypted.push_back((temp >> 8) & 0x000000FF);
                  return decrypted;
               case 2: //Two pad characters
                  decrypted.push_back((temp >> 10) & 0x000000FF);
                  return decrypted;
               default:
                  throw "Invalid Padding in Base 64!";
            }
         }
         else
            throw "Non-Valid Character in Base 64!";
         cursor++;
      }
      decrypted.push_back((temp >> 16) & 0x000000FF);
      decrypted.push_back((temp >> 8) & 0x000000FF);
      decrypted.push_back((temp) & 0x000000FF);
   }
   return decrypted;
}

const Base64::BytesContainer
Base64::getBytesFromString(const std::string &str)
{
   return BytesContainer(str.begin(), str.end());
}

const std::string Base64::getStringFromBytes(const BytesContainer &bytes)
{
   return std::string(bytes.begin(), bytes.end());
}