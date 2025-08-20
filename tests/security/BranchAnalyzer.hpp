/**
     * @brief Branch prediction behavior analysis for detecting timing side-channels
     * 
     * The BranchAnalyzer class provides comprehensive analysis of CPU branch prediction
     * behavior to detect potential side-channel vulnerabilities in cryptographic code.
     * Branch prediction attacks exploit the CPU's speculative execution mechanisms
     * to infer secret information through timing differences caused by mispredicted branches.
     * 
     * Key Capabilities:
     * - CPU branch predictor state profiling and analysis
     * - Detection of secret-dependent conditional branches
     * - Statistical analysis of branch prediction patterns
     * - Entropy calculation for branch sequences
     * - Chi-squared independence testing for branch outcomes
     * - Training/probe attack simulation and detection
     * 
     * Security Analysis Methods:
     * - Branch entropy analysis to detect predictable patterns
     * - Correlation analysis between secret data and branch patterns  
     * - Statistical independence testing using chi-squared tests
     * - Branch predictor state change detection after operations
     * - Training pattern analysis for predictor manipulation attacks
     */
     struct BranchProfile
     {
         std::vector<bool> branch_outcomes;    ///< Sequence of taken/not-taken branches
         double branch_entropy;               ///< Entropy of branch sequence (0=predictable, 1=random)
         double prediction_accuracy;          ///< CPU branch predictor accuracy rate
         uint64_t total_branches;            ///< Total number of branches analyzed
         uint64_t correct_predictions;       ///< Number of correctly predicted branches
         uint64_t mispredictions;            ///< Number of mispredicted branches
         std::map<std::string, double> timing_characteristics;  ///< Timing measurements per branch type
         
         /**
          * @brief Calculate branch entropy H(B) = -Σ P(bi) log2 P(bi)
          * @return Entropy value between 0.0 (predictable) and 1.0 (random)
          */
         double calculateEntropy() const
         {
             if (branch_outcomes.empty()) return 0.0;
             
             size_t taken_count = std::count(branch_outcomes.begin(), branch_outcomes.end(), true);
             size_t total = branch_outcomes.size();
             
             if (taken_count == 0 || taken_count == total) return 0.0;  // Completely predictable
             
             double p_taken = static_cast<double>(taken_count) / total;
             double p_not_taken = 1.0 - p_taken;
             
             return -(p_taken * std::log2(p_taken) + p_not_taken * std::log2(p_not_taken));
         }
     };
     
     class BranchAnalyzer
     {
     private:
         std::vector<BranchProfile> profile_history;
         std::unique_ptr<PerformanceCounters> perf_counters;
         BranchProfile current_profile;
         bool is_profiling = false;
         
         // Branch prediction state tracking
         uint64_t baseline_branch_misses = 0;
         uint64_t baseline_total_branches = 0;
         
     public:
         /**
          * @brief Initialize branch analyzer with performance counter access
          */
         BranchAnalyzer()
         {
             perf_counters = std::make_unique<PerformanceCounters>();
             initializeCounters();
         }
         
         /**
          * @brief Reset branch analyzer state for new analysis session
          * 
          * Clears all previous profiling data and resets performance counters
          * to establish a clean baseline for branch prediction analysis.
          */
         void reset()
         {
             current_profile = BranchProfile{};
             profile_history.clear();
             is_profiling = false;
             
             // Reset performance counters and establish baseline
             perf_counters->reset();
             auto baseline_counters = perf_counters->read();
             baseline_branch_misses = baseline_counters.branch_misses;
             baseline_total_branches = baseline_counters.total_branches;
         }
         
         /**
          * @brief Start branch prediction profiling session
          * 
          * Begins monitoring CPU branch prediction behavior, collecting
          * statistics on branch outcomes, prediction accuracy, and timing.
          */
         void startProfiling()
         {
             if (is_profiling) return;
             
             is_profiling = true;
             current_profile = BranchProfile{};
             perf_counters->reset();
         }
         
         /**
          * @brief Stop profiling and finalize current branch profile
          * @return Completed branch profile with analysis results
          */
         BranchProfile stopProfiling()
         {
             if (!is_profiling) return BranchProfile{};
             
             is_profiling = false;
             
             // Read final performance counters
             auto counters = perf_counters->read();
             
             current_profile.total_branches = counters.total_branches - baseline_total_branches;
             current_profile.mispredictions = counters.branch_misses - baseline_branch_misses;
             current_profile.correct_predictions = current_profile.total_branches - current_profile.mispredictions;
             
             if (current_profile.total_branches > 0) {
                 current_profile.prediction_accuracy = 
                     static_cast<double>(current_profile.correct_predictions) / current_profile.total_branches;
             }
             
             current_profile.branch_entropy = current_profile.calculateEntropy();
             
             profile_history.push_back(current_profile);
             return current_profile;
         }
         
         /**
          * @brief Get current branch profile without stopping profiling
          * @return Current branch prediction profile snapshot
          */
         BranchProfile getProfile() const
         {
             return current_profile;
         }
         
         /**
          * @brief Analyze branch independence using chi-squared statistical test
          * @param secret_profile Branch profile when processing secret data
          * @param public_profile Branch profile when processing public data  
          * @return Chi-squared test statistic and p-value for independence
          */
         std::pair<double, double> analyzeBranchIndependence(
             const BranchProfile& secret_profile,
             const BranchProfile& public_profile) const
         {
             // Chi-squared test for independence: χ² = Σ (Observed - Expected)² / Expected
             
             if (secret_profile.branch_outcomes.empty() || public_profile.branch_outcomes.empty()) {
                 return {0.0, 1.0};  // No data, assume independent
             }
             
             // Calculate observed frequencies
             size_t secret_taken = std::count(secret_profile.branch_outcomes.begin(), 
                                            secret_profile.branch_outcomes.end(), true);
             size_t secret_total = secret_profile.branch_outcomes.size();
             size_t secret_not_taken = secret_total - secret_taken;
             
             size_t public_taken = std::count(public_profile.branch_outcomes.begin(),
                                            public_profile.branch_outcomes.end(), true);
             size_t public_total = public_profile.branch_outcomes.size();
             size_t public_not_taken = public_total - public_taken;
             
             // Calculate expected frequencies assuming independence
             double total_samples = secret_total + public_total;
             double total_taken = secret_taken + public_taken;
             double total_not_taken = secret_not_taken + public_not_taken;
             
             double expected_secret_taken = (secret_total * total_taken) / total_samples;
             double expected_secret_not_taken = (secret_total * total_not_taken) / total_samples;
             double expected_public_taken = (public_total * total_taken) / total_samples;
             double expected_public_not_taken = (public_total * total_not_taken) / total_samples;
             
             // Calculate chi-squared statistic
             double chi_squared = 0.0;
             
             if (expected_secret_taken > 0) {
                 chi_squared += std::pow(secret_taken - expected_secret_taken, 2) / expected_secret_taken;
             }
             if (expected_secret_not_taken > 0) {
                 chi_squared += std::pow(secret_not_taken - expected_secret_not_taken, 2) / expected_secret_not_taken;
             }
             if (expected_public_taken > 0) {
                 chi_squared += std::pow(public_taken - expected_public_taken, 2) / expected_public_taken;
             }
             if (expected_public_not_taken > 0) {
                 chi_squared += std::pow(public_not_taken - expected_public_not_taken, 2) / expected_public_not_taken;
             }
             
             // Calculate p-value (degrees of freedom = 1 for 2x2 contingency table)
             double p_value = calculateChiSquaredPValue(chi_squared, 1);
             
             return {chi_squared, p_value};
         }
         
         /**
          * @brief Detect branch predictor training attacks
          * @param training_profile Branch profile during training phase
          * @param target_profile Branch profile during target execution
          * @param probe_profile Branch profile during probe phase
          * @return Security analysis result indicating training attack detection
          */
         SecurityAnalysisResult detectTrainingAttack(
             const BranchProfile& training_profile,
             const BranchProfile& target_profile, 
             const BranchProfile& probe_profile) const
         {
             SecurityAnalysisResult result;
             result.test_name = "Branch Predictor Training Attack Detection";
             result.analysis_type = "Branch Prediction Security";
             result.security_verdict = SecurityVerdict::SECURE;
             
             // Analyze predictor state changes
             double training_accuracy = training_profile.prediction_accuracy;
             double probe_accuracy = probe_profile.prediction_accuracy;
             double accuracy_change = std::abs(probe_accuracy - training_accuracy);
             
             // Statistical significance threshold for accuracy change
             const double SIGNIFICANT_CHANGE_THRESHOLD = 0.05;  // 5% accuracy change
             
             if (accuracy_change > SIGNIFICANT_CHANGE_THRESHOLD) {
                 result.security_verdict = SecurityVerdict::VULNERABLE;
                 result.vulnerabilities.push_back(
                     "Significant branch predictor state change detected: " +
                     std::to_string(accuracy_change * 100.0) + "% accuracy change"
                 );
             }
             
             // Analyze entropy changes
             double training_entropy = training_profile.branch_entropy;
             double probe_entropy = probe_profile.branch_entropy;
             double entropy_change = std::abs(probe_entropy - training_entropy);
             
             const double ENTROPY_CHANGE_THRESHOLD = 0.1;  // 10% entropy change
             
             if (entropy_change > ENTROPY_CHANGE_THRESHOLD) {
                 result.security_verdict = SecurityVerdict::VULNERABLE;
                 result.vulnerabilities.push_back(
                     "Branch pattern entropy change detected: " +
                     std::to_string(entropy_change) + " entropy difference"
                 );
             }
             
             // Calculate correlation between target execution and probe changes
             double correlation = calculateBranchPatternCorrelation(target_profile, probe_profile);
             result.correlation_coefficient = correlation;
             
             if (std::abs(correlation) > 0.3) {  // 30% correlation threshold
                 result.security_verdict = SecurityVerdict::VULNERABLE;
                 result.vulnerabilities.push_back(
                     "High correlation between target execution and probe pattern: " +
                     std::to_string(correlation)
                 );
             }
             
             result.detailed_report = generateTrainingAttackReport(
                 training_profile, target_profile, probe_profile, accuracy_change, correlation
             );
             
             return result;
         }
         
     private:
         /**
          * @brief Initialize performance counters for branch monitoring
          */
         void initializeCounters()
         {
             // Platform-specific initialization for branch prediction counters
             perf_counters->enableBranchCounters();
             perf_counters->reset();
         }
         
         /**
          * @brief Calculate correlation between two branch pattern profiles
          * @param profile1 First branch profile
          * @param profile2 Second branch profile
          * @return Pearson correlation coefficient (-1 to 1)
          */
         double calculateBranchPatternCorrelation(
             const BranchProfile& profile1,
             const BranchProfile& profile2) const
         {
             if (profile1.branch_outcomes.size() != profile2.branch_outcomes.size() ||
                 profile1.branch_outcomes.empty()) {
                 return 0.0;  // Cannot calculate correlation
             }
             
             // Convert boolean outcomes to numerical values for correlation
             std::vector<double> x, y;
             for (size_t i = 0; i < profile1.branch_outcomes.size(); ++i) {
                 x.push_back(profile1.branch_outcomes[i] ? 1.0 : 0.0);
                 y.push_back(profile2.branch_outcomes[i] ? 1.0 : 0.0);
             }
             
             return calculatePearsonCorrelation(x, y);
         }
         
         /**
          * @brief Calculate p-value for chi-squared distribution
          * @param chi_squared Chi-squared test statistic
          * @param degrees_of_freedom Degrees of freedom
          * @return P-value for the test statistic
          */
         double calculateChiSquaredPValue(double chi_squared, int degrees_of_freedom) const
         {
             // Simplified p-value calculation for chi-squared test
             // For df=1, critical value at p=0.05 is approximately 3.84
             if (degrees_of_freedom == 1) {
                 if (chi_squared > 10.83) return 0.001;  // p < 0.001
                 if (chi_squared > 6.63) return 0.01;    // p < 0.01
                 if (chi_squared > 3.84) return 0.05;    // p < 0.05
                 return 0.1;  // p > 0.05 (not significant)
             }
             
             return 0.1;  // Conservative estimate for other degrees of freedom
         }
         
         /**
          * @brief Generate detailed report for training attack analysis
          */
         std::string generateTrainingAttackReport(
             const BranchProfile& training_profile,
             const BranchProfile& target_profile,
             const BranchProfile& probe_profile,
             double accuracy_change,
             double correlation) const
         {
             std::ostringstream report;
             report << "Branch Predictor Training Attack Analysis:\n";
             report << "  Training Phase:\n";
             report << "    - Branch Accuracy: " << (training_profile.prediction_accuracy * 100.0) << "%\n";
             report << "    - Branch Entropy: " << training_profile.branch_entropy << "\n";
             report << "  Target Execution:\n";
             report << "    - Total Branches: " << target_profile.total_branches << "\n";
             report << "    - Mispredictions: " << target_profile.mispredictions << "\n";
             report << "  Probe Phase:\n";
             report << "    - Branch Accuracy: " << (probe_profile.prediction_accuracy * 100.0) << "%\n";
             report << "    - Branch Entropy: " << probe_profile.branch_entropy << "\n";
             report << "  Analysis Results:\n";
             report << "    - Accuracy Change: " << (accuracy_change * 100.0) << "%\n";
             report << "    - Pattern Correlation: " << correlation << "\n";
             
             return report.str();
         }
     };