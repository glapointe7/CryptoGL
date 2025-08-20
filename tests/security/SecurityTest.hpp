#pragma once

#include "Test.hpp"
#include <chrono>
#include <functional>
#include <string>
#include <vector>
#include <map>

namespace SecurityTests
{
    enum class SecurityAnalysis
    {
        TIMING_ATTACK,
        CACHE_TIMING,
        BRANCH_PREDICTION,
        INPUT_VALIDATION,
        MEMORY_SAFETY,
        FAULT_INJECTION,
        CRYPTOGRAPHIC_PROPERTIES
    };
    
    enum class SecurityVerdict
    {
        SECURE,
        VULNERABLE,
        INCONCLUSIVE,
        ERROR
    };
    
    struct SecurityResult
    {
        std::string operation;
        uint64_t input_value;
        uint64_t output_value;
        double timing;
        bool success;
        bool exception_thrown;
    };
    
    struct SecurityAnalysisResult
    {
        std::string test_name;
        std::string analysis_type;
        SecurityVerdict security_verdict;
        double statistical_significance;
        double test_statistic;
        double correlation_coefficient;
        std::vector<std::string> vulnerabilities;
        std::map<std::string, bool> test_results;
        std::string detailed_report;
    };
    
    /**
     * @brief Base class for security testing with cryptographic focus
     * 
     * Provides specialized testing capabilities for detecting security vulnerabilities
     * in cryptographic implementations, including timing attacks, side-channel attacks,
     * and other implementation-specific security issues.
     */
    class SecurityTest : public UnitTests::Test
    {
    protected:
        SecurityAnalysis analysis_type;
        size_t sample_size = 10000;
        double confidence_level = 0.95;
        std::vector<SecurityAnalysisResult> security_results;
        
    public:
        /**
         * @brief Set the type of security analysis to perform
         * @param type Type of security analysis
         */
        void setAnalysisType(SecurityAnalysis type) { analysis_type = type; }
        
        /**
         * @brief Set the sample size for statistical analysis
         * @param size Number of samples to collect
         */
        void setSampleSize(size_t size) { sample_size = size; }
        
        /**
         * @brief Set the confidence level for statistical tests
         * @param level Confidence level (0.0 to 1.0)
         */
        void setConfidenceLevel(double level) { confidence_level = level; }
        
        /**
         * @brief Record a security analysis result
         * @param result Security analysis result to record
         */
        void recordSecurityResult(const SecurityAnalysisResult& result)
        {
            security_results.push_back(result);
        }
        
        /**
         * @brief Record a security exception for analysis
         * @param operation Operation that caused the exception
         * @param message Exception message
         */
        void recordSecurityException(const std::string& operation, const std::string& message)
        {
            SecurityAnalysisResult result;
            result.test_name = operation + " Exception";
            result.analysis_type = "Exception Analysis";
            result.security_verdict = SecurityVerdict::ERROR;
            result.detailed_report = "Exception occurred: " + message;
            security_results.push_back(result);
        }
        
        // Override base Test methods for security-specific behavior
        void run() override = 0;
        
        void printResult() const override
        {
            printSecurityReport();
        }
        
        bool hasPassed() const override
        {
            // Security tests "pass" if no vulnerabilities are detected
            for (const auto& result : security_results) {
                if (result.security_verdict == SecurityVerdict::VULNERABLE) {
                    return false;
                }
            }
            return true;
        }
        
        /**
         * @brief Get all security analysis results
         * @return Vector of security analysis results
         */
        const std::vector<SecurityAnalysisResult>& getSecurityResults() const
        {
            return security_results;
        }
        
    protected:
        /**
         * @brief Print comprehensive security report
         */
        void printSecurityReport() const
        {
            std::cout << "\n" << std::string(80, '=') << "\n";
            std::cout << "🛡️ SECURITY ANALYSIS REPORT\n";
            std::cout << "Test: " << getName() << "\n";
            std::cout << std::string(80, '=') << "\n";
            
            size_t secure_count = 0;
            size_t vulnerable_count = 0;
            size_t inconclusive_count = 0;
            size_t error_count = 0;
            
            for (const auto& result : security_results) {
                printSecurityResultSummary(result);
                
                switch (result.security_verdict) {
                    case SecurityVerdict::SECURE:
                        secure_count++;
                        break;
                    case SecurityVerdict::VULNERABLE:
                        vulnerable_count++;
                        break;
                    case SecurityVerdict::INCONCLUSIVE:
                        inconclusive_count++;
                        break;
                    case SecurityVerdict::ERROR:
                        error_count++;
                        break;
                }
            }
            
            std::cout << "\n📊 SECURITY SUMMARY:\n";
            std::cout << "  ✅ Secure: " << secure_count << "\n";
            std::cout << "  ❌ Vulnerable: " << vulnerable_count << "\n";
            std::cout << "  ❓ Inconclusive: " << inconclusive_count << "\n";
            std::cout << "  ⚠️  Errors: " << error_count << "\n";
            
            SecurityVerdict overall_verdict = determineOverallVerdict(secure_count, vulnerable_count, error_count);
            printOverallVerdict(overall_verdict);
        }
        
    private:
        void printSecurityResultSummary(const SecurityAnalysisResult& result) const
        {
            std::cout << "\n🔍 " << result.test_name << " (" << result.analysis_type << ")\n";
            
            std::string verdict_symbol;
            std::string verdict_color;
            switch (result.security_verdict) {
                case SecurityVerdict::SECURE:
                    verdict_symbol = "✅";
                    verdict_color = "SECURE";
                    break;
                case SecurityVerdict::VULNERABLE:
                    verdict_symbol = "❌";
                    verdict_color = "VULNERABLE";
                    break;
                case SecurityVerdict::INCONCLUSIVE:
                    verdict_symbol = "❓";
                    verdict_color = "INCONCLUSIVE";
                    break;
                case SecurityVerdict::ERROR:
                    verdict_symbol = "⚠️";
                    verdict_color = "ERROR";
                    break;
            }
            
            std::cout << "  Verdict: " << verdict_symbol << " " << verdict_color << "\n";
            
            if (result.statistical_significance != 0.0) {
                std::cout << "  P-value: " << std::fixed << std::setprecision(6) 
                          << result.statistical_significance << "\n";
            }
            
            if (result.correlation_coefficient != 0.0) {
                std::cout << "  Correlation: " << std::fixed << std::setprecision(4)
                          << result.correlation_coefficient << "\n";
            }
            
            if (!result.vulnerabilities.empty()) {
                std::cout << "  Vulnerabilities:\n";
                for (const auto& vuln : result.vulnerabilities) {
                    std::cout << "    • " << vuln << "\n";
                }
            }
        }
        
        SecurityVerdict determineOverallVerdict(size_t secure, size_t vulnerable, size_t errors) const
        {
            if (vulnerable > 0) return SecurityVerdict::VULNERABLE;
            if (errors > 0) return SecurityVerdict::ERROR;
            if (secure > 0) return SecurityVerdict::SECURE;
            return SecurityVerdict::INCONCLUSIVE;
        }
        
        void printOverallVerdict(SecurityVerdict verdict) const
        {
            std::cout << "\n🏆 OVERALL SECURITY VERDICT: ";
            switch (verdict) {
                case SecurityVerdict::SECURE:
                    std::cout << "✅ SECURE - No vulnerabilities detected\n";
                    break;
                case SecurityVerdict::VULNERABLE:
                    std::cout << "❌ VULNERABLE - Security issues found\n";
                    break;
                case SecurityVerdict::INCONCLUSIVE:
                    std::cout << "❓ INCONCLUSIVE - Unable to determine security status\n";
                    break;
                case SecurityVerdict::ERROR:
                    std::cout << "⚠️ ERROR - Testing errors occurred\n";
                    break;
            }
            std::cout << std::string(80, '=') << "\n";
        }
    };
    
    // Macro for security test registration
    #define SECURITY_TEST(ClassName, Parent) \
        class ClassName : public Parent \
        { \
        public: \
            ClassName() { setName(#ClassName); } \
            void run() override; \
        }; \
        class AddSecurityTest##ClassName \
        { \
        public: \
            AddSecurityTest##ClassName() { SecurityTestContainer::getInstance().append(new ClassName()); } \
        }; \
        static AddSecurityTest##ClassName AddThisSecurityTest##ClassName; \
        void ClassName::run()
}