
#ifndef TEST_HPP
#define	TEST_HPP

#include <sys/time.h>

#include "../src/Types.hpp"
#include "TextColors.hpp"
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
        
        /* Print the result for one test if passed or failed, name and execution time. */
        void printResult()
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
                std::cout << input_value << "\nExpected value: " << expected_value;
            }

            std::cout << "\nTime elapsed: " << end_time - start_time << " ms\n";
        }
        
        /* Start the timer for the current test. */
        void startTimer()
        {
            gettimeofday(&time_obj, NULL);
            start_time = time_obj.tv_sec + (time_obj.tv_usec / 1000.0);
        }
        
        /* Stop the timer for the current test. */
        void stopTimer()
        {
            gettimeofday(&time_obj, NULL);
            end_time = time_obj.tv_sec + (time_obj.tv_usec / 1000.0);
        }
        
        void setName(const String &name) { this->name = name; }
        bool hasPassed() { return has_passed; }
        double getExecutedTime() { return end_time - start_time; }
        
    protected:
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
            if(expected_value == input_value)
            {
                has_passed = true;
            }
            else
            {
                this->expected_value = uint64::toString(expected_value);
                this->input_value = uint64::toString(input_value);
            }
        }
        
        
    private:
        String expected_value;
        String input_value;
        struct timeval time_obj;
        bool has_passed = false;
        double start_time;
        double end_time;
        String name;
    };
}

#endif