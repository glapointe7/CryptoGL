/* 
 * File:   StringCipher.h
 * Author: gabriel
 *
 * Created on March 23, 2013, 9:12 AM
 */

#ifndef STRINGCIPHER_H
#define	STRINGCIPHER_H

#include <string>

using namespace std;

class StringCipher
{
public:
   StringCipher();
   StringCipher(const StringCipher& orig);
   virtual ~StringCipher() = default;

   virtual string encode()const = 0;
   virtual string decode()const = 0;
   //virtual void setKey(const string key);

protected:
   string cipher_text;
   string clear_text;
   string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
};

#endif	/* STRINGCIPHER_H */

