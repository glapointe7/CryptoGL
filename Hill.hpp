
#ifndef HILL_HPP
#define	HILL_HPP

#include "StringCipher.h"
//#include "Matrix.hpp"
//#include <vector>

#include <Eigen/Core>


class Hill : public StringCipher
{
public:
   Hill();
   
   std::string encode(const std::string &);
   std::string decode(const std::string &);
   
   void setKey(const Eigen::MatrixXi &);
   
private:
   Eigen::MatrixXi key;
};

#endif	/* HILL_HPP */

