#include "Base64.hpp"

#include "exceptions/BadChar.hpp"

const std::string Base64::alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

const std::string Base64::encode(const BytesContainer &clear_data)
{
   const uint32_t clear_len = clear_data.size();
   std::string crypted;
   crypted.reserve(((clear_len / 3) + (clear_len % 3 > 0)) * 4);

   uint32_t temp;
   BytesContainer::const_iterator cursor = clear_data.begin();
   for (uint32_t idx = 0; idx < clear_len / 3; idx++)
   {
      temp = (*cursor++) << 16; 
      temp += (*cursor++) << 8;
      temp += (*cursor++);
      
      crypted += alpha[(temp & 0x00FC0000) >> 18];
      crypted += alpha[(temp & 0x0003F000) >> 12];
      crypted += alpha[(temp & 0x00000FC0) >> 6 ];
      crypted += alpha[(temp & 0x0000003F)];
   }
   switch (clear_len % 3)
   {
      case 1:
         temp = (*cursor++) << 16; 
         crypted += alpha[(temp & 0x00FC0000) >> 18];
         crypted += alpha[(temp & 0x0003F000) >> 12];
         crypted.append(2, pad_character);
         break;
         
      case 2:
         temp = (*cursor++) << 16; 
         temp += (*cursor++) << 8;
         crypted += alpha[(temp & 0x00FC0000) >> 18];
         crypted += alpha[(temp & 0x0003F000) >> 12];
         crypted += alpha[(temp & 0x00000FC0) >> 6 ];
         crypted += pad_character;
         break;
   }

   return crypted;
}

const Base64::BytesContainer Base64::decode(const std::string &cipher_data)
{
   const uint32_t cipher_len = cipher_data.length();
   if (cipher_len % 4)
   {
      throw "Non-Valid base64!";
   }
   uint32_t padding = 0;
   if (!cipher_data.empty())
   {
      if (cipher_data[cipher_len - 1] == pad_character)
         padding++;
      if (cipher_data[cipher_len - 2] == pad_character)
         padding++;
   }
   // Setup a vector to hold the result.
   BytesContainer decrypted;
   decrypted.reserve(((cipher_len / 4) * 3) - padding);
   
   // Holds decoded quanta.
   uint32_t temp = 0; 
   std::string::const_iterator cursor = cipher_data.begin();
   while (cursor < cipher_data.end())
   {
      for (uint32_t quantumPosition = 0; quantumPosition < 4; quantumPosition++)
      {
         temp <<= 6;
         if (*cursor >= 0x41 && *cursor <= 0x5A) // This area will need tweaking if
            temp |= *cursor - 0x41; // you are using an alternate alphabet
         else if (*cursor >= 0x61 && *cursor <= 0x7A)
            temp |= *cursor - 0x47;
         else if (*cursor >= 0x30 && *cursor <= 0x39)
            temp |= *cursor + 0x04;
         else 
         {
            switch(*cursor)
            {
               case 0x2B:
                  temp |= 0x3E; //change to 0x2D for URL alphabet
                  break;
                  
               case 0x2F:
                  temp |= 0x3F; //change to 0x5F for URL alphabet
                  break;
                  
               case pad_character:
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
               
               default:
                  throw BadChar("Non-Valid Character in Base 64!", *cursor);
            }
         }
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