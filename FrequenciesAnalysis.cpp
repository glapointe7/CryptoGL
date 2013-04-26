#include "FrequenciesAnalysis.hpp"

#include <algorithm>
#include <fstream>

#include "Tools.hpp"

FrequenciesAnalysis::FrequenciesAnalysis()
{

}

double FrequenciesAnalysis::getIC() const
{
   return ic;
}

void FrequenciesAnalysis::setAlpha(const std::string &alpha)
{
   this->alpha = alpha;
}

std::map<char, unsigned int> FrequenciesAnalysis::getCharsCount(const std::string &text)
{
   std::map<char, unsigned int> chars_freq;

   for (auto c : alpha)
   {
      unsigned int char_count = std::count(text.begin(), text.end(), c);
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

   for (auto pair : chars_freq)
   {
      ic +=  static_cast<double>(pair.second * (pair.second - 1)) / (text_len * (text_len - 1));
   }
}

std::string FrequenciesAnalysis::load(const std::string &filename)
{
   std::ifstream in(filename.c_str());
   if (in)
   {
      std::string contents;
      in.seekg(0, std::ios::end);
      contents.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();
      if (!contents.empty() && *contents.rbegin() == '\n')
      {
         contents.erase(contents.length() - 1, 1);
      }
      
      std::string chars = " \n\r\t";
      eraseChars(contents, chars);
      std::transform(contents.begin(), contents.end(), contents.begin(), ::toupper);
      
      return contents;
   }
   throw errno;
}