
#ifndef TEST_HPP
#define	TEST_HPP

#include "../src/Types.hpp"

#include "TestContainer.hpp"

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
//            if(is_success)
//            {
//                std::cout << "[PASSED] " << name;
//            }
//            else
//            {
//                std::cout << "[FAILED] " << name;
//            }
            std::cout << (is_success) ? "[PASSED] " : "[FAILED] ";
            std::cout << name << " ";
            
        }
        
    protected:
        explicit Test(const String &name) : name(name) 
        { 
            TestContainer::getInstance().append(this); 
        }
        
        virtual ~Test() {}
        
        template <class Type>
        void compare(const Type &expected_value, const Type &input_value)
        {
            if(expected_value == input_value)
            {
                total_passed_tests++;
                is_success = true;
            }
            total_tests++;
        }
        
    private:
        bool is_success = false;
        static uint64_t total_tests = 0;
        static uint64_t total_passed_tests = 0;
        const String name;
    };
}

#endif