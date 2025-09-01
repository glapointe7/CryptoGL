#pragma once

#include "Test.hpp"
#include "../../src/core/types/String.hpp"
#include "../../src/core/types/Vector.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

using namespace CryptoGL;

namespace UnitTests
{
    /**
     * @brief Comprehensive test result data structure
     * 
     * Stores all information about a single test execution including
     * timing, memory usage, and detailed failure information.
     */
    struct TestResult
    {
        String test_name;                    // Name of the test
        String test_category;               // Category/module (e.g., "BigInteger", "AES")
        bool passed;                         // Test result
        double execution_time_ms;           // Execution time in milliseconds
        String expected_value;              // Expected result (for failures)
        String actual_value;                // Actual result (for failures)
        String failure_message;             // Custom failure message
        size_t assertions_count;            // Number of assertions made
        
        // Timing information
        std::chrono::high_resolution_clock::time_point start_time;
        std::chrono::high_resolution_clock::time_point end_time;
        
        /**
         * @brief Default constructor
         */
        TestResult() : passed(false), execution_time_ms(0.0), assertions_count(0) {}
    };

    /**
     * @brief Test suite statistics and summary
     */
    struct TestSuiteStatistics
    {
        size_t total_tests = 0;
        size_t passed_tests = 0;
        size_t failed_tests = 0;
        double total_execution_time_ms = 0.0;
        double average_execution_time_ms = 0.0;
        double pass_rate_percent = 0.0;
        double test_rate = 0.0;
        
        /**
         * @brief Calculate statistics from test results
         */
        void calculate(const Vector<TestResult>& results)
        {
            total_tests = results.size();
            passed_tests = std::count_if(results.begin(), results.end(),
                                            [](const TestResult& r) { return r.passed; });
            failed_tests = total_tests - passed_tests;
            
            total_execution_time_ms = std::accumulate(results.begin(), results.end(), 0.0,
                                                  [](double sum, const TestResult& r) { 
                                                      return sum + r.execution_time_ms; 
                                                  });
            
            average_execution_time_ms = total_tests > 0 ? total_execution_time_ms / total_tests : 0.0;
            pass_rate_percent = total_tests > 0 ? (static_cast<double>(passed_tests) / total_tests) * 100.0 : 0.0;
            test_rate = total_tests > 0 ? total_tests / total_execution_time_ms * 1000.0 : 0.0;
        }
    };

    /**
     * @brief Advanced test reporter with multiple output formats
     * 
     * Provides comprehensive test reporting capabilities including
     * console output, HTML reports, XML (JUnit) format, and JSON export.
     */
    class TestReporter
    {
    public:
        /**
         * @brief Output format options
         */
        enum class OutputFormat 
        {
            CONSOLE,    // Color-coded console output
            HTML,       // Professional HTML report
            XML_JUNIT,  // JUnit XML for CI/CD
            JSON        // JSON format for integration
        };
        
        /**
         * @brief Initialize reporter with suite name
         */
        explicit TestReporter(const String& suite_name = "CryptoGL Test Suite")
            : m_suite_name(suite_name), 
              m_start_time(std::chrono::high_resolution_clock::now()) {}
        
        /**
         * @brief Add test result to the report
         */
        void addTestResult(const TestResult& result)
        {
            m_test_results.push_back(result);
        }
        
        /**
         * @brief Generate and output report in specified format
         */
        void generateReport(OutputFormat format, const String& filename = "")
        {
            // Calculate statistics
            m_statistics.calculate(m_test_results);
            
            switch (format)
            {
                case OutputFormat::CONSOLE:
                    generateConsoleReport();
                    break;
                case OutputFormat::HTML:
                    generateHTMLReport(filename);
                    break;
                case OutputFormat::XML_JUNIT:
                    generateJUnitXMLReport(filename);
                    break;
                case OutputFormat::JSON:
                    generateJSONReport(filename);
                    break;
            }
        }
        
        /**
         * @brief Get test suite statistics
         */
        const TestSuiteStatistics& getStatistics() const { return m_statistics; }
        
    private:
        String m_suite_name;
        Vector<TestResult> m_test_results;
        TestSuiteStatistics m_statistics;
        std::chrono::high_resolution_clock::time_point m_start_time;
        
        /**
         * @brief Generate comprehensive console report with Unicode formatting
         */
        void generateConsoleReport()
        {
            auto end_time = std::chrono::high_resolution_clock::now();
    
            // Header with Unicode box-drawing
            std::cout << Color::FG_GREEN << "╔══════════════════════════════════════════════════════════════════╗\n";
            std::cout << "║                      " << m_suite_name << "                        ║\n";
            std::cout << "╠══════════════════════════════════════════════════════════════════╣" << Color::FG_DEFAULT << "\n\n";
            
            // Test execution section
            std::cout << Color::FG_CYAN << "┌─ Test Execution ─────────────────────────────────────────────────┐" << Color::FG_DEFAULT << "\n\n";
            
            // Print individual test results
            for (const auto& result : m_test_results)
            {
                const String status_symbol = result.passed ? "✓" : "✗";
                const Color color = result.passed ? Color::FG_GREEN : Color::FG_RED;
                String test_name = result.test_name;
                
                // Format timing and category
                std::ostringstream timing_stream;
                timing_stream << "[" << std::fixed << std::setprecision(2) << result.execution_time_ms << "ms]";
                String timing = String(timing_stream.str());
                String category = String("[") + result.test_category + "]";
                
                std::cout << color << status_symbol << " " << Color::FG_WHITE << test_name;
                
                // Pad to align timing info
                size_t padding = 50 - test_name.length();
                for (size_t i = 0; i < padding; ++i) std::cout << " ";
                
                std::cout << Color::FG_LIGHT_GRAY << timing << " " << category << Color::FG_DEFAULT << "\n";
                
                // Show failure details
                if (!result.passed)
                {
                    std::cout << "  " << Color::FG_RED << "Expected: " << Color::FG_DEFAULT << result.expected_value << "\n";
                    std::cout << "  " << Color::FG_RED << "Actual  : " << Color::FG_DEFAULT << result.actual_value << "\n";
                    if (!result.failure_message.empty())
                    {
                        std::cout << "  " << Color::FG_RED << "Message : " << Color::FG_DEFAULT << result.failure_message << "\n";
                    }
                }
            }
            
            std::cout << "\n" << Color::FG_CYAN << "└──────────────────────────────────────────────────────────────────┘" << Color::FG_DEFAULT << "\n\n";
            
            // Category summary
            generateCategorySummary();
            
            // Final summary
            std::cout << Color::FG_GREEN << "╔══════════════════════════════════════════════════════════════════╗\n";
            std::cout << "║                         Summary                                  ║\n";
            std::cout << "╠══════════════════════════════════════════════════════════════════╣" << Color::FG_DEFAULT << "\n";
            
            std::cout << Color::FG_WHITE << "Total Tests    : " << Color::FG_CYAN << m_statistics.total_tests << Color::FG_DEFAULT << "\n";
            std::cout << Color::FG_WHITE << "Passed         : " << Color::FG_GREEN << m_statistics.passed_tests 
                    << " (" << std::fixed << std::setprecision(1) << m_statistics.pass_rate_percent << "%)" << Color::FG_DEFAULT << "\n";
            std::cout << Color::FG_WHITE << "Failed         : " << Color::FG_RED << m_statistics.failed_tests 
                    << " (" << std::fixed << std::setprecision(1) << (100.0 - m_statistics.pass_rate_percent) << "%)" << Color::FG_DEFAULT << "\n";
            std::cout << Color::FG_WHITE << "Total Time     : " << Color::FG_LIGHT_GRAY << std::fixed << std::setprecision(2) 
                    << m_statistics.total_execution_time_ms << "ms" << Color::FG_DEFAULT << "\n";
            std::cout << Color::FG_WHITE << "Average Time     : " << Color::FG_LIGHT_GRAY << std::fixed << std::setprecision(2) 
                    << m_statistics.average_execution_time_ms << "ms" << Color::FG_DEFAULT << "\n";
            std::cout << Color::FG_WHITE << "Test Rate     : " << Color::FG_LIGHT_GRAY << std::fixed << std::setprecision(2) 
                    << m_statistics.test_rate << "tests/sec" << Color::FG_DEFAULT << "\n";
            
            // Final status
            if (m_statistics.failed_tests > 0)
            {
                std::cout << Color::FG_RED << "Test suite failed - " << m_statistics.failed_tests << " test(s) failed" << Color::FG_DEFAULT << "\n";
            }
            else
            {
                std::cout << Color::FG_GREEN << "All tests passed successfully!" << Color::FG_DEFAULT << "\n";
            }
        }

        /**
         * @brief Generate professional HTML report with modern styling
         */
        void generateHTMLReport(const String& filename)
        {
            String output_file = filename.empty() ? "test_report.html" : filename;
            std::ofstream file(output_file.toStdString());
            
            if (!file.is_open())
            {
                std::cerr << "Error: Could not create HTML report file: " << output_file << std::endl;
                return;
            }
            
            // HTML Header and CSS
            file << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
            file << "<meta charset=\"UTF-8\">\n";
            file << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
            file << "<title>" << m_suite_name << " - Test Report</title>\n";
            
            // CSS Styling (abbreviated for space)
            file << "<style>\n"
                << "body { font-family: 'Segoe UI', sans-serif; margin: 0; padding: 20px; background: #f5f5f5; }\n"
                << ".container { max-width: 1200px; margin: 0 auto; background: white; border-radius: 10px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }\n"
                << ".header { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 30px; border-radius: 10px 10px 0 0; }\n"
                << ".stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; padding: 20px; }\n"
                << ".test-item.passed { border-left: 4px solid #28a745; }\n"
                << ".test-item.failed { border-left: 4px solid #dc3545; background-color: #fff5f5; }\n"
                << "</style>\n";
            file << "</head>\n<body>\n";
            
            // Content structure
            file << "<div class=\"container\">\n";
            
            // Header
            file << "<div class=\"header\">\n"
                << "<h1>" << m_suite_name << "</h1>\n"
                << "<p>Generated on " << getCurrentTimestamp() << "</p>\n"
                << "</div>\n";
            
            // Statistics Cards
            file << "<div class=\"stats-grid\">\n";
            file << "<div class=\"stat-card\">\n<h3>" << m_statistics.total_tests << "</h3>\n<p>Total Tests</p>\n</div>\n";
            file << "<div class=\"stat-card\">\n<h3 class=\"pass\">" << m_statistics.passed_tests << "</h3>\n<p>Passed</p>\n</div>\n";
            file << "<div class=\"stat-card\">\n<h3 class=\"fail\">" << m_statistics.failed_tests << "</h3>\n<p>Failed</p>\n</div>\n";
            file << "</div>\n";
            
            // Test Results
            file << "<div class=\"test-list\">\n<h2>Test Results</h2>\n";
            
            for (const auto& result : m_test_results)
            {
                String status_class = result.passed ? "passed" : "failed";
                String status_text = result.passed ? "✓ PASS" : "✗ FAIL";
                
                file << "<div class=\"test-item " << status_class << "\">\n"
                    << "<div><span class=\"test-name\">" << result.test_name << "</span>\n"
                    << "<span class=\"test-category\">" << result.test_category << "</span></div>\n";
                    
                if (!result.passed)
                {
                    file << "<div class=\"failure-details\">\n"
                        << "<strong>Expected:</strong> " << result.expected_value << "<br>\n"
                        << "<strong>Actual:</strong> " << result.actual_value << "<br>\n"
                        << "</div>\n";
                }
                
                file << "</div>\n";
            }
            
            file << "</div>\n</div>\n</body>\n</html>\n";
            file.close();
            
            std::cout << "HTML report generated: " << output_file << std::endl;
        }

        /**
         * @brief Generate JUnit XML format for CI/CD integration
         */
        void generateJUnitXMLReport(const String& filename)
        {
            String output_file = filename.empty() ? "junit_results.xml" : filename;
            std::ofstream file(output_file.toStdString());
            
            if (!file.is_open())
            {
                std::cerr << "Error: Could not create JUnit XML file: " << output_file << std::endl;
                return;
            }
            
            // Group tests by category
            std::map<String, Vector<TestResult>> categories;
            for (const auto& result : m_test_results)
            {
                categories[result.test_category].push_back(result);
            }
            
            // XML Header
            file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            file << "<testsuites name=\"" << m_suite_name << "\" tests=\"" << m_statistics.total_tests 
                << "\" failures=\"" << m_statistics.failed_tests << "\" time=\"" 
                << std::fixed << std::setprecision(3) << (m_statistics.total_execution_time_ms / 1000.0) << "\">\n";
            
            // Generate testsuite for each category
            for (const auto& [category, tests] : categories)
            {
                size_t category_failures = std::count_if(tests.begin(), tests.end(), 
                                                        [](const TestResult& r) { return !r.passed; });
                double category_time = std::accumulate(tests.begin(), tests.end(), 0.0,
                                                    [](double sum, const TestResult& r) { return sum + r.execution_time_ms; });
                
                file << "  <testsuite name=\"" << category << "\" tests=\"" << tests.size() 
                    << "\" failures=\"" << category_failures << "\" time=\"" 
                    << std::fixed << std::setprecision(3) << (category_time / 1000.0) << "\">\n";
                
                // Generate testcase for each test
                for (const auto& result : tests)
                {
                    file << "    <testcase name=\"" << result.test_name << "\" time=\"" 
                        << std::fixed << std::setprecision(3) << (result.execution_time_ms / 1000.0) << "\"";
                    
                    if (result.passed)
                    {
                        file << "/>\n";
                    }
                    else
                    {
                        file << ">\n";
                        file << "      <failure message=\"Test assertion failed\">";
                        file << "<![CDATA[\nExpected: " << result.expected_value 
                            << "\nActual: " << result.actual_value << "\n]]></failure>\n";
                        file << "    </testcase>\n";
                    }
                }
                
                file << "  </testsuite>\n";
            }
            
            file << "</testsuites>\n";
            file.close();
            
            std::cout << "JUnit XML report generated: " << output_file << std::endl;
        }

        /**
         * @brief Generate JSON format for programmatic analysis
         */
        void generateJSONReport(const String& filename)
        {
            String output_file = filename.empty() ? "test_report.json" : filename;
            std::ofstream file(output_file.toStdString());
            
            if (!file.is_open())
            {
                std::cerr << "Error: Could not create JSON report file: " << output_file << std::endl;
                return;
            }
            
            // JSON structure
            file << "{\n";
            file << "  \"suite_name\": \"" << m_suite_name << "\",\n";
            file << "  \"timestamp\": \"" << getCurrentTimestamp() << "\",\n";
            
            // Statistics section
            file << "  \"statistics\": {\n";
            file << "    \"total_tests\": " << m_statistics.total_tests << ",\n";
            file << "    \"passed_tests\": " << m_statistics.passed_tests << ",\n";
            file << "    \"failed_tests\": " << m_statistics.failed_tests << ",\n";
            file << "    \"pass_rate_percent\": " << std::fixed << std::setprecision(2) << m_statistics.pass_rate_percent << ",\n";
            file << "    \"total_execution_time_ms\": " << std::fixed << std::setprecision(2) << m_statistics.total_execution_time_ms << "\n";
            file << "  },\n";
            
            // Test results array
            file << "  \"test_results\": [\n";
            
            for (size_t i = 0; i < m_test_results.size(); ++i)
            {
                const auto& result = m_test_results[i];
                
                file << "    {\n";
                file << "      \"test_name\": \"" << result.test_name << "\",\n";
                file << "      \"test_category\": \"" << result.test_category << "\",\n";
                file << "      \"passed\": " << (result.passed ? "true" : "false") << ",\n";
                file << "      \"execution_time_ms\": " << std::fixed << std::setprecision(2) << result.execution_time_ms << ",\n";
                file << "      \"assertions_count\": " << result.assertions_count;
                
                if (!result.passed)
                {
                    file << ",\n";
                    file << "      \"expected_value\": \"" << escapeJsonString(result.expected_value) << "\",\n";
                    file << "      \"actual_value\": \"" << escapeJsonString(result.actual_value) << "\"";
                }
                
                file << "\n    }";
                
                if (i < m_test_results.size() - 1)
                {
                    file << ",";
                }
                file << "\n";
            }
            
            file << "  ]\n";
            file << "}\n";
            
            file.close();
            std::cout << "JSON report generated: " << output_file << std::endl;
        }

         /**
         * @brief Generate category summary for console output
         */
        void generateCategorySummary()
        {
            std::map<String, std::pair<size_t, size_t>> category_stats; // passed, failed
            std::map<String, double> category_times;
            
            for (const auto& result : m_test_results)
            {
                if (result.passed)
                    category_stats[result.test_category].first++;
                else
                    category_stats[result.test_category].second++;
                
                category_times[result.test_category] += result.execution_time_ms;
            }
            
            std::cout << Color::FG_YELLOW << "┌─ Test Categories ────────────────────────────────────────────────┐" << Color::FG_DEFAULT << "\n";
            
            for (const auto& [category, stats] : category_stats)
            {
                size_t total = stats.first + stats.second;
                double avg_time = total > 0 ? category_times[category] / total : 0.0;
                
                std::cout << Color::FG_WHITE << std::left << std::setw(12) << category << ": ";
                std::cout << Color::FG_GREEN << stats.first << " passed" << Color::FG_DEFAULT << ", ";
                std::cout << Color::FG_RED << stats.second << " failed" << Color::FG_DEFAULT << "  ";
                std::cout << Color::FG_LIGHT_GRAY << "(avg: " << std::fixed << std::setprecision(2) << avg_time << "ms)" << Color::FG_DEFAULT << "\n";
            }
            
            std::cout << Color::FG_YELLOW << "└──────────────────────────────────────────────────────────────────┘" << Color::FG_DEFAULT << "\n\n";
        }
        
        /**
         * @brief Get current timestamp for reports
         */
        String getCurrentTimestamp()
        {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            
            std::ostringstream oss;
            oss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
            return String(oss.str());
        }
        
        /**
         * @brief Escape special characters for JSON
         */
        String escapeJsonString(const String& str)
        {
            String result = str;
            // Basic JSON escaping
            std::replace(result.begin(), result.end(), '"', '\'');
            return result;
        }
    };
}