/*
 * TO DO : Save
 */
#ifndef FREQUENCIESANALYSIS_HPP
#define	FREQUENCIESANALYSIS_HPP

#include <string>
#include <map>

class FrequenciesAnalysis
{
public:
   FrequenciesAnalysis();
   virtual ~FrequenciesAnalysis() {}
   
   void calculateIC(const std::string &);
   double getIC() const;
   
   void setAlpha(const std::string &);
   
   std::map<char, unsigned int> getCharsCount(const std::string &);
   
   static std::string load(const std::string &);
   static void save(const std::string &, const std::string &);
   
private:
   double ic = 0;
   std::string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
};

#endif

