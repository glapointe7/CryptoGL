/*
 * Frequencies taken on : http://www.letterfrequency.org/
 */
#ifndef FREQUENCIESANALYSIS_HPP
#define	FREQUENCIESANALYSIS_HPP

#include <string>
#include <map>

class FrequenciesAnalysis
{
public:
   virtual ~FrequenciesAnalysis() {}
   
   enum class Language : uint8_t {
      french,
      english,
      spanish,
      german,
      dutch
   };
   
   void calculateIC(const std::string &);
   double getIC() const;
   
   void setAlpha(const std::string &alpha);
   void setLanguage(const Language lang);
   
   const std::map<char, uint32_t> getCharsCount(const std::string &text) const;
   
private:
   double ic = 0;
   std::string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   std::string language = "etaoinsrhldcumfgpywbvkxjqz";
};

#endif

