/*
 * Interface for exceptions handler.
 */
#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "../types/String.hpp"

class Exception
{
public:
   using ErrorType = CryptoGL::String;
   
   Exception() {}
   explicit Exception(const ErrorType &message) : message(message) {}
   virtual ~Exception() {};
   
   const ErrorType what() const noexcept { return message; };
   
protected:
   ErrorType message = "";
};

#endif