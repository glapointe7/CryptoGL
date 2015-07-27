
#ifndef TEST_HPP
#define	TEST_HPP

#include "../src/Types.hpp"
#include "TextColors.hpp"
#include "TestContainer.hpp"

#include <iostream>

using namespace CryptoGL;

namespace UnitTests
{    
// Macro to be able to automatically run unit tests easily.
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

    
    // Parent class for all unit tests.
    class Test  
    {
    public:
        virtual ~Test() {}
        
        virtual void setUp() = 0;
        virtual void run() = 0;
        virtual void tearDown() = 0;
        
        /* Print the result for one test if passed or failed with the expected result. */
        void printResult() const
        {
            if(has_passed)
            {
                std::cout << Color::FG_GREEN << "\n[PASSED] ";
                std::cout << Color::FG_DEFAULT << name;
            }
            else
            {
                std::cout << Color::FG_RED << "\n[FAILED] ";
                std::cout << Color::FG_DEFAULT << name << " \n   Input value: ";
                std::cout << Color::FG_RED << input_value;
                std::cout << Color::FG_DEFAULT << "\nExpected value: " << expected_value;
            }
        }
                
        bool hasPassed() const { return has_passed; }
                
    protected:
        void setName(const String &name) { this->name = name; }
        
        /* Compare Strings */
        void compare(const String &expected_value, const String &input_value)
        {
            if(expected_value == input_value)
            {
                has_passed = true;
            }
            else
            {
                this->expected_value = expected_value;
                this->input_value = input_value;
            }
        }
        
        /* Compare integers */
        void compare(const uint64_t &expected_value, const uint64_t &input_value)
        {
            compare(uint64::toString(expected_value), uint64::toString(input_value));
        }
        
        /* Cast the input value to an hexa String and compare them. */
        void compare(const String &expected_value, const BytesVector &input_value)
        {
            compare(expected_value, input_value.toHexString());
        }
        
        /* Cast expected and input values to an hexa String and compare them. */
        void compare(const BytesVector &expected_value, const BytesVector &input_value)
        {
            compare(expected_value.toHexString(), input_value.toHexString());
        }
        
        /* Cast expected and input values to an hexa String and compare them. */
        void compare(const String &expected_value, const UInt32Vector &input_value)
        {
            compare(expected_value, input_value.toHexString());
        }
        
        void compare(const UInt32Vector &expected_value, const UInt32Vector &input_value)
        {            
            compare(expected_value.toHexString(), input_value.toHexString());
        }
        
        void compare(const Int64Matrix &expected_value, const Int64Matrix &input_value)
        {    
            const uint64_t expected_value_size = expected_value.size();
            if(expected_value_size == input_value.size())
            {
                String input = "", output = "";
                for(uint64_t i = 0; i < expected_value_size; ++i)
                {
                    input.push_back('{');
                    output.push_back('{');
                    
                    input.append(input_value[i].join(','));
                    output.append(expected_value[i].join(','));
                    
                    input.append("} ");
                    output.append("} ");
                }
                
                compare(output, input);
            }
        }
               
    private:
        String expected_value;
        String input_value;
        bool has_passed = false;
        String name;
    };
}

#endif