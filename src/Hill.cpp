
#include "Hill.hpp"

#include "exceptions/EmptyMatrix.hpp"
#include "exceptions/MatrixNotSquare.hpp"
#include "exceptions/MatrixKeyNotReversible.hpp"
#include "exceptions/EmptyKey.hpp"

#include "MathematicalTools.hpp"

void Hill::setKey(const Matrice &key)
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

   // The key must be reversible.
   if (GCD(this->key->det(), this->key->getModulo()) != 1)
   {
      throw MatrixKeyNotReversible("Your matrix key should be reversible to be able to decode the message.");
   }
}

// Process encode / decode of the data with the matrix key K.

const Hill::ClassicalType Hill::process(const ClassicalType &data, const Matrix *K)
{
   if(key->getMatrix().empty())
   {
      throw EmptyKey("The matrix key is not set.");
   }
   
   const uint32_t key_dim = K->getDimension();
   const uint32_t data_len = data.length();
   ClassicalType message = "";
   message.reserve(data_len + key_dim);

   for (uint32_t i = 0; i < data_len; i += key_dim)
   {
      std::vector<uint32_t> pos;
      pos.reserve(key_dim);
      for (uint32_t j = 0; j < key_dim; ++j)
      {
         pos.push_back(alpha.find(data[i + j]));
      }
      const std::vector<uint32_t> cipher_pos(K * pos);

      for (const auto x : cipher_pos)
      {
         message += alpha[x];
      }
   }

   return message;
}

const Hill::ClassicalType Hill::encode(const ClassicalType &clear_text)
{
   const ClassicalType full_text(appendChars(clear_text, key->getDimension(), 'X'));
   
   return process(full_text, key);
}

const Hill::ClassicalType Hill::decode(const ClassicalType &cipher_text)
{
   return process(cipher_text, key->inverse());
}