
#ifndef CIPHER_H
#define CIPHER_H

template <class T>
class Cipher
{
public:
   virtual ~Cipher() {}

   virtual T encode(const T &) = 0;
   virtual T decode(const T &) = 0;
};

#endif // CIPHER_H
