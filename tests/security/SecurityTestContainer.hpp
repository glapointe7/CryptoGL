 /**
     * @brief Security test container for managing and executing security tests
     * 
     * The SecurityTestContainer class extends the basic TestContainer functionality
     * to provide specialized management for security-focused tests. It handles
     * the collection, execution, and reporting of cryptographic security tests
     * with enhanced security-specific analytics and vulnerability detection.
     * 
     * Key Features:
     * - Manages security test lifecycle and execution
     * - Provides security-specific reporting and analytics
     * - Handles vulnerability detection and classification
     * - Supports statistical analysis of security test results
     * - Enables batch security testing with comprehensive reporting
     * 
     * Design Note:
     * This class inherits from TestContainer to reuse base functionality while
     * adding security-specific capabilities. Since SecurityTest inherits from Test,
     * we can leverage the base container's infrastructure while providing enhanced
     * security analysis and vulnerability detection capabilities.
     */
     class SecurityTestContainer : public TestContainer
     {
     private:
         SecurityTestContainer() : TestContainer() {}
         
         static Vector<SecurityTest*> security_tests_vector;
         static uint64_t total_secure_tests;
         static uint64_t total_vulnerable_tests;
         static double total_security_execution_time;
         static std::map<SecurityAnalysis, uint64_t> vulnerability_counts;
         
     public:
         static SecurityTestContainer &getInstance() 
         { 
             static SecurityTestContainer instance;
             return instance; 
         }
         
         /**
          * @brief Add a security test to the container
          * @param security_test Pointer to the security test to add
          * 
          * This method adds the test to both the base TestContainer (for compatibility)
          * and our security-specific vector (for enhanced security analysis).
          */
         void append(SecurityTest *const security_test)
         {
             // Add to base class container for basic test functionality
             TestContainer::append(static_cast(security_test));
             
             // Also add to our security-specific container for enhanced analysis
             security_tests_vector.push_back(security_test);
         }
         
         /**
          * @brief Execute all security tests in the container
          * 
          * Runs all registered security tests with comprehensive timing analysis,
          * vulnerability detection, and statistical reporting. Each test is
          * executed in isolation with proper setup and teardown procedures.
          * 
          * This method extends the base TestContainer::runAllTests() functionality
          * with security-specific analysis and reporting.
          */
         static void runAllSecurityTests()
         {
             std::cout << "\n🛡️ Starting Comprehensive Security Test Suite\n";
             std::cout << std::string(80, '=') << "\n";
             
             for(const auto ¤t_test : security_tests_vector)
             {
                 const auto start_time = Time::now();
                 
                 try {
                     current_test->setUp();
                     current_test->run();
                     current_test->tearDown();
                     
                     // Analyze security test results
                     analyzeSecurityTestResults(current_test);
                     
                 } catch (const std::exception& e) {
                     current_test->recordSecurityException("Test Execution", e.what());
                 }
                 
                 const auto end_time = Time::now();
                 
                 const double elapsed_time = duration<double, std::milli>(end_time - start_time).count();
                 total_security_execution_time += elapsed_time;
                 
                 if (current_test->hasPassed()) {
                     total_secure_tests++;
                 } else {
                     total_vulnerable_tests++;
                 }
                 
                 current_test->printResult();
                 std::cout << "\n⏱️ Security Test Time: " << elapsed_time << " ms\n";
                 std::cout << std::string(80, '-') << "\n";
             }
             
             printSecurityReport();
         }
         
         /**
          * @brief Run both basic and security tests with unified reporting
          * 
          * This method demonstrates how to leverage inheritance by running
          * both the base TestContainer tests and SecurityTestContainer tests,
          * then providing a unified security-enhanced report.
          */
         static void runAllTestsWithSecurityAnalysis()
         {
             std::cout << "\n🧪 Running Complete Test Suite (Basic + Security)\n";
             std::cout << std::string(80, '=') << "\n";
             
             // First run basic unit tests using base class functionality
             std::cout << "\n📋 Basic Unit Tests:\n";
             std::cout << std::string(40, '-') << "\n";
             TestContainer::runAllTests();
             
             // Then run security tests with enhanced analysis
             std::cout << "\n🛡️ Security Tests:\n";
             std::cout << std::string(40, '-') << "\n";
             runAllSecurityTests();
             
             std::cout << "\n✅ Complete Test Suite Finished\n";
         }
         
     private:
         /**
          * @brief Analyze individual security test results for patterns
          * @param security_test The security test to analyze
          */
         static void analyzeSecurityTestResults(SecurityTest* security_test)
         {
             const auto& results = security_test->getSecurityResults();
             
             for (const auto& result : results) {
                 if (result.security_verdict == SecurityVerdict::VULNERABLE) {
                     // Count vulnerabilities by analysis type
                     for (const auto& vuln : result.vulnerabilities) {
                         if (vuln.find("timing") != std::string::npos) {
                             vulnerability_counts[SecurityAnalysis::TIMING_ATTACK]++;
                         } else if (vuln.find("cache") != std::string::npos) {
                             vulnerability_counts[SecurityAnalysis::CACHE_TIMING]++;
                         } else if (vuln.find("branch") != std::string::npos) {
                             vulnerability_counts[SecurityAnalysis::BRANCH_PREDICTION]++;
                         }
                         // Add other vulnerability type classifications as needed
                     }
                 }
             }
         }
         
         /**
          * @brief Print comprehensive security analysis report
          */
         static void printSecurityReport()
         {
             const uint64_t total_tests = security_tests_vector.size();
             
             std::cout << "\n🛡️ COMPREHENSIVE SECURITY ANALYSIS REPORT\n";
             std::cout << std::string(80, '=') << "\n";
             
             std::cout << "📊 Test Execution Summary:\n";
             std::cout << "  Total Security Tests: " << total_tests << "\n";
             std::cout << "  ✅ Secure Tests: " << total_secure_tests << "\n";
             std::cout << "  ❌ Vulnerable Tests: " << total_vulnerable_tests << "\n";
             std::cout << "  ⏱️ Total Execution Time: " << total_security_execution_time << " ms\n\n";
             
             // Calculate security metrics
             double security_pass_rate = total_tests > 0 ? 
                 (static_cast<double>(total_secure_tests) / total_tests) * 100.0 : 0.0;
             
             std::cout << "🔒 Security Metrics:\n";
             std::cout << "  Security Pass Rate: " << std::fixed << std::setprecision(2) 
                      << security_pass_rate << "%\n";
             
             if (!vulnerability_counts.empty()) {
                 std::cout << "\n⚠️ Vulnerability Analysis:\n";
                 for (const auto& [analysis_type, count] : vulnerability_counts) {
                     std::cout << "  " << getAnalysisTypeName(analysis_type) << ": " << count << "\n";
                 }
             }
             
             // Overall security verdict
             if (total_vulnerable_tests == 0) {
                 std::cout << "\n🏆 OVERALL SECURITY STATUS: ✅ SECURE\n";
                 std::cout << "All security tests passed. No vulnerabilities detected.\n";
             } else {
                 std::cout << "\n🚨 OVERALL SECURITY STATUS: ❌ VULNERABILITIES DETECTED\n";
                 std::cout << "Security issues found. Review test results and implement fixes.\n";
             }
             
             std::cout << std::string(80, '=') << "\n";
         }
         
         /**
          * @brief Get human-readable name for security analysis type
          * @param type The security analysis type
          * @return String representation of the analysis type
          */
         static std::string getAnalysisTypeName(SecurityAnalysis type)
         {
             switch (type) {
                 case SecurityAnalysis::TIMING_ATTACK: return "Timing Attacks";
                 case SecurityAnalysis::CACHE_TIMING: return "Cache Timing Attacks";
                 case SecurityAnalysis::BRANCH_PREDICTION: return "Branch Prediction Attacks";
                 case SecurityAnalysis::INPUT_VALIDATION: return "Input Validation Issues";
                 case SecurityAnalysis::MEMORY_SAFETY: return "Memory Safety Issues";
                 case SecurityAnalysis::FAULT_INJECTION: return "Fault Injection Vulnerabilities";
                 case SecurityAnalysis::CRYPTOGRAPHIC_PROPERTIES: return "Cryptographic Property Violations";
                 default: return "Unknown Analysis Type";
             }
         }
     };
     
     // Static member definitions
     Vector<SecurityTest*> SecurityTestContainer::security_tests_vector;
     uint64_t SecurityTestContainer::total_secure_tests = 0;
     uint64_t SecurityTestContainer::total_vulnerable_tests = 0;
     double SecurityTestContainer::total_security_execution_time = 0.0;
     std::map<SecurityAnalysis, uint64_t> SecurityTestContainer::vulnerability_counts;