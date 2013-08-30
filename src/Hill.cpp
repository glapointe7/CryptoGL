
#include "Hill.hpp"

#include "exceptions/EmptyMatrix.hpp"
#include "exceptions/MatrixNotSquare.hpp"
#include "exceptions/MatrixKeyNotReversible.hpp"
#include "exceptions/EmptyKey.hpp"

#include "MathematicalTools.hpp" 

void Hill::setKey(const Int32Matrix &key)
{
   try
   {
      this->key->setMatrix(key);
      this->key->setModulo(alpha.length());
   }
   catch (EmptyMatrix & EM)
   {
      throw EM.what();
   }
   catch (MatrixNotSquare &MNS)
   {
      throw MNS.what();
   }

   if (!Maths::areCoprimes(this->key->det(), this->key->getModulo()))
   {
      throw MatrixKeyNotReversible("Your matrix key should be reversible to be able to decode the message.");
   }
}

/* Process encode / decode of the data with the matrix key K. */

const ClassicalType Hill::process(const ClassicalType &data, const SquareMatrix *K)
{  
   const uint32_t key_dim = K->getDimension();
   const uint32_t data_len = data.length();
   ClassicalType message;
   message.reserve(data_len + key_dim);
   std::vector<uint32_t> pos;
   pos.reserve(key_dim);
   
   for (uint32_t i = 0; i < data_len; i += key_dim)
   {
      for (uint32_t j = 0; j < key_dim; ++j)
      {
         pos.push_back(alpha.find(data[i + j]));
      }
      const std::vector<uint32_t> cipher_pos(K * pos);

      for (const auto x : cipher_pos)
      {
         message.push_back(alpha[x]);
      }
      pos.clear();
   }

   return message;
}

const ClassicalType Hill::encode(const ClassicalType &clear_text)
{
   const ClassicalType full_text(appendChars(clear_text, key->getDimension(), 'X'));
   
   return process(full_text, key);
}

const ClassicalType Hill::decode(const ClassicalType &cipher_text)
{
   return process(cipher_text, key->inverse());
}