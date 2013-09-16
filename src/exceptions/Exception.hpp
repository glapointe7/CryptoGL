/*
 * Interface for exceptions handler.
 */
#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <string>

class Exception
{
public:
   using ErrorType = std::string;
   
   Exception() {}
   explicit Exception(const ErrorType &message) : message(message) {}
   virtual ~Exception() {};
   
   const ErrorType what() const noexcept { return message; };
   
protected:
   ErrorType message = "";
};

#endif