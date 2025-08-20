/**
     * @brief Bit distribution analysis for cryptographic randomness evaluation
     * 
     * The BitDistributionAnalysis class provides comprehensive statistical analysis
     * of bit patterns in cryptographic operations to detect non-random distributions
     * that could indicate security weaknesses. Proper bit distribution is essential
     * for cryptographic security, as biased patterns can be exploited by attackers.
     * 
     * Mathematical Foundation:
     * For a truly random bit sequence of length n:
     * - Each bit position should have P(0) = P(1) = 0.5
     * - Runs of consecutive identical bits should follow geometric distribution
     * - No detectable patterns or correlations should exist
     * - Chi-squared test: χ² = Σ(Observed - Expected)² / Expected should be close to degrees of freedom
     * 
     * Security Implications:
     * - Biased bit distributions reveal implementation flaws
     * - Pattern detection enables cryptanalytic attacks
     * - Poor randomness quality compromises key generation
     * - Statistical biases can be amplified through operations
     * 
     * Analysis Tests:
     * - Frequency Test: Distribution of 0s and 1s in each bit position
     * - Runs Test: Distribution of consecutive identical bits
     * - Chi-squared Goodness of Fit: Statistical randomness evaluation
     * - Serial Correlation: Detection of bit position correlations
     * - Pattern Analysis: Search for recurring bit patterns
     * - Entropy Estimation: Information-theoretic randomness measure
     */
     struct BitDistributionResult
     {
         std::vector<uint32_t> bit_counts_0;      ///< Count of 0s in each bit position (32 positions)
         std::vector<uint32_t> bit_counts_1;      ///< Count of 1s in each bit position (32 positions)
         std::vector<double> bit_probabilities_0; ///< Probability of 0 in each position
         std::vector<double> bit_probabilities_1; ///< Probability of 1 in each position
         std::vector<double> chi_squared_values;  ///< Chi-squared statistic for each bit position
         std::vector<uint32_t> run_lengths_0;     ///< Lengths of consecutive runs of 0s
         std::vector<uint32_t> run_lengths_1;     ///< Lengths of consecutive runs of 1s
         double overall_entropy;                  ///< Overall bit sequence entropy
         double randomness_score;                 ///< Composite randomness score (0-1)
         uint32_t total_samples;                  ///< Total number of samples analyzed
         
         /**
          * @brief Initialize result structure for 32-bit analysis
          */
         BitDistributionResult() : bit_counts_0(32, 0), bit_counts_1(32, 0),
                                  bit_probabilities_0(32, 0.0), bit_probabilities_1(32, 0.0),
                                  chi_squared_values(32, 0.0), overall_entropy(0.0),
                                  randomness_score(0.0), total_samples(0) {}
         
         /**
          * @brief Calculate bit bias for a specific position
          * @param position Bit position (0-31)
          * @return Bias value: 0.0 = perfect balance, 1.0 = completely biased
          */
         double calculateBitBias(uint32_t position) const
         {
             if (position >= 32 || total_samples == 0) return 1.0;
             
             double prob_0 = bit_probabilities_0[position];
             double prob_1 = bit_probabilities_1[position];
             
             return std::abs(prob_0 - prob_1);  // Deviation from 0.5/0.5 balance
         }
         
         /**
          * @brief Check if bit distribution passes statistical tests
          * @return True if distribution appears statistically random
          */
         bool passesRandomnessTests() const
         {
             // Check if overall randomness score meets threshold
             return (randomness_score >= 0.95);  // 95% randomness threshold
         }
     };
     
     class BitDistributionAnalysis
     {
     private:
         BitDistributionResult analysis_result;
         std::vector<uint32_t> sample_data;
         bool analysis_completed = false;
         
         // Statistical test parameters
         static constexpr double CHI_SQUARED_CRITICAL_VALUE = 3.84;  // p=0.05, df=1
         static constexpr double ACCEPTABLE_BIT_BIAS = 0.1;          // 10% maximum bias
         static constexpr uint32_t MIN_SAMPLES_FOR_ANALYSIS = 1000;  // Minimum samples for reliable analysis
         
     public:
         /**
          * @brief Initialize bit distribution analysis
          */
         BitDistributionAnalysis()
         {
             resetAnalysis();
         }
         
         /**
          * @brief Reset analysis state for new evaluation
          */
         void resetAnalysis()
         {
             analysis_result = BitDistributionResult();
             sample_data.clear();
             analysis_completed = false;
         }
         
         /**
          * @brief Add a sample value to the distribution analysis
          * @param original_value Original input value
          * @param processed_value Output value after processing
          * @param operation_parameter Parameter used in the operation (for correlation analysis)
          */
         void addSample(uint32_t original_value, uint32_t processed_value, uint32_t operation_parameter)
         {
             sample_data.push_back(processed_value);
             analysis_result.total_samples++;
             analysis_completed = false;  // Mark analysis as needing update
             
             // Immediate bit counting for efficiency
             for (uint32_t bit_pos = 0; bit_pos < 32; ++bit_pos) {
                 if (processed_value & (1U << bit_pos)) {
                     analysis_result.bit_counts_1[bit_pos]++;
                 } else {
                     analysis_result.bit_counts_0[bit_pos]++;
                 }
             }
         }
         
         /**
          * @brief Perform complete bit distribution analysis
          * @return Comprehensive security analysis result
          */
         SecurityAnalysisResult getResults()
         {
             if (!analysis_completed) {
                 performCompleteAnalysis();
             }
             
             SecurityAnalysisResult result;
             result.test_name = "Bit Distribution Analysis";
             result.analysis_type = "Cryptographic Properties";
             result.security_verdict = SecurityVerdict::SECURE;
             
             if (analysis_result.total_samples < MIN_SAMPLES_FOR_ANALYSIS) {
                 result.security_verdict = SecurityVerdict::INCONCLUSIVE;
                 result.detailed_report = "Insufficient samples for reliable bit distribution analysis";
                 return result;
             }
             
             result.test_statistic = analysis_result.randomness_score;
             result.correlation_coefficient = analysis_result.overall_entropy;
             
             // Evaluate security based on distribution analysis
             evaluateBitDistributionSecurity(result);
             
             // Generate comprehensive analysis report
             result.detailed_report = generateDistributionReport();
             
             return result;
         }
         
         /**
          * @brief Get detailed bit distribution results
          * @return Complete bit distribution analysis results
          */
         const BitDistributionResult& getBitDistributionResults()
         {
             if (!analysis_completed) {
                 performCompleteAnalysis();
             }
             return analysis_result;
         }
         
         /**
          * @brief Calculate monobit frequency test for randomness
          * @return P-value for monobit test (higher is better, >0.01 passes)
          */
         double calculateMonobitTest() const
         {
             if (sample_data.empty()) return 0.0;
             
             uint32_t total_ones = 0;
             uint32_t total_bits = sample_data.size() * 32;
             
             for (uint32_t value : sample_data) {
                 total_ones += __builtin_popcount(value);
             }
             
             uint32_t total_zeros = total_bits - total_ones;
             double expected_ones = total_bits / 2.0;
             
             // Calculate chi-squared statistic
             double chi_squared = std::pow(total_ones - expected_ones, 2) / expected_ones +
                                std::pow(total_zeros - expected_ones, 2) / expected_ones;
             
             // Convert to approximate p-value
             if (chi_squared > 10.83) return 0.001;
             if (chi_squared > 6.63) return 0.01;
             if (chi_squared > 3.84) return 0.05;
             return 0.1;
         }
         
         /**
          * @brief Analyze runs of consecutive identical bits
          * @return Statistical significance of run distribution
          */
         double analyzeRunDistribution()
         {
             if (!analysis_completed) {
                 performCompleteAnalysis();
             }
             
             // Calculate expected run distribution for random data
             double total_runs = analysis_result.run_lengths_0.size() + analysis_result.run_lengths_1.size();
             if (total_runs == 0) return 0.0;
             
             // Analyze run length distribution
             std::map<uint32_t, uint32_t> run_frequency;
             for (uint32_t length : analysis_result.run_lengths_0) {
                 run_frequency[length]++;
             }
             for (uint32_t length : analysis_result.run_lengths_1) {
                 run_frequency[length]++;
             }
             
             // Calculate chi-squared for run distribution
             double chi_squared = 0.0;
             for (const auto& [length, freq] : run_frequency) {
                 // Expected frequency for geometric distribution: runs of length k have probability (1/2)^k
                 double expected_freq = total_runs * std::pow(0.5, length);
                 if (expected_freq > 5.0) {  // Chi-squared test validity requirement
                     chi_squared += std::pow(freq - expected_freq, 2) / expected_freq;
                 }
             }
             
             return chi_squared;
         }
         
         /**
          * @brief Detect patterns and correlations in bit sequences
          * @return Pattern analysis score (0-1, higher is better randomness)
          */
         double analyzePatternComplexity() const
         {
             if (sample_data.size() < 100) return 0.0;  // Need sufficient data
             
             // Analyze 8-bit patterns (256 possible patterns)
             std::map<uint8_t, uint32_t> pattern_counts;
             
             for (size_t i = 0; i < sample_data.size(); ++i) {
                 uint32_t value = sample_data[i];
                 
                 // Extract 4 byte patterns from each 32-bit value
                 for (int shift = 0; shift < 32; shift += 8) {
                     uint8_t byte_pattern = (value >> shift) & 0xFF;
                     pattern_counts[byte_pattern]++;
                 }
             }
             
             // Calculate pattern entropy
             uint32_t total_patterns = pattern_counts.size();
             double entropy = 0.0;
             
             for (const auto& [pattern, count] : pattern_counts) {
                 if (count > 0) {
                     double probability = static_cast<double>(count) / (sample_data.size() * 4);
                     entropy -= probability * std::log2(probability);
                 }
             }
             
             // Normalize entropy (max entropy for 8-bit patterns is 8.0)
             return entropy / 8.0;
         }
         
     private:
         /**
          * @brief Perform comprehensive bit distribution analysis
          */
         void performCompleteAnalysis()
         {
             if (analysis_result.total_samples == 0) return;
             
             // Calculate bit probabilities
             for (uint32_t bit_pos = 0; bit_pos < 32; ++bit_pos) {
                 analysis_result.bit_probabilities_0[bit_pos] = 
                     static_cast<double>(analysis_result.bit_counts_0[bit_pos]) / analysis_result.total_samples;
                 analysis_result.bit_probabilities_1[bit_pos] = 
                     static_cast<double>(analysis_result.bit_counts_1[bit_pos]) / analysis_result.total_samples;
                 
                 // Calculate chi-squared for each bit position
                 double expected = analysis_result.total_samples / 2.0;
                 analysis_result.chi_squared_values[bit_pos] = 
                     (std::pow(analysis_result.bit_counts_0[bit_pos] - expected, 2) +
                      std::pow(analysis_result.bit_counts_1[bit_pos] - expected, 2)) / expected;
             }
             
             // Calculate run lengths
             calculateRunLengths();
             
             // Calculate overall entropy
             analysis_result.overall_entropy = calculateOverallEntropy();
             
             // Calculate composite randomness score
             analysis_result.randomness_score = calculateCompositeRandomnessScore();
             
             analysis_completed = true;
         }
         
         /**
          * @brief Calculate run lengths of consecutive identical bits
          */
         void calculateRunLengths()
         {
             analysis_result.run_lengths_0.clear();
             analysis_result.run_lengths_1.clear();
             
             if (sample_data.empty()) return;
             
             // Convert sample data to bit sequence for run analysis
             std::vector<bool> bit_sequence;
             for (uint32_t value : sample_data) {
                 for (int bit_pos = 0; bit_pos < 32; ++bit_pos) {
                     bit_sequence.push_back((value >> bit_pos) & 1);
                 }
             }
             
             // Find runs of consecutive identical bits
             if (!bit_sequence.empty()) {
                 bool current_bit = bit_sequence[0];
                 uint32_t run_length = 1;
                 
                 for (size_t i = 1; i < bit_sequence.size(); ++i) {
                     if (bit_sequence[i] == current_bit) {
                         run_length++;
                     } else {
                         // End of run
                         if (current_bit) {
                             analysis_result.run_lengths_1.push_back(run_length);
                         } else {
                             analysis_result.run_lengths_0.push_back(run_length);
                         }
                         
                         current_bit = bit_sequence[i];
                         run_length = 1;
                     }
                 }
                 
                 // Handle final run
                 if (current_bit) {
                     analysis_result.run_lengths_1.push_back(run_length);
                 } else {
                     analysis_result.run_lengths_0.push_back(run_length);
                 }
             }
         }
         
         /**
          * @brief Calculate overall entropy of the bit sequence
          * @return Entropy value (higher is better)
          */
         double calculateOverallEntropy() const
         {
             if (analysis_result.total_samples == 0) return 0.0;
             
             uint32_t total_ones = 0;
             for (uint32_t count : analysis_result.bit_counts_1) {
                 total_ones += count;
             }
             
             uint32_t total_bits = analysis_result.total_samples * 32;
             uint32_t total_zeros = total_bits - total_ones;
             
             if (total_ones == 0 || total_zeros == 0) return 0.0;  // No entropy
             
             double p_one = static_cast<double>(total_ones) / total_bits;
             double p_zero = static_cast<double>(total_zeros) / total_bits;
             
             return -(p_one * std::log2(p_one) + p_zero * std::log2(p_zero));
         }
         
         /**
          * @brief Calculate composite randomness score combining multiple tests
          * @return Composite score between 0.0 and 1.0
          */
         double calculateCompositeRandomnessScore() const
         {
             double monobit_score = calculateMonobitTest() * 10.0;  // Scale p-value
             double entropy_score = analysis_result.overall_entropy;  // Already 0-1
             double pattern_score = analyzePatternComplexity();
             
             // Bit position balance score
             double balance_score = 1.0;
             for (uint32_t bit_pos = 0; bit_pos < 32; ++bit_pos) {
                 double bias = analysis_result.calculateBitBias(bit_pos);
                 balance_score *= (1.0 - bias);
             }
             
             // Chi-squared score
             double chi_score = 1.0;
             for (double chi_val : analysis_result.chi_squared_values) {
                 if (chi_val > CHI_SQUARED_CRITICAL_VALUE) {
                     chi_score *= 0.5;  // Penalize high chi-squared values
                 }
             }
             
             // Weighted combination of scores
             return (0.3 * std::min(monobit_score, 1.0) + 
                    0.3 * entropy_score + 
                    0.2 * pattern_score + 
                    0.1 * balance_score + 
                    0.1 * chi_score);
         }
         
         /**
          * @brief Evaluate bit distribution security and set verdict
          */
         void evaluateBitDistributionSecurity(SecurityAnalysisResult& result)
         {
             // Check overall randomness score
             if (analysis_result.randomness_score < 0.8) {
                 result.security_verdict = SecurityVerdict::VULNERABLE;
                 result.vulnerabilities.push_back(
                     "Poor overall randomness score: " + 
                     std::to_string(analysis_result.randomness_score * 100.0) + "% (target: >80%)"
                 );
             }
             
             // Check individual bit position biases
             for (uint32_t bit_pos = 0; bit_pos < 32; ++bit_pos) {
                 double bias = analysis_result.calculateBitBias(bit_pos);
                 if (bias > ACCEPTABLE_BIT_BIAS) {
                     result.security_verdict = SecurityVerdict::VULNERABLE;
                     result.vulnerabilities.push_back(
                         "Bit position " + std::to_string(bit_pos) + " shows bias: " +
                         std::to_string(bias * 100.0) + "% (target: <10%)"
                     );
                 }
             }
             
             // Check chi-squared values
             for (uint32_t bit_pos = 0; bit_pos < 32; ++bit_pos) {
                 if (analysis_result.chi_squared_values[bit_pos] > CHI_SQUARED_CRITICAL_VALUE) {
                     result.security_verdict = SecurityVerdict::VULNERABLE;
                     result.vulnerabilities.push_back(
                         "Bit position " + std::to_string(bit_pos) + " fails chi-squared test: " +
                         std::to_string(analysis_result.chi_squared_values[bit_pos]) + 
                         " (critical: " + std::to_string(CHI_SQUARED_CRITICAL_VALUE) + ")"
                     );
                 }
             }
             
             // Check monobit test
             double monobit_p = calculateMonobitTest();
             if (monobit_p < 0.01) {
                 result.security_verdict = SecurityVerdict::VULNERABLE;
                 result.vulnerabilities.push_back(
                     "Failed monobit frequency test (p=" + std::to_string(monobit_p) + ")"
                 );
             }
         }
         
         /**
          * @brief Generate comprehensive bit distribution report
          */
         std::string generateDistributionReport() const
         {
             std::ostringstream report;
             report << "Bit Distribution Analysis Report:\n";
             report << "  Total Samples: " << analysis_result.total_samples << "\n";
             report << "  Overall Entropy: " << analysis_result.overall_entropy << "\n";
             report << "  Randomness Score: " << (analysis_result.randomness_score * 100.0) << "%\n";
             report << "  Monobit Test P-value: " << calculateMonobitTest() << "\n";
             report << "  Pattern Complexity: " << (analyzePatternComplexity() * 100.0) << "%\n";
             
             report << "\n  Bit Position Analysis:\n";
             for (uint32_t bit_pos = 0; bit_pos < 32; ++bit_pos) {
                 double bias = analysis_result.calculateBitBias(bit_pos);
                 double chi_squared = analysis_result.chi_squared_values[bit_pos];
                 
                 if (bias > ACCEPTABLE_BIT_BIAS || chi_squared > CHI_SQUARED_CRITICAL_VALUE) {
                     report << "    Bit " << std::setw(2) << bit_pos 
                            << ": bias=" << std::fixed << std::setprecision(3) << bias
                            << ", χ²=" << std::fixed << std::setprecision(2) << chi_squared;
                     
                     if (bias > ACCEPTABLE_BIT_BIAS) report << " [BIASED]";
                     if (chi_squared > CHI_SQUARED_CRITICAL_VALUE) report << " [CHI-FAIL]";
                     report << "\n";
                 }
             }
             
             report << "\n  Run Analysis:\n";
             report << "    Runs of 0s: " << analysis_result.run_lengths_0.size() << "\n";
             report << "    Runs of 1s: " << analysis_result.run_lengths_1.size() << "\n";
             report << "    Run Distribution χ²: " << const_cast<BitDistributionAnalysis*>(this)->analyzeRunDistribution() << "\n";
             
             return report.str();
         }
     };