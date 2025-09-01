#pragma once

#include "Test.hpp"
#include <chrono>
#include "TestReporter.hpp"

namespace UnitTests
{
    using namespace std::chrono;
    using Time = high_resolution_clock;
    
    class TestContainer
    {
    private:        
        static Vector<Test *> unit_tests;
        
        TestContainer() = default;

    public:        
        static TestContainer &getInstance() 
        { 
            static TestContainer instance;
            return instance; 
        }    

        void append(Test *const current_test)
        {
            unit_tests.push_back(current_test);
        }
        
        static void runAllTests()
        {
            TestReporter reporter("CryptoGL Unit Tests");
            
            // Execute all tests and collect results
            for (Test* test : unit_tests)
            {
                const TestResult result = test->executeWithReporting();
                reporter.addTestResult(result);
            }
            
            // Generate multiple report formats
            reporter.generateReport(TestReporter::OutputFormat::CONSOLE);
            reporter.generateReport(TestReporter::OutputFormat::HTML, "test_report.html");
            //reporter.generateReport(TestReporter::OutputFormat::XML_JUNIT, "junit_results.xml");
            reporter.getStatistics();
        }
    };
    Vector<Test *> TestContainer::unit_tests;
}