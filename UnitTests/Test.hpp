
#ifndef TEST_HPP
#define	TEST_HPP

#include "../src/Types.hpp"
#include "TestContainer.hpp"

#include <iostream>

using namespace CryptoGL;

namespace UnitTests
{    
    
#define TEST(ClassName, Parent) \
    class ClassName : public Parent \
    { \
    public: \
        ClassName() { setName(#ClassName); } \
        void run() override; \
    }; \
    class Add##ClassName \
    { \
    public: \
        Add##ClassName() { TestContainer::getInstance().append(new ClassName()); } \
    }; \
    static Add##ClassName AddThis##ClassName; \
    void ClassName::run()

    
    class Test  
    {
    public:
        virtual ~Test() {}
        
        virtual void setUp() = 0;
        virtual void run() = 0;
        virtual void tearDown() = 0;
        
        void printResult()
        {
            std::cout << ((has_passed) ? "[PASSED] " : "[FAILED] ");
            std::cout << name << " \n";
        }
        
        void setName(const String &name) { this->name = name; }
        bool hasPassed() { return has_passed; }
        
    protected:
        template <class Type>
        void compare(const Type &expected_value, const Type &input_value)
        {
            if(expected_value == input_value)
            {
                has_passed = true;
            }
        }
        
    private:
        bool has_passed = false;
        double execution_time;
        String name;
    };
}

#endif