/*
 * Interface for exceptions handler.
 */
#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <string>

class Exception
{
public:
   Exception() {}
   Exception(const std::string &text) : message(text) {}
   virtual ~Exception() {};
   
   const std::string what() const noexcept { return message; };
   
protected:
   std::string message = "";
};

#endif