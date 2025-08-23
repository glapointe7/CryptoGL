#pragma once

#include "../../src/core/types/Types.hpp"
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
        virtual ~Test() = default;
        
        virtual void setUp() = 0;
        virtual void run() = 0;
        virtual void tearDown() = 0;
        
        /* Print the result for one test if passed or failed with the expected result. */
        virtual void printResult() const
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
                
        virtual bool hasPassed() const { return has_passed; }
                
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

        template<typename T>
        void compare(const T& expected_value, const T& input_value)
        {
            static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>,
                        "Generic compare only for arithmetic/enum types");
            
            if (expected_value == input_value) {
                has_passed = true;
            } else {
                // Only convert to string on failure (lazy evaluation)
                this->expected_value = uint64::toString(static_cast<uint64_t>(expected_value));
                this->input_value = uint64::toString(static_cast<uint64_t>(input_value));
            }
        }

        void compare(bool expected_value, bool input_value)
        {
            if (expected_value == input_value) {
                has_passed = true;
            } else {
                this->expected_value = expected_value ? String("true") : String("false");
                this->input_value = input_value ? String("true") : String("false");
            }
        }

        template<typename T>
        void compare(const Vector<T>& expected_value, const Vector<T>& input_value)
        {
            // Fast path: check sizes first
            if (expected_value.size() != input_value.size()) {
                has_passed = false;
                this->expected_value = String("size=") + uint64::toString(expected_value.size());
                this->input_value = String("size=") + uint64::toString(input_value.size());
                return;
            }
            
            // Fast equality check
            if (expected_value == input_value) {
                has_passed = true;
            } else {
                // Only convert to hex on failure
                this->expected_value = expected_value.toHexString();
                this->input_value = input_value.toHexString();
            }
        }

        void compare(size_t expected_value, size_t input_value)
        {
            if (expected_value == input_value) {
                has_passed = true;
            } else {
                this->expected_value = uint64::toString(expected_value);
                this->input_value = uint64::toString(input_value);
            }
        }

        // Support for move semantics to avoid copies
        void compare(String&& expected_value, String&& input_value)
        {
            if (expected_value == input_value) {
                has_passed = true;
            } else {
                this->expected_value = std::move(expected_value);
                this->input_value = std::move(input_value);
            }
        }

        // For cryptographic timing tests - allows small variance
        void compare(double expected_value, double input_value, const double epsilon = 1e-9)
        {
            if (std::abs(expected_value - input_value) <= epsilon) {
                has_passed = true;
            } else {
                this->expected_value = String(std::to_string(expected_value));
                this->input_value = String(std::to_string(input_value));
            }
        }

        /* Cast the input value to an hexa String and compare them. */
        template<typename T>
        void compare(const String &expected_value, const Vector<T> &input_value)
        {
            compare(expected_value, input_value.toHexString());
        }
               
    private:
        String expected_value;
        String input_value;
        bool has_passed = false;
        String name;
    };
}