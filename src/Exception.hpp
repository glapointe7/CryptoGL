/*
 * Interface pour la gestion des exceptions.
 */
#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

class Exception
{
public:
   Exception() {}
   Exception(const std::string &text) : message(text) {}
   virtual ~Exception() = 0;
   
protected:
   std::string message;
};

#endif