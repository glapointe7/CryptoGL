#pragma once

#include "../../src/core/types/Types.hpp"
#include "../../src/core/types/BigInteger.hpp"
#include "TextColors.hpp"
#include "TestContainer.hpp"
#include "TestReporter.hpp"

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

/**
 * @brief Enhanced TEST macro with category support
 */
#define TEST_WITH_CATEGORY(ClassName, Parent, Category) \
    class ClassName : public Parent \
    { \
    public: \
        ClassName() { setName(#ClassName); setCategory(#Category); } \
        void run() override; \
    }; \
    static ClassName instance_##ClassName; \
    void ClassName::run()

    
    // Parent class for all unit tests.
    class Test  
    {
    public:
        virtual ~Test() = default;
        
        virtual void setUp() = 0;
        virtual void run() = 0;
        virtual void tearDown() = 0;

        /**
         * @brief Execute test with timing and result collection
         */
        TestResult executeWithReporting()
        {
            TestResult result;
            result.test_name = getName();
            result.test_category = extractCategory(result.test_name);
            result.start_time = std::chrono::high_resolution_clock::now();
            
            try
            {
                setUp();
                run();
                tearDown();
                
                result.passed = hasPassed();
                result.assertions_count = assertion_count;
                
                if (!result.passed)
                {
                    result.expected_value = expected_value;
                    result.actual_value = input_value;
                }
            }
            catch (const std::exception& e)
            {
                result.passed = false;
                result.failure_message = String("Exception: ") + e.what();
            }
            
            result.end_time = std::chrono::high_resolution_clock::now();
            const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(result.end_time - result.start_time);
            result.execution_time_ms = duration.count() / 1000.0;
            
            return result;
        }
                
        virtual bool hasPassed() const { return has_passed; }
                
    protected:
        /**
         * @brief Get test name (make name accessible)
         */
        const String& getName() const { return name; }
        
        /**
         * @brief Get expected value for reporting
         */
        const String& getExpectedValue() const { return expected_value; }
        
        /**
         * @brief Get actual value for reporting
         */
        const String& getActualValue() const { return input_value; }
        
        /**
         * @brief Set custom failure message
         */
        void setFailureMessage(const String& message) { failure_message = message; }
        
        /**
         * @brief Get failure message
         */
        const String& getFailureMessage() const { return failure_message; }

        void setName(const String &name) { this->name = name; }
        
        /* Compare Strings */
        void compare(const String &expected_value, const String &input_value)
        {
            ++assertion_count;
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
            
            ++assertion_count;
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
            ++assertion_count;
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
            ++assertion_count;
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
            ++assertion_count;
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
            ++assertion_count;
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
            ++assertion_count;
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
            ++assertion_count;
            compare(expected_value, input_value.toHexString());
        }

        // For BigInteger comparisons
        void compare(const BigInteger& expected, const BigInteger& actual) {
            ++assertion_count;
            if (expected == actual) {
                has_passed = true;
            } else {
                // Convert to strings ONLY on failure for debugging
                this->expected_value = expected.toHexString();
                this->input_value = actual.toHexString();
                has_passed = false;
            }
        }
               
    private:
        String expected_value;
        String input_value;
        bool has_passed = false;
        String name;
        String failure_message;
        size_t assertion_count = 0;

        /**
         * @brief Extract category from test name (e.g., "AESEncryptionTest" -> "AES")
         */
        String extractCategory(const String& test_name)
        {
            // Simple heuristic: find first uppercase letter after lowercase
            for (size_t i = 1; i < test_name.length(); ++i)
            {
                if (std::isupper(test_name[i]) && std::islower(test_name[i-1]))
                {
                    return test_name.substr(0, i);
                }
            }
            return "General";
        }
    };
}