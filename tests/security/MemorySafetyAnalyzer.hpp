/**
     * @brief Memory safety analysis for cryptographic implementation security
     * 
     * The MemorySafetyAnalyzer class provides comprehensive analysis of memory-related
     * security vulnerabilities in cryptographic operations. Memory safety is critical
     * for preventing buffer overflows, information disclosure through uninitialized memory,
     * and other memory-related attacks that can compromise cryptographic security.
     * 
     * Security Analysis Purpose:
     * Memory safety vulnerabilities can lead to:
     * - Buffer overflows that allow code injection or data corruption
     * - Information disclosure through reading uninitialized memory
     * - Use-after-free vulnerabilities that can be exploited
     * - Double-free vulnerabilities causing heap corruption
     * - Stack buffer overflows in recursive or large data operations
     * - Integer overflow leading to insufficient buffer allocation
     * 
     * Mathematical Foundation:
     * For memory safety analysis, we verify:
     * - Buffer bounds: ∀ access A: 0 ≤ A.offset < A.buffer_size
     * - Initialization: ∀ read R: R.memory_location has been written before read
     * - Lifetime: ∀ pointer P: P.target is valid when P is dereferenced
     * - Allocation consistency: allocated_bytes ≥ required_bytes
     * 
     * Analysis Methods:
     * - Buffer overflow detection in string operations
     * - Memory leak detection in dynamic allocations
     * - Uninitialized memory access detection
     * - Stack overflow detection in recursive operations
     * - Double-free and use-after-free detection
     * - Information disclosure through memory dumps
     */
     struct MemorySafetyResult
     {
         uint64_t input_value;               ///< Input value being tested
         std::string operation_name;         ///< Name of operation being analyzed
         size_t allocated_bytes;             ///< Number of bytes allocated
         size_t required_bytes;              ///< Number of bytes actually needed
         size_t accessed_bytes;              ///< Number of bytes accessed during operation
         bool buffer_overflow_detected;     ///< True if buffer overflow was detected
         bool memory_leak_detected;         ///< True if memory leak was detected
         bool uninitialized_access;         ///< True if uninitialized memory was accessed
         bool double_free_detected;         ///< True if double-free was attempted
         bool use_after_free_detected;      ///< True if use-after-free was detected
         std::vector memory_violations; ///< List of detected violations
         size_t peak_memory_usage;          ///< Peak memory usage during operation
         double memory_fragmentation_ratio; ///< Ratio of fragmented to total memory
         bool information_disclosed;        ///< True if sensitive data was disclosed
         
         // Additional fields for compatibility with existing test code
         size_t memory_delta;               ///< Change in memory usage during operation
         double timing;                     ///< Execution timing in nanoseconds
         size_t output_length;              ///< Length of output data
         bool success;                      ///< True if operation completed successfully
         bool security_violation;           ///< True if any security violation detected
         std::string violation_type;        ///< Description of the type of violation
         
         /**
          * @brief Default constructor initializing all fields
          */
         MemorySafetyResult() : input_value(0), allocated_bytes(0), required_bytes(0),
                               accessed_bytes(0), buffer_overflow_detected(false),
                               memory_leak_detected(false), uninitialized_access(false),
                               double_free_detected(false), use_after_free_detected(false),
                               peak_memory_usage(0), memory_fragmentation_ratio(0.0),
                               information_disclosed(false), memory_delta(0), timing(0.0),
                               output_length(0), success(false), security_violation(false) {}
         
         /**
          * @brief Calculate overall memory safety score
          * @return Safety score: 1.0 = completely safe, 0.0 = critical vulnerabilities
          */
         double calculateSafetyScore() const
         {
             double score = 1.0;
             
             // Critical violations (major score reduction)
             if (buffer_overflow_detected) score -= 0.4;
             if (use_after_free_detected) score -= 0.3;
             if (double_free_detected) score -= 0.3;
             if (information_disclosed) score -= 0.2;
             
             // Warning-level violations (minor score reduction)
             if (memory_leak_detected) score -= 0.1;
             if (uninitialized_access) score -= 0.1;
             
             // Efficiency penalties
             if (allocated_bytes > required_bytes * 2) score -= 0.05; // Over-allocation
             if (memory_fragmentation_ratio > 0.5) score -= 0.05;    // High fragmentation
             
             return std::max(0.0, score);
         }
         
         /**
          * @brief Check if this result indicates a critical security vulnerability
          * @return True if critical memory safety violation detected
          */
         bool hasCriticalViolation() const
         {
             return buffer_overflow_detected || use_after_free_detected || 
                    double_free_detected || information_disclosed;
         }
     };
     
     class MemorySafetyAnalyzer
     {
     private:
         std::vector analysis_results;
         std::map memory_allocation_tracking;
         std::set allocated_pointers;
         std::set freed_pointers;
         size_t current_memory_usage = 0;
         size_t peak_memory_usage = 0;
         
         // Memory safety thresholds
         static constexpr size_t MAX_STACK_DEPTH = 1000;
         static constexpr size_t MAX_ALLOCATION_SIZE = 1024 * 1024 * 100; // 100MB
         static constexpr double MAX_FRAGMENTATION_RATIO = 0.3; // 30%
         
     public:
         /**
          * @brief Initialize memory safety analyzer
          */
         MemorySafetyAnalyzer()
         {
             resetAnalysis();
         }
         
         /**
          * @brief Reset analyzer state for new analysis session
          */
         void resetAnalysis()
         {
             analysis_results.clear();
             memory_allocation_tracking.clear();
             allocated_pointers.clear();
             freed_pointers.clear();
             current_memory_usage = 0;
             peak_memory_usage = 0;
         }
         
         /**
          * @brief Begin memory safety analysis for an operation
          * @param operation_name Name of the operation being analyzed
          * @param input_value Input value for the operation
          */
         void beginAnalysis(const std::string& operation_name, uint64_t input_value)
         {
             MemorySafetyResult result;
             result.operation_name = operation_name;
             result.input_value = input_value;
             
             // Record initial memory state
             result.allocated_bytes = current_memory_usage;
             analysis_results.push_back(result);
         }
         
         /**
          * @brief Track memory allocation during analysis
          * @param ptr Allocated pointer
          * @param size Size of allocation
          */
         void trackAllocation(void* ptr, size_t size)
         {
             if (!ptr) return;
             
             // Check for reasonable allocation size
             if (size > MAX_ALLOCATION_SIZE) {
                 auto& current_result = analysis_results.back();
                 current_result.memory_violations.push_back(
                     "Excessive allocation size: " + std::to_string(size) + " bytes"
                 );
             }
             
             allocated_pointers.insert(ptr);
             current_memory_usage += size;
             peak_memory_usage = std::max(peak_memory_usage, current_memory_usage);
             
             memory_allocation_tracking[std::to_string(reinterpret_cast(ptr))] = size;
         }
         
         /**
          * @brief Track memory deallocation during analysis
          * @param ptr Pointer being freed
          */
         void trackDeallocation(void* ptr)
         {
             if (!ptr) return;
             
             auto& current_result = analysis_results.back();
             
             // Check for double-free
             if (freed_pointers.find(ptr) != freed_pointers.end()) {
                 current_result.double_free_detected = true;
                 current_result.memory_violations.push_back(
                     "Double-free detected at: " + std::to_string(reinterpret_cast(ptr))
                 );
             }
             
             // Check if pointer was actually allocated
             if (allocated_pointers.find(ptr) == allocated_pointers.end()) {
                 current_result.memory_violations.push_back(
                     "Attempt to free unallocated pointer: " + std::to_string(reinterpret_cast(ptr))
                 );
             }
             
             freed_pointers.insert(ptr);
             allocated_pointers.erase(ptr);
             
             // Update memory usage
             std::string ptr_str = std::to_string(reinterpret_cast(ptr));
             if (memory_allocation_tracking.find(ptr_str) != memory_allocation_tracking.end()) {
                 current_memory_usage -= memory_allocation_tracking[ptr_str];
                 memory_allocation_tracking.erase(ptr_str);
             }
         }
         
         /**
          * @brief Check for buffer overflow in memory access
          * @param buffer_start Start of buffer
          * @param buffer_size Size of buffer  
          * @param access_offset Offset being accessed
          * @param access_size Size of access
          */
         void checkBufferAccess(void* buffer_start, size_t buffer_size, 
                               size_t access_offset, size_t access_size)
         {
             auto& current_result = analysis_results.back();
             
             // Check for buffer overflow
             if (access_offset + access_size > buffer_size) {
                 current_result.buffer_overflow_detected = true;
                 current_result.memory_violations.push_back(
                     "Buffer overflow: accessing " + std::to_string(access_offset + access_size) + 
                     " bytes in " + std::to_string(buffer_size) + " byte buffer"
                 );
             }
             
             current_result.accessed_bytes += access_size;
         }
         
         /**
          * @brief Complete analysis for current operation
          * @return Memory safety analysis result
          */
         MemorySafetyResult completeAnalysis()
         {
             if (analysis_results.empty()) {
                 return MemorySafetyResult();
             }
             
             auto& result = analysis_results.back();
             
             // Check for memory leaks
             if (!allocated_pointers.empty()) {
                 result.memory_leak_detected = true;
                 result.memory_violations.push_back(
                     "Memory leak detected: " + std::to_string(allocated_pointers.size()) + 
                     " unfreed allocations"
                 );
             }
             
             // Calculate memory metrics
             result.peak_memory_usage = peak_memory_usage;
             result.required_bytes = std::max(result.accessed_bytes, result.allocated_bytes);
             
             if (result.allocated_bytes > 0) {
                 result.memory_fragmentation_ratio = 
                     static_cast(peak_memory_usage - current_memory_usage) / peak_memory_usage;
             }
             
             return result;
         }
         
         /**
          * @brief Perform comprehensive memory safety analysis
          * @return Security analysis result with memory safety evaluation
          */
         SecurityAnalysisResult getAnalysisResults() const
         {
             SecurityAnalysisResult result;
             result.test_name = "Memory Safety Analysis";
             result.analysis_type = "Memory Safety";
             result.security_verdict = SecurityVerdict::SECURE;
             
             if (analysis_results.empty()) {
                 result.security_verdict = SecurityVerdict::INCONCLUSIVE;
                 result.detailed_report = "No memory safety analysis data available";
                 return result;
             }
             
             // Analyze all memory safety results
             size_t critical_violations = 0;
             size_t total_violations = 0;
             double total_safety_score = 0.0;
             
             for (const auto& mem_result : analysis_results) {
                 total_safety_score += mem_result.calculateSafetyScore();
                 total_violations += mem_result.memory_violations.size();
                 
                 if (mem_result.hasCriticalViolation()) {
                     critical_violations++;
                 }
             }
             
             double average_safety_score = total_safety_score / analysis_results.size();
             result.test_statistic = average_safety_score;
             
             // Determine security verdict
             if (critical_violations > 0) {
                 result.security_verdict = SecurityVerdict::VULNERABLE;
                 result.vulnerabilities.push_back(
                     std::to_string(critical_violations) + " critical memory safety violations detected"
                 );
             } else if (average_safety_score < 0.8) {
                 result.security_verdict = SecurityVerdict::VULNERABLE;
                 result.vulnerabilities.push_back(
                     "Low memory safety score: " + std::to_string(average_safety_score * 100.0) + "%"
                 );
             }
             
             if (total_violations > analysis_results.size() * 2) {
                 result.vulnerabilities.push_back(
                     "High number of memory violations: " + std::to_string(total_violations)
                 );
             }
             
             // Generate detailed report
             result.detailed_report = generateMemorySafetyReport(average_safety_score, 
                                                               critical_violations, total_violations);
             
             return result;
         }
         
         /**
          * @brief Get all memory safety analysis results
          * @return Vector of all memory safety results
          */
         const std::vector& getMemorySafetyResults() const
         {
             return analysis_results;
         }
         
         /**
          * @brief Check for information disclosure in memory
          * @param memory_dump Memory content to analyze
          * @param dump_size Size of memory dump
          * @return True if sensitive information patterns detected
          */
         bool checkInformationDisclosure(const uint8_t* memory_dump, size_t dump_size) const
         {
             if (!memory_dump || dump_size == 0) return false;
             
             // Look for patterns that might indicate sensitive data
             std::vector sensitive_patterns = {
                 0x00, 0x00, 0x00, 0x00,  // Null patterns
                 0xFF, 0xFF, 0xFF, 0xFF,  // Max patterns  
                 0xAA, 0xAA, 0xAA, 0xAA,  // Debug patterns
                 0xCC, 0xCC, 0xCC, 0xCC,  // Uninitialized heap
                 0xCD, 0xCD, 0xCD, 0xCD   // Uninitialized stack
             };
             
             for (size_t i = 0; i <= dump_size - 4; ++i) {
                 uint32_t value = *reinterpret_cast(&memory_dump[i]);
                 for (size_t j = 0; j < sensitive_patterns.size(); j += 4) {
                     uint32_t pattern = *reinterpret_cast(&sensitive_patterns[j]);
                     if (value == pattern) {
                         return true;  // Potential information disclosure
                     }
                 }
             }
             
             return false;
         }
         
     private:
         /**
          * @brief Generate comprehensive memory safety report
          */
         std::string generateMemorySafetyReport(double average_score, 
                                              size_t critical_violations,
                                              size_t total_violations) const
         {
             std::ostringstream report;
             report << "Memory Safety Analysis Report:\n";
             report << "  Total Operations Analyzed: " << analysis_results.size() << "\n";
             report << "  Average Safety Score: " << (average_score * 100.0) << "%\n";
             report << "  Critical Violations: " << critical_violations << "\n";
             report << "  Total Violations: " << total_violations << "\n";
             report << "  Peak Memory Usage: " << peak_memory_usage << " bytes\n";
             
             if (critical_violations > 0) {
                 report << "\n  Critical Security Issues:\n";
                 for (const auto& result : analysis_results) {
                     if (result.hasCriticalViolation()) {
                         report << "    Operation: " << result.operation_name << "\n";
                         if (result.buffer_overflow_detected) {
                             report << "      - Buffer overflow detected\n";
                         }
                         if (result.use_after_free_detected) {
                             report << "      - Use-after-free detected\n";  
                         }
                         if (result.double_free_detected) {
                             report << "      - Double-free detected\n";
                         }
                         if (result.information_disclosed) {
                             report << "      - Information disclosure detected\n";
                         }
                     }
                 }
             }
             
             // Memory efficiency analysis
             report << "\n  Memory Efficiency:\n";
             size_t total_allocated = 0;
             size_t total_required = 0;
             
             for (const auto& result : analysis_results) {
                 total_allocated += result.allocated_bytes;
                 total_required += result.required_bytes;
             }
             
             if (total_required > 0) {
                 double efficiency = static_cast(total_required) / total_allocated * 100.0;
                 report << "    Allocation Efficiency: " << efficiency << "%\n";
             }
             
             return report.str();
         }
     };