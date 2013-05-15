#include "FrequenciesAnalysis.hpp"

#include <algorithm>
#include <fstream>

#include "Tools.hpp"

double FrequenciesAnalysis::getIC() const
{
   return ic;
}

void FrequenciesAnalysis::setAlpha(const std::string &alpha)
{
   this->alpha = alpha;
}

void FrequenciesAnalysis::setLanguage(const Language &lang)
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

const std::map<char, unsigned int> FrequenciesAnalysis::getCharsCount(const std::string &text) const
{
   std::map<char, unsigned int> chars_freq;

   for (const auto c : alpha)
   {
      const unsigned int char_count = std::count(text.begin(), text.end(), c);
      if (char_count > 0)
      {
         chars_freq[c] = char_count;
      }
   }

   return chars_freq;
}

void FrequenciesAnalysis::calculateIC(const std::string &text)
{
   const unsigned int text_len = text.length();
   const auto chars_freq = getCharsCount(text);

   for (const auto pair : chars_freq)
   {
      ic +=  static_cast<double>(pair.second * (pair.second - 1)) / (text_len * (text_len - 1));
   }
}