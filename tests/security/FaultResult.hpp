/**
     * @brief Comprehensive fault injection result structure for security analysis
     * 
     * The FaultResult structure captures detailed information about fault injection
     * experiments used to test the resilience of cryptographic operations against
     * hardware-based attacks. Fault injection attacks attempt to corrupt computation
     * by introducing errors (voltage glitches, clock glitches, electromagnetic pulses)
     * and analyzing the resulting differences in behavior or output.
     * 
     * Security Analysis Purpose:
     * Fault injection testing is critical for cryptographic implementations because:
     * - Hardware attacks can bypass software-level security measures
     * - Differential fault analysis can reveal secret keys
     * - Fault-resistant implementations are required for high-security applications
     * - Side-channel attacks often combine timing and fault analysis
     * - Physical security evaluation requires comprehensive fault testing
     * 
     * Mathematical Foundation:
     * For fault injection analysis, we consider:
     * - Normal execution: f(x) = correct result
     * - Faulty execution: f'(x) = result under fault condition
     * - Fault differential: Δ = f(x) ⊕ f'(x)
     * - Security condition: Δ should not reveal information about secret inputs
     * 
     * Attack Models Covered:
     * - Bit flip faults: Single or multiple bit inversions
     * - Instruction skip faults: Incomplete execution paths
     * - Timing faults: Clock glitch induced errors
     * - Variable corruption: Memory/register modification
     * - Branch corruption: Control flow manipulation
     */
     struct FaultResult
     {
         // Test execution metadata
         uint64_t iteration;                 ///< Test iteration number for tracking
         std::string fault_type;             ///< Type of fault injected ("bit_flip", "timing", "instruction_skip", etc.)
         uint64_t fault_timestamp;           ///< CPU timestamp when fault was injected
         
         // Input and fault configuration
         uint32_t original_input;            ///< Input value before fault injection
         uint32_t faulted_input;             ///< Input value after fault injection (if input was modified)
         uint32_t fault_position;            ///< Position/location of injected fault
         double fault_intensity;             ///< Intensity or magnitude of the fault (0.0-1.0)
         
         // Execution results
         uint32_t normal_output;             ///< Expected output under normal execution
         uint32_t faulted_output;            ///< Actual output under fault condition
         uint32_t output_difference;         ///< XOR difference between normal and faulted outputs
         bool success;                       ///< True if operation completed without exceptions
         bool exception_thrown;              ///< True if fault caused an exception
         std::string exception_message;      ///< Exception message if one occurred
         
         // Timing analysis
         double normal_execution_time;       ///< Execution time under normal conditions (nanoseconds)
         double faulted_execution_time;      ///< Execution time under fault conditions (nanoseconds)
         double timing_difference;           ///< Absolute difference in execution times
         bool timing_anomaly;                ///< True if timing differs significantly from expected
         
         // Security analysis fields  
         bool information_leaked;            ///< True if fault revealed secret information
         double leakage_confidence;          ///< Confidence level of information leakage (0.0-1.0)
         std::vector security_implications; ///< List of identified security issues
         
         // Statistical properties
         uint32_t hamming_distance;          ///< Hamming distance between normal and faulted outputs
         double correlation_with_secret;     ///< Correlation coefficient with secret data
         bool exploitable_pattern;          ///< True if fault creates exploitable patterns
         
         /**
          * @brief Default constructor initializing all fields
          */
         FaultResult() : iteration(0), fault_timestamp(0), original_input(0), faulted_input(0),
                        fault_position(0), fault_intensity(0.0), normal_output(0), faulted_output(0),
                        output_difference(0), success(false), exception_thrown(false),
                        normal_execution_time(0.0), faulted_execution_time(0.0), 
                        timing_difference(0.0), timing_anomaly(false), information_leaked(false),
                        leakage_confidence(0.0), hamming_distance(0), correlation_with_secret(0.0),
                        exploitable_pattern(false) {}
         
         /**
          * @brief Calculate the impact severity of the fault injection
          * @return Severity score: 0.0 = no impact, 1.0 = critical security impact
          */
         double calculateImpactSeverity() const
         {
             double severity = 0.0;
             
             // Base severity from output difference
             if (output_difference != 0) {
                 severity += 0.2;  // Any change in output is notable
                 
                 // Additional severity based on number of changed bits
                 double bit_change_ratio = static_cast(__builtin_popcount(output_difference)) / 32.0;
                 severity += bit_change_ratio * 0.3;  // Up to 0.3 for complete bit change
             }
             
             // Timing anomaly contribution
             if (timing_anomaly) {
                 severity += 0.2;
             }
             
             // Exception handling impact
             if (exception_thrown) {
                 severity += 0.1;  // Exceptions can reveal implementation details
             }
             
             // Information leakage is critical
             if (information_leaked) {
                 severity += leakage_confidence * 0.5;  // Up to 0.5 based on confidence
             }
             
             // Exploitable patterns are high risk
             if (exploitable_pattern) {
                 severity += 0.3;
             }
             
             // High correlation with secrets is dangerous
             severity += std::abs(correlation_with_secret) * 0.3;
             
             return std::min(severity, 1.0);  // Cap at 1.0
         }
         
         /**
          * @brief Determine if this fault result indicates a security vulnerability
          * @return True if fault injection reveals a potential security weakness
          */
         bool indicatesVulnerability() const
         {
             // High impact severity indicates vulnerability
             if (calculateImpactSeverity() > 0.7) {
                 return true;
             }
             
             // Direct information leakage
             if (information_leaked && leakage_confidence > 0.5) {
                 return true;
             }
             
             // Strong correlation with secret data
             if (std::abs(correlation_with_secret) > 0.3) {
                 return true;
             }
             
             // Consistent exploitable patterns
             if (exploitable_pattern) {
                 return true;
             }
             
             // Significant timing anomalies can be exploited
             if (timing_anomaly && timing_difference > 1000.0) {  // >1μs timing difference
                 return true;
             }
             
             return false;
         }
         
         /**
          * @brief Get a human-readable description of the fault type and impact
          * @return Descriptive string explaining the fault injection result
          */
         std::string getDescription() const
         {
             std::ostringstream desc;
             desc << "Fault Injection Analysis (Iteration " << iteration << "):\n";
             desc << "  Fault Type: " << fault_type;
             
             if (fault_position > 0) {
                 desc << " at position " << fault_position;
             }
             
             desc << "\n  Impact: ";
             
             if (output_difference != 0) {
                 desc << "Output changed (" << __builtin_popcount(output_difference) << " bits affected), ";
             }
             
             if (timing_anomaly) {
                 desc << "Timing anomaly detected (" << timing_difference << "ns difference), ";
             }
             
             if (exception_thrown) {
                 desc << "Exception: " << exception_message << ", ";
             }
             
             if (information_leaked) {
                 desc << "Information leakage detected (confidence: " 
                      << (leakage_confidence * 100.0) << "%), ";
             }
             
             desc << "\n  Security Assessment: ";
             if (indicatesVulnerability()) {
                 desc << "VULNERABLE (severity: " << (calculateImpactSeverity() * 100.0) << "%)";
             } else {
                 desc << "Secure (no significant impact detected)";
             }
             
             return desc.str();
         }
         
         /**
          * @brief Generate detailed fault analysis report
          * @return Comprehensive analysis of the fault injection experiment
          */
         std::string generateDetailedReport() const
         {
             std::ostringstream report;
             report << "=== FAULT INJECTION DETAILED ANALYSIS ===\n";
             report << "Iteration: " << iteration << "\n";
             report << "Timestamp: " << fault_timestamp << "\n";
             report << "Fault Type: " << fault_type << "\n\n";
             
             report << "INPUT ANALYSIS:\n";
             report << "  Original Input: 0x" << std::hex << original_input << std::dec << "\n";
             if (faulted_input != original_input) {
                 report << "  Faulted Input:  0x" << std::hex << faulted_input << std::dec << "\n";
                 report << "  Input XOR Diff: 0x" << std::hex << (original_input ^ faulted_input) << std::dec << "\n";
             }
             report << "  Fault Position: " << fault_position << "\n";
             report << "  Fault Intensity: " << (fault_intensity * 100.0) << "%\n\n";
             
             report << "OUTPUT ANALYSIS:\n";
             report << "  Normal Output:  0x" << std::hex << normal_output << std::dec << "\n";
             report << "  Faulted Output: 0x" << std::hex << faulted_output << std::dec << "\n";
             report << "  Output XOR Diff: 0x" << std::hex << output_difference << std::dec << "\n";
             report << "  Hamming Distance: " << hamming_distance << " bits\n";
             report << "  Bit Change Ratio: " << (static_cast(hamming_distance) / 32.0 * 100.0) << "%\n\n";
             
             report << "TIMING ANALYSIS:\n";
             report << "  Normal Time: " << normal_execution_time << " ns\n";
             report << "  Faulted Time: " << faulted_execution_time << " ns\n";
             report << "  Time Difference: " << timing_difference << " ns\n";
             report << "  Timing Anomaly: " << (timing_anomaly ? "YES" : "NO") << "\n\n";
             
             report << "SECURITY ANALYSIS:\n";
             report << "  Success: " << (success ? "YES" : "NO") << "\n";
             report << "  Exception: " << (exception_thrown ? ("YES - " + exception_message) : "NO") << "\n";
             report << "  Information Leaked: " << (information_leaked ? "YES" : "NO") << "\n";
             if (information_leaked) {
                 report << "  Leakage Confidence: " << (leakage_confidence * 100.0) << "%\n";
             }
             report << "  Secret Correlation: " << correlation_with_secret << "\n";
             report << "  Exploitable Pattern: " << (exploitable_pattern ? "YES" : "NO") << "\n";
             report << "  Impact Severity: " << (calculateImpactSeverity() * 100.0) << "%\n";
             report << "  Vulnerability Risk: " << (indicatesVulnerability() ? "HIGH" : "LOW") << "\n\n";
             
             if (!security_implications.empty()) {
                 report << "SECURITY IMPLICATIONS:\n";
                 for (const auto& implication : security_implications) {
                     report << "  - " << implication << "\n";
                 }
                 report << "\n";
             }
             
             report << "RECOMMENDATION: ";
             if (indicatesVulnerability()) {
                 report << "IMPLEMENT FAULT DETECTION/CORRECTION MECHANISMS";
             } else {
                 report << "Continue monitoring with additional fault scenarios";
             }
             report << "\n";
             
             return report.str();
         }
         
         /**
          * @brief Compare this fault result with another for differential analysis
          * @param other Another fault result to compare with
          * @return Correlation score indicating similarity of fault effects
          */
         double compareWith(const FaultResult& other) const
         {
             double similarity = 0.0;
             
             // Compare output differences
             uint32_t output_correlation = ~(output_difference ^ other.output_difference);
             similarity += (static_cast(__builtin_popcount(output_correlation)) / 32.0) * 0.4;
             
             // Compare timing patterns
             double timing_correlation = 1.0 - std::abs(timing_difference - other.timing_difference) / 
                                               std::max(timing_difference, other.timing_difference);
             similarity += timing_correlation * 0.2;
             
             // Compare security implications
             if (information_leaked == other.information_leaked) {
                 similarity += 0.2;
             }
             
             if (exploitable_pattern == other.exploitable_pattern) {
                 similarity += 0.1;
             }
             
             // Compare exception behavior
             if (exception_thrown == other.exception_thrown) {
                 similarity += 0.1;
             }
             
             return similarity;
         }
     };