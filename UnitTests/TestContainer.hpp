
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
        
        uint64_t total_tests = 0;
        uint64_t total_passed_tests = 0;
        
    public:
        static TestContainer &getInstance() 
        { 
            static TestContainer instance;
            return instance; 
        }
        
        static void append(Test *CurrentTest)
        {
            unit_tests_vector.push_back(CurrentTest);
        }
        
        /* Run all tests */
        static void runAllTests()
        {
            for(const auto &current_test : unit_tests_vector)
            {
                current_test->setUp();
                current_test->run();
                current_test->tearDown();

                current_test->printResult();
            }
        }
    };
    
    Vector<Test *> TestContainer::unit_tests_vector;
}

#endif