
#ifndef TESTCONTAINER_HPP
#define	TESTCONTAINER_HPP

#include "Test.hpp"

namespace UnitTests
{
    class TestContainer
    {
    private:
        TestContainer() {}
        
        static Vector<Test *> unit_tests_vector;
        
        static uint64_t total_passed_tests;
        
    public:
        static TestContainer &getInstance() 
        { 
            static TestContainer instance;
            return instance; 
        }
        
        void append(Test *const CurrentTest)
        {
            unit_tests_vector.push_back(CurrentTest);
        }
        
        /* Run all tests */
        static int runAllTests()
        {
            for(const auto current_test : unit_tests_vector)
            {
                current_test->setUp();
                current_test->run();
                current_test->tearDown();
                total_passed_tests += current_test->hasPassed();
                current_test->printResult();
            }
            
            printReport();
            
            return 0;
        }
        
        static void printReport()
        {
            const uint64_t total_tests = unit_tests_vector.size();
            std::cout << "\n Tests passed: " << total_passed_tests;
            std::cout << "\n Tests failed: " << (total_tests - total_passed_tests);
            std::cout << "\n Total Tests executed: " << total_tests;
        }
    };
    
    Vector<Test *> TestContainer::unit_tests_vector;
    uint64_t TestContainer::total_passed_tests = 0;
}

#endif