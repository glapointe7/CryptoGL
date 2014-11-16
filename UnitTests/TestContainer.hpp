
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
        static double total_executed_time;
        
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
            for(const auto &current_test : unit_tests_vector)
            {
                current_test->startTimer();
                current_test->setUp();
                current_test->run();
                current_test->tearDown();
                current_test->stopTimer();
                
                total_executed_time += current_test->getExecutedTime();
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
            std::cout << "\n Total tests executed: " << total_tests;
            std::cout << "\n Total time elapsed: " << total_executed_time << " ms\n";
        }
    };
    
    Vector<Test *> TestContainer::unit_tests_vector;
    uint64_t TestContainer::total_passed_tests = 0;
    double TestContainer::total_executed_time = 0.0;
}

#endif