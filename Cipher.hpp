
#ifndef CIPHER_H
#define CIPHER_H

#include <string>

template <class T>
class Cipher
{
public:
   virtual ~Cipher() {}

private:
   virtual T encode(const T &) = 0;
   virtual T decode(const T &) = 0;

   virtual void save(const std::string &) = 0;
   virtual T load(const std::string &) = 0;
};

#endif // CIPHER_H
