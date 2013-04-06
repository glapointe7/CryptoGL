//#include "Delastelle.hpp"
#include "Matrix.hpp"
//#include "Railfence.h"
//#include "Playfair.h"
//#include "Polybe.h"
//#include "Wolseley.h"
//#include "Cesar.h"
//#include "StringCipher.h"
//#include "Tools.h"
//#include "converterTools.h"
//#include "Morse.h"
#include <iostream>

int main()
{
   // below some demonstration of the usage of the Matrix class
   // create an empty matrix of 3x3 (will initially contain zeros)
   int cols = 3;
   int rows = 3;
   Matrix A = Matrix(cols, rows);

   // fill in some values in matrix a
   int count = 0;
   for (int r = 1; r <= rows; r++)
   {
      for (int c = 1; c <= cols; c++)
      {
         count++;
         A(r, c) = count;
      }
   }

   // adjust a value in the matrix (indexes are one-based)
   A(2, 1) = 1.23;

   // read a value from the matrix (indexes are one-based)
   double centervalue = A(2, 2);
   printf("centervalue = %f \n", centervalue);
   printf("\n");

   // print the whole matrix
   printf("A = \n");
   A.print();
   printf("\n");

   Matrix B = Matrix::ones(rows, cols) + Matrix::diag(rows);
   printf("B = \n");
   B.print();
   printf("\n");

   Matrix A_inv = Matrix::inverse(A);
   printf("Inverse(A) = \n");
   A_inv.print();
   printf("\n");


   /*Delastelle *D = new Delastelle();
   D->setBlockLength(5);
   D->setKey("DIMANCHE");
   D->setGridDimension(5);
   
   //string text = "SALUTNILILPARAITQUETUCONNAISLAPROGRAMMATIONORIENTEEOBJETSURLEBOUTDESDOIGTS";
   //string text = "POSSNDGOBBPPDRQSVTCOVGDNZDRABSSKPOMDDSKTMMQZXIAHOXEEEYESYOPLHORTNIPGLNMSVNR";
   string text = D->load(R"(../GIT_CryptoCode/cipher_text.txt)");
   cout << D->decode(text);*/

   //Playfair *P = new Playfair();
   //P->setKey("MAISON");
   //cout << P->encode();

   //string key = "ABCDEEFDABE";
   //string text = "iI PArait Que TU vEUX que Je tEstE kpTa1 aVEc mA pRoPre cLE";
   //string symbol = "01";
   //replaceChar(text, 'a', 'A');
   //convertMajMinToSymbol(text, symbol);
   //removeRepeatedLetters(key);
   //cout << key;
   //cout << convertTextToBinaryString(key);
   /*vector<uint_fast32_t> seq = {3, 5, 15, 25, 54, 110, 225};
   const uint_fast32_t nb = 10;
   const uint_fast32_t modulo = 439;
   Hellman H("plain.txt", "crypted.txt");
   H.encode(modulo, nb, seq);*/
}
