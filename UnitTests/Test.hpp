
#ifndef TEST_HPP
#define	TEST_HPP

#include "../src/Types.hpp"

#include <iostream>

using namespace CryptoGL;

namespace UnitTests
{       
    class Test  
    {
    public:
        virtual void setUp() = 0;
        virtual void run() = 0;
        virtual void tearDown() = 0;
        
        void printResult()
        {
            std::cout << ((is_success) ? "[PASSED] " : "[FAILED] ");
            std::cout << name << " ";
        }
        
    protected:
        virtual ~Test() {}
        
        template <class Type>
        void compare(const Type &expected_value, const Type &input_value)
        {
            if(expected_value == input_value)
            {
                is_success = true;
            }
        }
        
        void setName(const String &name) { this->name = name; }
        
    private:
        bool is_success = false;
        double execution_time;
        String name;
    };
}

#endif