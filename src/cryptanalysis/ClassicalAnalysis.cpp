#include "ClassicalAnalysis.hpp"

#include <algorithm>

void ClassicalAnalysis::setAlpha(const ClassicalType &alpha)
{
   this->alpha = alpha;
}

void ClassicalAnalysis::setLanguage(const Language lang)
{
   switch(lang)
   {
      case Language::french: language = "easitnrulodcmpvqgfbhjxyzkw"; break;
      case Language::english: language = "etaoinsrhldcumfgpywbvkxjqz"; break;
      case Language::spanish: language = "eaosrnidlctumpbgyvqhfzjxwk"; break;
      case Language::german: language = "enistradhugmclbofkwvzpjqyx"; break;
      case Language::dutch: language = "entaoirdslghmvbwkupcizjfx"; break;
   }
}

std::map<char, uint32_t> ClassicalAnalysis::getTextFrequency(const ClassicalType &text) const
{
   std::map<char, uint32_t> chars_freq;

   for (const auto c : alpha)
   {
      const uint32_t char_count = std::count(text.begin(), text.end(), c);
      if (char_count > 0)
      {
         chars_freq[c] = char_count;
      }
   }

   return chars_freq;
}

void ClassicalAnalysis::calculateIC(const ClassicalType &text)
{
   const uint32_t text_length = text.length();
   const uint32_t text_length_square = text_length * (text_length - 1);
   const auto chars_freq = getTextFrequency(text);

   for (const auto &pair : chars_freq)
   {
      ic +=  static_cast<float>(pair.second * (pair.second - 1)) / text_length_square;
   }
}