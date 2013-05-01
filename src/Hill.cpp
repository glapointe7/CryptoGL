
#include "Hill.hpp"
#include "EmptyMatrix.hpp"
#include "MatrixNotSquare.hpp"
#include "MatrixKeyNotReversible.hpp"
#include "MathematicalTools.hpp" // GCD

#include <iostream>

void Hill::setKey(const Matrice &key)
{
   Matrix M;
   try
   {
      M.setMatrix(key);
      M.setModulo(alpha.length());
   }
   catch (EmptyMatrix & EM)
   {
      std::cout << EM.what();
   }
   catch (MatrixNotSquare &MNS)
   {
      std::cout << MNS.what();
   }

   // La clé doit être inversible.
   if (GCD(Matrix::det(M), M.getModulo()) != 1)
   {
      throw MatrixKeyNotReversible("Your matrix key should be reversible to be able to decode the message.");
   }
   else
   {
      this->key = M;
   }
}

// Exécute l'encodage / décodage de data avec la clé K.

const Hill::ClassicalType Hill::process(const ClassicalType &data, const Matrix &K)
{
   const uint32_t key_dim = key.getDimension();
   const uint32_t data_len = data.length();
   ClassicalType message = "";
   message.reserve(data_len * key_dim);

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
   const ClassicalType full_text(appendChars(clear_text, key.getDimension(), 'X'));
   
   return process(full_text, key);
}

const Hill::ClassicalType Hill::decode(const ClassicalType &cipher_text)
{
   return process(cipher_text, Matrix::inverse(key));
}