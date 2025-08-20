/**
     * @brief Avalanche effect analysis for cryptographic security evaluation
     * 
     * The AvalancheAnalysis class provides comprehensive analysis of the avalanche effect
     * in cryptographic operations. The avalanche effect is a critical security property
     * where small changes in input (single bit flip) should cause significant changes
     * in output (approximately 50% of output bits should flip on average).
     * 
     * Mathematical Foundation:
     * For a function f: {0,1}^n → {0,1}^m to satisfy the avalanche criterion:
     * - For any input x and any single bit position i
     * - Let x' = x ⊕ 2^i (flip bit i in x)
     * - Then |{j : f(x)[j] ≠ f(x')[j]}| ≈ m/2
     * 
     * Security Implications:
     * - Strong avalanche effect prevents cryptanalysis
     * - Weak avalanche reveals structural weaknesses
     * - Linear operations typically have poor avalanche
     * - Good cryptographic functions exhibit near-ideal avalanche
     * 
     * Analysis Metrics:
     * - Bit Change Probability: P(output_bit_i changes | input_bit_j flipped)
     * - Avalanche Coefficient: Average percentage of output bits that change
     * - Strict Avalanche Criterion: Each input bit affects each output bit with probability 0.5
     * - Completeness: Each output bit depends on all input bits
     * - Statistical Independence: Output bit changes are uncorrelated
     */
     struct AvalancheResult
     {
         uint32_t input_bit_position;        ///< Position of the flipped input bit (0-31)
         uint32_t original_input;            ///< Original input value before bit flip
         uint32_t modified_input;            ///< Input value after single bit flip
         uint32_t original_output;           ///< Function output for original input
         uint32_t modified_output;           ///< Function output for modified input
         uint32_t output_difference;         ///< XOR of original and modified outputs
         uint32_t changed_bits_count;        ///< Number of output bits that changed
         double bit_change_ratio;            ///< Ratio of changed bits (0.0 to 1.0)
         
         /**
          * @brief Calculate Hamming distance between original and modified outputs
          * @return Number of bits that differ between outputs
          */
         uint32_t calculateHammingDistance() const
         {
             return __builtin_popcount(output_difference);  // Count set bits in XOR result
         }
         
         /**
          * @brief Check if this result satisfies strict avalanche criterion
          * @return True if approximately 50% of output bits changed
          */
         bool satisfiesStrictAvalanche() const
         {
             return (bit_change_ratio >= 0.4 && bit_change_ratio <= 0.6);  // 40-60% range
         }
     };
     
     class AvalancheAnalysis
     {
     private:
         std::vector<AvalancheResult> avalanche_results;
         std::map<uint32_t, std::vector<double>> bit_change_matrix;  // [input_bit][output_bit] → change_probability
         uint32_t total_samples = 0;
         
         // Statistical thresholds for avalanche evaluation
         static constexpr double IDEAL_AVALANCHE_RATIO = 0.5;      // 50% of bits should change
         static constexpr double ACCEPTABLE_DEVIATION = 0.1;       // ±10% from ideal
         static constexpr double MIN_SAMPLES_FOR_STATISTICS = 100;  // Minimum samples for reliable statistics
         
     public:
         /**
          * @brief Initialize avalanche analysis with default parameters
          */
         AvalancheAnalysis()
         {
             resetAnalysis();
         }
         
         /**
          * @brief Reset analysis state for new evaluation session
          */
         void resetAnalysis()
         {
             avalanche_results.clear();
             bit_change_matrix.clear();
             total_samples = 0;
         }
         
         /**
          * @brief Add a single avalanche test sample to the analysis
          * @param original_output Function output for original input
          * @param modified_output Function output for input with single bit flip
          * @param input_bit_position Position of the flipped input bit
          */
         void addSample(uint32_t original_output, uint32_t modified_output, uint32_t input_bit_position)
         {
             AvalancheResult result;
             result.input_bit_position = input_bit_position;
             result.original_output = original_output;
             result.modified_output = modified_output;
             result.output_difference = original_output ^ modified_output;
             result.changed_bits_count = result.calculateHammingDistance();
             result.bit_change_ratio = static_cast<double>(result.changed_bits_count) / 32.0;
             
             avalanche_results.push_back(result);
             total_samples++;
             
             // Update bit change matrix for statistical analysis
             updateBitChangeMatrix(result);
         }
         
         /**
          * @brief Get comprehensive avalanche analysis results
          * @return Security analysis result with avalanche evaluation
          */
         SecurityAnalysisResult getResults() const
         {
             SecurityAnalysisResult result;
             result.test_name = "Avalanche Effect Analysis";
             result.analysis_type = "Cryptographic Properties";
             result.security_verdict = SecurityVerdict::SECURE;
             
             if (total_samples < MIN_SAMPLES_FOR_STATISTICS) {
                 result.security_verdict = SecurityVerdict::INCONCLUSIVE;
                 result.detailed_report = "Insufficient samples for reliable avalanche analysis";
                 return result;
             }
             
             // Calculate overall avalanche statistics
             double average_avalanche_ratio = calculateAverageAvalancheRatio();
             double avalanche_variance = calculateAvalancheVariance();
             double completeness_score = calculateCompletenessScore();
             
             result.test_statistic = average_avalanche_ratio;
             result.correlation_coefficient = completeness_score;
             
             // Evaluate security based on avalanche criteria
             evaluateAvalancheSecurity(result, average_avalanche_ratio, avalanche_variance, completeness_score);
             
             // Generate detailed analysis report
             result.detailed_report = generateAvalancheReport(average_avalanche_ratio, avalanche_variance, completeness_score);
             
             return result;
         }
         
         /**
          * @brief Get all individual avalanche test results
          * @return Vector of all avalanche test results
          */
         const std::vector<AvalancheResult>& getAvalancheResults() const
         {
             return avalanche_results;
         }
         
         /**
          * @brief Calculate strict avalanche criterion compliance
          * @return Percentage of tests that satisfy strict avalanche criterion (0-100%)
          */
         double calculateStrictAvalancheCompliance() const
         {
             if (avalanche_results.empty()) return 0.0;
             
             size_t satisfying_results = std::count_if(
                 avalanche_results.begin(), avalanche_results.end(),
                 [](const AvalancheResult& result) { return result.satisfiesStrictAvalanche(); }
             );
             
             return (static_cast<double>(satisfying_results) / avalanche_results.size()) * 100.0;
         }
         
         /**
          * @brief Analyze bit independence between input and output positions
          * @return Matrix showing correlation between input bit positions and output changes
          */
         std::vector<std::vector<double>> analyzeBitIndependence() const
         {
             std::vector<std::vector<double>> independence_matrix(32, std::vector<double>(32, 0.0));
             
             if (avalanche_results.empty()) return independence_matrix;
             
             // Count bit changes for each input/output bit combination
             std::vector<std::vector<uint32_t>> change_counts(32, std::vector<uint32_t>(32, 0));
             std::vector<uint32_t> input_bit_samples(32, 0);
             
             for (const auto& result : avalanche_results) {
                 input_bit_samples[result.input_bit_position]++;
                 
                 for (uint32_t output_bit = 0; output_bit < 32; ++output_bit) {
                     if (result.output_difference & (1U << output_bit)) {
                         change_counts[result.input_bit_position][output_bit]++;
                     }
                 }
             }
             
             // Calculate change probabilities
             for (uint32_t input_bit = 0; input_bit < 32; ++input_bit) {
                 if (input_bit_samples[input_bit] > 0) {
                     for (uint32_t output_bit = 0; output_bit < 32; ++output_bit) {
                         independence_matrix[input_bit][output_bit] = 
                             static_cast<double>(change_counts[input_bit][output_bit]) / 
                             input_bit_samples[input_bit];
                     }
                 }
             }
             
             return independence_matrix;
         }
         
     private:
         /**
          * @brief Update the bit change probability matrix
          * @param result Latest avalanche result to incorporate
          */
         void updateBitChangeMatrix(const AvalancheResult& result)
         {
             auto& input_bit_data = bit_change_matrix[result.input_bit_position];
             
             // Initialize if first sample for this input bit
             if (input_bit_data.empty()) {
                 input_bit_data.resize(32, 0.0);
             }
             
             // Update change probabilities for each output bit
             for (uint32_t output_bit = 0; output_bit < 32; ++output_bit) {
                 bool bit_changed = (result.output_difference & (1U << output_bit)) != 0;
                 input_bit_data[output_bit] = input_bit_data[output_bit] + (bit_changed ? 1.0 : 0.0);
             }
         }
         
         /**
          * @brief Calculate average avalanche ratio across all samples
          * @return Average ratio of output bits that change per input bit flip
          */
         double calculateAverageAvalancheRatio() const
         {
             if (avalanche_results.empty()) return 0.0;
             
             double total_ratio = 0.0;
             for (const auto& result : avalanche_results) {
                 total_ratio += result.bit_change_ratio;
             }
             
             return total_ratio / avalanche_results.size();
         }
         
         /**
          * @brief Calculate variance in avalanche ratios
          * @return Variance in avalanche effect consistency
          */
         double calculateAvalancheVariance() const
         {
             if (avalanche_results.size() < 2) return 0.0;
             
             double mean_ratio = calculateAverageAvalancheRatio();
             double variance_sum = 0.0;
             
             for (const auto& result : avalanche_results) {
                 double deviation = result.bit_change_ratio - mean_ratio;
                 variance_sum += deviation * deviation;
             }
             
             return variance_sum / (avalanche_results.size() - 1);
         }
         
         /**
          * @brief Calculate completeness score (how well each output bit depends on all input bits)
          * @return Completeness score between 0.0 and 1.0
          */
         double calculateCompletenessScore() const
         {
             auto independence_matrix = analyzeBitIndependence();
             double completeness_sum = 0.0;
             uint32_t valid_entries = 0;
             
             for (uint32_t output_bit = 0; output_bit < 32; ++output_bit) {
                 for (uint32_t input_bit = 0; input_bit < 32; ++input_bit) {
                     double change_prob = independence_matrix[input_bit][output_bit];
                     if (change_prob > 0.0) {  // Only count non-zero probabilities
                         completeness_sum += std::abs(change_prob - 0.5);  // Deviation from ideal 0.5
                         valid_entries++;
                     }
                 }
             }
             
             if (valid_entries == 0) return 0.0;
             
             double average_deviation = completeness_sum / valid_entries;
             return 1.0 - (2.0 * average_deviation);  // Convert to score where 1.0 is perfect
         }
         
         /**
          * @brief Evaluate overall avalanche security and set verdict
          */
         void evaluateAvalancheSecurity(SecurityAnalysisResult& result, 
                                      double average_ratio, 
                                      double variance,
                                      double completeness) const
         {
             // Check if average avalanche ratio is close to ideal (0.5)
             double ratio_deviation = std::abs(average_ratio - IDEAL_AVALANCHE_RATIO);
             
             if (ratio_deviation > ACCEPTABLE_DEVIATION) {
                 result.security_verdict = SecurityVerdict::VULNERABLE;
                 result.vulnerabilities.push_back(
                     "Poor avalanche effect: " + std::to_string(average_ratio * 100.0) + 
                     "% (target: 50.0%)"
                 );
             }
             
             // Check avalanche consistency (low variance is good)
             if (variance > 0.05) {  // 5% variance threshold
                 result.security_verdict = SecurityVerdict::VULNERABLE;
                 result.vulnerabilities.push_back(
                     "Inconsistent avalanche effect (high variance): " + std::to_string(variance)
                 );
             }
             
             // Check completeness score
             if (completeness < 0.7) {  // 70% completeness threshold
                 result.security_verdict = SecurityVerdict::VULNERABLE;
                 result.vulnerabilities.push_back(
                     "Poor completeness (output bits don't depend on all input bits): " + 
                     std::to_string(completeness * 100.0) + "%"
                 );
             }
             
             // Calculate strict avalanche compliance
             double sac_compliance = calculateStrictAvalancheCompliance();
             if (sac_compliance < 70.0) {  // 70% compliance threshold
                 result.security_verdict = SecurityVerdict::VULNERABLE;
                 result.vulnerabilities.push_back(
                     "Poor Strict Avalanche Criterion compliance: " + 
                     std::to_string(sac_compliance) + "% (target: >70%)"
                 );
             }
         }
         
         /**
          * @brief Generate comprehensive avalanche analysis report
          */
         std::string generateAvalancheReport(double average_ratio, double variance, double completeness) const
         {
             std::ostringstream report;
             report << "Avalanche Effect Analysis Report:\n";
             report << "  Total Samples: " << total_samples << "\n";
             report << "  Average Avalanche Ratio: " << (average_ratio * 100.0) << "% (ideal: 50.0%)\n";
             report << "  Avalanche Variance: " << variance << "\n";
             report << "  Completeness Score: " << (completeness * 100.0) << "%\n";
             report << "  Strict Avalanche Compliance: " << calculateStrictAvalancheCompliance() << "%\n";
             
             report << "\n  Bit-Level Analysis:\n";
             auto independence_matrix = analyzeBitIndependence();
             
             // Find input bits with poor avalanche
             for (uint32_t input_bit = 0; input_bit < 32; ++input_bit) {
                 double bit_average = 0.0;
                 uint32_t valid_outputs = 0;
                 
                 for (uint32_t output_bit = 0; output_bit < 32; ++output_bit) {
                     if (independence_matrix[input_bit][output_bit] > 0.0) {
                         bit_average += independence_matrix[input_bit][output_bit];
                         valid_outputs++;
                     }
                 }
                 
                 if (valid_outputs > 0) {
                     bit_average /= valid_outputs;
                     if (std::abs(bit_average - 0.5) > 0.2) {  // Significant deviation
                         report << "    Input bit " << input_bit << ": poor avalanche (" 
                                << (bit_average * 100.0) << "%)\n";
                     }
                 }
             }
             
             return report.str();
         }
     };