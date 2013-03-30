/* 
 * File:   StringCipher.cpp
 * Author: gabriel
 * 
 * Created on March 23, 2013, 9:12 AM
 */

#include "StringCipher.h"

StringCipher::StringCipher()
{

}

StringCipher::StringCipher(const StringCipher& orig)
{
}

void StringCipher::setAlpha(const string &alpha)
{
   this->alpha = alpha;
}

string StringCipher::getAlpha() const
{
   return alpha;
}
