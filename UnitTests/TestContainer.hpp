
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
        
    public:
        static TestContainer &getInstance() 
        { 
            static TestContainer instance;
            return instance; 
        }
        
        void append(const Test *CurrentTest)
        {
            unit_tests_vector.push_back(CurrentTest);
        }
        
        /* Run all tests */
        static void runAllTests()
        {
            if(!unit_tests_vector.empty())
            {
                for(const auto current_test : unit_tests_vector)
                {
                    current_test->setUp();
                    current_test->run();
                    current_test->tearDown();
                    
                    current_test->printResult();
                }
            }
        }
    };
}

#endif