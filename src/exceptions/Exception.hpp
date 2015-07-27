/*
 * Interface for exceptions handler.
 */
#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "../String.hpp"

namespace CryptoGL
{
    class Exception
    {
    public:
       using ErrorType = String;

       Exception() {}
       explicit Exception(const ErrorType &message) : message(message) {}
       virtual ~Exception() {};

       const ErrorType what() const noexcept { return message; };

    protected:
       ErrorType message = "";
    };
}

#endif