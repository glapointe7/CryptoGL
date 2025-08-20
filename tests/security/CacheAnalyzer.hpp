/**
 * @brief Cache analysis framework for detecting cache-timing vulnerabilities
 */
 class CacheAnalyzer
 {
 private:
     struct CacheProfile
     {
         std::vector<double> cache_miss_rates;
         std::vector<uint64_t> cache_access_patterns;
         std::map<size_t, double> cache_line_utilization;
         double average_miss_rate;
         std::string operation_name;
     };
     
     // Hardware performance counters (platform-specific)
     std::unique_ptr<PerformanceCounters> perf_counters;
     
 public:
     CacheAnalyzer()
     {
         perf_counters = std::make_unique<PerformanceCounters>();
         initializeCounters();
     }
     
     /**
      * @brief Prime the cache with known data patterns
      */
     void prime()
     {
         // Fill cache lines with known data to establish baseline
         const size_t cache_size = getCacheSize();
         std::vector<uint8_t> prime_data(cache_size * 2, 0xAA);
         
         // Access memory to fill cache
         volatile uint8_t dummy = 0;
         for (size_t i = 0; i < prime_data.size(); i += 64) { // 64-byte cache lines
             dummy ^= prime_data[i];
         }
         (void)dummy;
         
         // Reset performance counters
         perf_counters->reset();
     }
     
     /**
      * @brief Probe cache state after operation
      */
     CacheProfile probe()
     {
         CacheProfile profile;
         
         // Read performance counters
         auto counters = perf_counters->read();
         
         profile.cache_miss_rates.push_back(counters.l1_miss_rate);
         profile.cache_miss_rates.push_back(counters.l2_miss_rate);
         profile.cache_miss_rates.push_back(counters.l3_miss_rate);
         
         profile.average_miss_rate = (counters.l1_miss_rate + 
                                    counters.l2_miss_rate + 
                                    counters.l3_miss_rate) / 3.0;
         
         // Analyze access patterns
         analyzeCacheAccessPatterns(profile);
         
         return profile;
     }
     
     /**
      * @brief Analyze cache uniformity across different operations
      */
     void analyzeCacheUniformity(const std::string& operation_name,
                                const std::vector<CacheProfile>& profiles)
     {
         SecurityAnalysisResult result;
         result.test_name = operation_name;
         result.analysis_type = "Cache Uniformity";
         
         // Calculate cache behavior statistics
         std::vector<double> miss_rates;
         for (const auto& profile : profiles) {
             miss_rates.push_back(profile.average_miss_rate);
         }
         
         double mean_miss_rate = std::accumulate(miss_rates.begin(), miss_rates.end(), 0.0) / miss_rates.size();
         
         // Calculate standard deviation of miss rates
         double variance = 0.0;
         for (const auto& rate : miss_rates) {
             variance += std::pow(rate - mean_miss_rate, 2);
         }
         double std_dev = std::sqrt(variance / miss_rates.size());
         
         // Coefficient of variation for cache behavior
         double cv = std_dev / mean_miss_rate;
         
         result.test_statistic = cv;
         
         // Security threshold: CV should be < 0.1 for uniform cache behavior
         bool cache_uniform = (cv < 0.1);
         
         result.security_verdict = cache_uniform ? 
             SecurityVerdict::SECURE : SecurityVerdict::VULNERABLE;
             
         if (!cache_uniform) {
             result.vulnerabilities.push_back("Non-uniform cache behavior detected (CV: " + 
                                             std::to_string(cv) + ")");
         }
         
         // Additional analysis: detect cache line correlation
         analyzeCacheLineCorrelation(profiles, result);
         
         recordSecurityResult(result);
     }
     
 private:
     void analyzeCacheAccessPatterns(CacheProfile& profile)
     {
         // Use memory prefetching to detect access patterns
         const size_t test_size = 4096; // 4KB test region
         std::vector<uint8_t> test_memory(test_size);
         
         auto start_counters = perf_counters->read();
         
         // Sequential access pattern
         for (size_t i = 0; i < test_size; i += 64) {
             volatile uint8_t dummy = test_memory[i];
             (void)dummy;
         }
         
         auto sequential_counters = perf_counters->read();
         
         // Random access pattern
         std::random_device rd;
         std::mt19937 gen(rd());
         std::uniform_int_distribution<size_t> dist(0, test_size - 1);
         
         for (size_t i = 0; i < test_size / 64; ++i) {
             size_t random_offset = dist(gen) & ~63; // Align to cache line
             volatile uint8_t dummy = test_memory[random_offset];
             (void)dummy;
         }
         
         auto random_counters = perf_counters->read();
         
         // Store access pattern analysis
         profile.cache_access_patterns.push_back(sequential_counters.cache_misses - start_counters.cache_misses);
         profile.cache_access_patterns.push_back(random_counters.cache_misses - sequential_counters.cache_misses);
     }
     
     void analyzeCacheLineCorrelation(const std::vector<CacheProfile>& profiles,
                                    SecurityAnalysisResult& result)
     {
         // Analyze correlation between input data and cache line usage
         std::vector<double> cache_line_usage;
         
         for (const auto& profile : profiles) {
             double total_usage = 0.0;
             for (const auto& line_usage : profile.cache_line_utilization) {
                 total_usage += line_usage.second;
             }
             cache_line_usage.push_back(total_usage);
         }
         
         // Calculate Pearson correlation coefficient
         double correlation = calculatePearsonCorrelation(cache_line_usage);
         result.correlation_coefficient = correlation;
         
         if (std::abs(correlation) > 0.3) {
             result.vulnerabilities.push_back("High correlation between input and cache usage detected");
             result.security_verdict = SecurityVerdict::VULNERABLE;
         }
     }
     
     size_t getCacheSize() const
     {
         // Platform-specific cache size detection
         #ifdef __linux__
             return sysconf(_SC_LEVEL1_DCACHE_SIZE);
         #elif defined(_WIN32)
             SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX info;
             // Windows-specific cache size detection
             return 32768; // Default L1 cache size
         #else
             return 32768; // Default assumption
         #endif
     }
 };