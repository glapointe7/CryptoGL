/*
 * Frequencies taken on : http://www.letterfrequency.org/
 */
#ifndef CLASSICALANALYSIS_HPP
#define CLASSICALANALYSIS_HPP

#include <map>

#include "../Types.hpp"

class ClassicalAnalysis
{  
public:
   virtual ~ClassicalAnalysis() {}
   
   enum class Language : uint8_t 
   {
      french,
      english,
      spanish,
      german,
      dutch
   };
   
   void calculateIC(const ClassicalType &text);
   float getIC() const { return ic; };
   
   void setAlpha(const ClassicalType &alpha);
   void setLanguage(const Language lang);
   
   std::map<char, uint32_t> getTextFrequency(const ClassicalType &text) const;
   
private:
   float ic = 0;
   ClassicalType alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   ClassicalType language = "etaoinsrhldcumfgpywbvkxjqz";
};

#endif