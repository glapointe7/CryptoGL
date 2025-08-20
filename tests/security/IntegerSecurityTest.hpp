#pragma once

#include "SecurityTest.hpp"
#include "../../src/core/types/Integer.hpp"
#include "../../src/core/types/Vector.hpp"
#include "../unit/TestContainer.hpp"
#include "TimingAnalyzer.hpp"
#include "CacheAnalyzer.hpp"
#include "BranchAnalyzer.hpp"
#include "AvalancheAnalysis.hpp"
#include "BitDistributionAnalysis.hpp"
#include "FaultResult.hpp"
#include "PerformanceCounters.hpp"
#include <random>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include <map>
#include <utility>
#include <string>

namespace SecurityTests
{
    // Import UnitTests namespace for TestContainer inheritance
    using namespace UnitTests;
    
    class IntegerSecurityTest : public SecurityTest
    {
    protected:
        // Test data sets for security analysis
        std::vector<uint8_t> secret_data_uint8;
        std::vector<uint16_t> secret_data_uint16;
        std::vector<uint32_t> secret_data_uint32;
        std::vector<uint64_t> secret_data_uint64;
        
        std::vector<uint8_t> public_data_uint8;
        std::vector<uint16_t> public_data_uint16;
        std::vector<uint32_t> public_data_uint32;
        std::vector<uint64_t> public_data_uint64;
        
        // Statistical analysis tools
        TimingAnalyzer timing_analyzer;
        CacheAnalyzer cache_analyzer;
        BranchAnalyzer branch_analyzer;
        
        std::mt19937 secure_rng;
        
        void setUp() override
        {
            // Initialize cryptographically secure RNG
            std::array<uint32_t, std::mt19937::state_size> seed_data;
            std::random_device rd;
            std::generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
            std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
            secure_rng.seed(seq);
            
            generateSecurityTestData();
        }
        
        void tearDown() override
        {
            // Secure cleanup - overwrite sensitive test data
            secureZeroMemory(secret_data_uint8);
            secureZeroMemory(secret_data_uint16);
            secureZeroMemory(secret_data_uint32);
            secureZeroMemory(secret_data_uint64);
        }
        
    private:
        void generateSecurityTestData()
        {
            const size_t test_size = 50000;
            
            // Generate diverse secret test patterns
            for (size_t i = 0; i < test_size; ++i)
            {
                secret_data_uint8.push_back(static_cast<uint8_t>(secure_rng()));
                secret_data_uint16.push_back(static_cast<uint16_t>(secure_rng()));
                secret_data_uint32.push_back(static_cast<uint32_t>(secure_rng()));
                secret_data_uint64.push_back(secure_rng());
                
                // Public data with known patterns
                public_data_uint8.push_back(static_cast<uint8_t>(i % 256));
                public_data_uint16.push_back(static_cast<uint16_t>(i % 65536));
                public_data_uint32.push_back(static_cast<uint32_t>(i));
                public_data_uint64.push_back(static_cast<uint64_t>(i));
            }
        }
        
        template<typename T>
        void secureZeroMemory(std::vector<T>& data)
        {
            // Secure memory clearing that cannot be optimized away
            volatile T* ptr = data.data();
            for (size_t i = 0; i < data.size(); ++i) {
                ptr[i] = T{0};
            }
            data.clear();
        }
    };

    // ========================================
    // Timing Attack Resistance Tests
    // ========================================
    
    SECURITY_TEST(IntegerLSBTimingResistance, IntegerSecurityTest)
    {
        using namespace CryptoGL;
        
        setAnalysisType(SecurityAnalysis::TIMING_ATTACK);
        setSampleSize(100000);
        setConfidenceLevel(0.95);
        
        auto test_function = [&](auto value, auto bits) {
            auto start = std::chrono::high_resolution_clock::now();
            volatile auto result = uint32::lsb(value, bits);
            auto end = std::chrono::high_resolution_clock::now();
            (void)result;
            return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        };
        
        // Test timing independence across different bit extraction amounts
        std::vector<std::vector<double>> timing_groups;
        for (uint8_t bits = 1; bits <= 31; ++bits) {
            std::vector<double> timings;
            for (const auto& value : secret_data_uint32) {
                timings.push_back(test_function(value, bits));
            }
            timing_groups.push_back(std::move(timings));
        }
        
        // Statistical analysis for timing uniformity
        analyzeTimingUniformity("LSB extraction timing", timing_groups);
        
        // Verify no correlation between input and timing
        verifyTimingIndependence("LSB timing vs bit count", timing_groups);
    }
    
    SECURITY_TEST(IntegerRotationTimingResistance, IntegerSecurityTest)
    {
        using namespace CryptoGL;
        
        setAnalysisType(SecurityAnalysis::TIMING_ATTACK);
        setSampleSize(100000);
        
        auto test_function = [&](auto value, auto shift) {
            auto start = std::chrono::high_resolution_clock::now();
            volatile auto result = uint32::rotateLeft(value, shift);
            auto end = std::chrono::high_resolution_clock::now();
            (void)result;
            return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        };
        
        // Test timing across all possible rotation amounts
        std::map<uint8_t, std::vector<double>> rotation_timings;
        
        for (uint8_t shift = 0; shift < 32; ++shift) {
            std::vector<double> timings;
            for (const auto& value : secret_data_uint32) {
                timings.push_back(test_function(value, shift));
            }
            rotation_timings[shift] = std::move(timings);
        }
        
        // Verify constant-time rotation
        analyzeRotationTimingUniformity(rotation_timings);
        
        // Check for shift-amount timing correlation
        verifyShiftIndependentTiming(rotation_timings);
    }
    
    SECURITY_TEST(IntegerStringConversionTimingResistance, IntegerSecurityTest)
    {
        using namespace CryptoGL;
        
        setAnalysisType(SecurityAnalysis::TIMING_ATTACK);
        setSampleSize(50000);
        
        auto test_function = [&](auto value) {
            auto start = std::chrono::high_resolution_clock::now();
            volatile auto result = uint64::toString(value);
            auto end = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        };
        
        // Group by number of digits to detect magnitude-based timing
        std::map<size_t, std::vector<double>> digit_timings;
        
        for (const auto& value : secret_data_uint64) {
            auto timing = test_function(value);
            size_t digits = std::to_string(value).length();
            digit_timings[digits].push_back(timing);
        }
        
        // Verify timing independence from number magnitude
        analyzeMagnitudeTimingIndependence(digit_timings);
        
        // Check for digit-count correlation
        verifyDigitCountIndependence(digit_timings);
    }
    
    // ========================================
    // Cache-Timing Attack Resistance Tests
    // ========================================
    
    SECURITY_TEST(IntegerBitAccessCacheResistance, IntegerSecurityTest)
    {
        using namespace CryptoGL;
        
        setAnalysisType(SecurityAnalysis::CACHE_TIMING);
        setSampleSize(100000);
        
        // Prime and probe cache analysis
        auto cache_test = [&](uint8_t position) {
            cache_analyzer.prime();
            
            for (const auto& value : secret_data_uint32) {
                volatile bool bit = uint32::getBitAtPosition(value, position);
                (void)bit;
            }
            
            return cache_analyzer.probe();
        };
        
        std::vector<CacheProfile> cache_profiles;
        for (uint8_t pos = 0; pos < 32; ++pos) {
            cache_profiles.push_back(cache_test(pos));
        }
        
        // Verify uniform cache behavior across bit positions
        analyzeCacheUniformity("Bit position access", cache_profiles);
        
        // Check for position-dependent cache patterns
        verifyPositionIndependentCache(cache_profiles);
    }
    
    SECURITY_TEST(IntegerByteSwapCacheResistance, IntegerSecurityTest)
    {
        using namespace CryptoGL;
        
        setAnalysisType(SecurityAnalysis::CACHE_TIMING);
        setSampleSize(50000);
        
        auto cache_test = [&](const auto& data_set) {
            cache_analyzer.prime();
            
            for (const auto& value : data_set) {
                volatile auto result = uint32::bytesSwap(value);
                (void)result;
            }
            
            return cache_analyzer.probe();
        };
        
        // Test different data patterns for cache behavior
        std::vector<CacheProfile> profiles;
        profiles.push_back(cache_test(secret_data_uint32));
        profiles.push_back(cache_test(public_data_uint32));
        
        // Verify cache behavior is independent of data content
        analyzeCacheDataIndependence("Byte swap operations", profiles);
    }
    
    // ========================================
    // Branch Prediction Attack Resistance
    // ========================================
    
    SECURITY_TEST(IntegerBranchPredictionResistance, IntegerSecurityTest)
    {
        using namespace CryptoGL;
        
        setAnalysisType(SecurityAnalysis::BRANCH_PREDICTION);
        setSampleSize(100000);
        
        // Analyze branch predictor state after operations
        auto branch_test = [&](const auto& test_data, const std::string& operation) {
            branch_analyzer.reset();
            
            for (const auto& value : test_data) {
                if (operation == "lsb") {
                    for (uint8_t bits = 1; bits < 8; ++bits) {
                        volatile auto result = uint8::lsb(static_cast<uint8_t>(value), bits);
                        (void)result;
                    }
                } else if (operation == "bit_access") {
                    for (uint8_t pos = 0; pos < 8; ++pos) {
                        volatile bool bit = uint8::getBitAtPosition(static_cast<uint8_t>(value), pos);
                        (void)bit;
                    }
                }
            }
            
            return branch_analyzer.getProfile();
        };
        
        // Test different data patterns
        auto secret_profile = branch_test(secret_data_uint8, "lsb");
        auto public_profile = branch_test(public_data_uint8, "lsb");
        
        // Verify branch predictor state is data-independent
        analyzeBranchIndependence("LSB operations", secret_profile, public_profile);
        
        // Test bit access patterns
        auto secret_bit_profile = branch_test(secret_data_uint8, "bit_access");
        auto public_bit_profile = branch_test(public_data_uint8, "bit_access");
        
        analyzeBranchIndependence("Bit access operations", secret_bit_profile, public_bit_profile);
    }
    
    // ========================================
    // Input Validation & Boundary Testing
    // ========================================
    
    SECURITY_TEST(IntegerInputValidationSecurity, IntegerSecurityTest)
    {
        using namespace CryptoGL;
        
        setAnalysisType(SecurityAnalysis::INPUT_VALIDATION);
        
        // Test boundary conditions securely
        auto test_boundary = [&](const std::string& operation, auto test_func) {
            std::vector<SecurityResult> results;
            
            try {
                // Test maximum values
                results.push_back(test_func(std::numeric_limits<uint32_t>::max()));
                results.push_back(test_func(std::numeric_limits<uint32_t>::min()));
                
                // Test edge cases
                results.push_back(test_func(0xFFFFFFFF));
                results.push_back(test_func(0x80000000));
                results.push_back(test_func(0x7FFFFFFF));
                results.push_back(test_func(0x00000001));
                results.push_back(test_func(0x00000000));
                
                // Test bit patterns
                results.push_back(test_func(0x55555555)); // Alternating bits
                results.push_back(test_func(0xAAAAAAAA)); // Alternating bits
                
            } catch (const std::exception& e) {
                recordSecurityException(operation, e.what());
            }
            
            return results;
        };
        
        // Test LSB extraction with extreme parameters
        auto lsb_test = [&](uint32_t value) {
            SecurityResult result;
            result.operation = "LSB";
            result.input_value = value;
            
            // Test all possible bit extraction counts
            for (uint8_t bits = 0; bits <= 32; ++bits) {
                try {
                    auto start = std::chrono::high_resolution_clock::now();
                    volatile auto extracted = uint32::lsb(value, bits);
                    auto end = std::chrono::high_resolution_clock::now();
                    
                    result.timing = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                    result.success = true;
                    result.output_value = extracted;
                    
                } catch (...) {
                    result.success = false;
                    result.exception_thrown = true;
                }
            }
            return result;
        };
        
        auto lsb_results = test_boundary("LSB Extraction", lsb_test);
        analyzeInputValidationSecurity("LSB boundary testing", lsb_results);
        
        // Test rotation with extreme shift values
        auto rotation_test = [&](uint32_t value) {
            SecurityResult result;
            result.operation = "Rotation";
            result.input_value = value;
            
            // Test extreme shift values
            std::vector<uint8_t> test_shifts = {0, 1, 31, 32, 33, 63, 64, 255};
            
            for (auto shift : test_shifts) {
                try {
                    auto start = std::chrono::high_resolution_clock::now();
                    volatile auto rotated = uint32::rotateLeft(value, shift);
                    auto end = std::chrono::high_resolution_clock::now();
                    
                    result.timing = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                    result.success = true;
                    result.output_value = rotated;
                    
                } catch (...) {
                    result.success = false;
                    result.exception_thrown = true;
                }
            }
            return result;
        };
        
        auto rotation_results = test_boundary("Rotation", rotation_test);
        analyzeInputValidationSecurity("Rotation boundary testing", rotation_results);
    }
    
    // ========================================
    // Memory Safety & Information Disclosure
    // ========================================
    
    SECURITY_TEST(IntegerMemorySafety, IntegerSecurityTest)
    {
        using namespace CryptoGL;
        
        setAnalysisType(SecurityAnalysis::MEMORY_SAFETY);
        
        // Test for potential buffer overflows in string conversion
        auto memory_safety_test = [&]() {
            std::vector<MemorySafetyResult> results;
            
            // Test string conversion with extreme values
            std::vector<uint64_t> extreme_values = {
                0,
                1,
                std::numeric_limits<uint64_t>::max(),
                std::numeric_limits<uint64_t>::max() - 1,
                0x8000000000000000ULL,
                0x7FFFFFFFFFFFFFFFULL
            };
            
            for (auto value : extreme_values) {
                MemorySafetyResult result;
                result.input_value = value;
                
                try {
                    // Monitor memory allocations during conversion
                    auto memory_before = getCurrentMemoryUsage();
                    auto start = std::chrono::high_resolution_clock::now();
                    
                    auto str_result = uint64::toString(value);
                    
                    auto end = std::chrono::high_resolution_clock::now();
                    auto memory_after = getCurrentMemoryUsage();
                    
                    result.memory_delta = memory_after - memory_before;
                    result.timing = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                    result.output_length = str_result.length();
                    result.success = true;
                    
                    // Verify string content integrity
                    if (str_result.empty() || containsNullBytes(str_result)) {
                        result.security_violation = true;
                        result.violation_type = "Invalid string content";
                    }
                    
                } catch (const std::exception& e) {
                    result.success = false;
                    result.exception_message = e.what();
                }
                
                results.push_back(result);
            }
            
            return results;
        };
        
        auto memory_results = memory_safety_test();
        analyzeMemorySafety("String conversion memory safety", memory_results);
        
        // Test for information disclosure in error conditions
        testInformationDisclosure();
    }
    
    // ========================================
    // Fault Injection Resistance Testing
    // ========================================
    
    SECURITY_TEST(IntegerFaultInjectionResistance, IntegerSecurityTest)
    {
        using namespace CryptoGL;
        
        setAnalysisType(SecurityAnalysis::FAULT_INJECTION);
        setSampleSize(10000);
        
        // Simulate fault injection scenarios
        auto fault_test = [&](const std::string& operation) {
            std::vector<FaultResult> results;
            
            for (size_t i = 0; i < 1000; ++i) {
                FaultResult result;
                result.iteration = i;
                
                uint32_t test_value = secret_data_uint32[i % secret_data_uint32.size()];
                
                try {
                    // Simulate various fault scenarios
                    if (operation == "bit_flip") {
                        // Simulate single bit flip
                        uint32_t faulted_value = test_value ^ (1U << (i % 32));
                        result.faulted_input = faulted_value;
                        
                        auto normal_result = uint32::rotateLeft(test_value, 8);
                        auto faulted_result = uint32::rotateLeft(faulted_value, 8);
                        
                        result.normal_output = normal_result;
                        result.faulted_output = faulted_result;
                        result.output_difference = normal_result ^ faulted_result;
                        
                    } else if (operation == "timing_fault") {
                        // Simulate timing-based faults
                        auto start = std::chrono::high_resolution_clock::now();
                        auto result_val = uint32::rotateLeft(test_value, 8);
                        auto end = std::chrono::high_resolution_clock::now();
                        
                        result.timing = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                        result.normal_output = result_val;
                        
                        // Check for timing anomalies
                        if (result.timing > expected_timing_threshold) {
                            result.timing_anomaly = true;
                        }
                    }
                    
                    result.success = true;
                    
                } catch (...) {
                    result.success = false;
                    result.exception_occurred = true;
                }
                
                results.push_back(result);
            }
            
            return results;
        };
        
        auto bit_flip_results = fault_test("bit_flip");
        analyzeFaultResistance("Bit flip fault injection", bit_flip_results);
        
        auto timing_fault_results = fault_test("timing_fault");
        analyzeFaultResistance("Timing fault injection", timing_fault_results);
    }
    
    // ========================================
    // Cryptographic Property Verification
    // ========================================
    
    SECURITY_TEST(IntegerCryptographicProperties, IntegerSecurityTest)
    {
        using namespace CryptoGL;
        
        setAnalysisType(SecurityAnalysis::CRYPTOGRAPHIC_PROPERTIES);
        
        // Test bit distribution properties
        auto bit_distribution_test = [&]() {
            BitDistributionAnalysis analysis;
            
            // Analyze bit distribution in rotated values
            for (const auto& value : secret_data_uint32) {
                for (uint8_t shift = 1; shift < 32; ++shift) {
                    auto rotated = uint32::rotateLeft(value, shift);
                    analysis.addSample(value, rotated, shift);
                }
            }
            
            return analysis.getResults();
        };
        
        auto distribution_results = bit_distribution_test();
        verifyBitDistributionProperties("Rotation bit distribution", distribution_results);
        
        // Test avalanche effect in operations
        auto avalanche_test = [&]() {
            AvalancheAnalysis analysis;
            
            for (const auto& value : secret_data_uint32) {
                for (uint8_t bit_pos = 0; bit_pos < 32; ++bit_pos) {
                    uint32_t flipped_value = value ^ (1U << bit_pos);
                    
                    auto original_lsb = uint32::lsb(value, 16);
                    auto flipped_lsb = uint32::lsb(flipped_value, 16);
                    
                    analysis.addSample(original_lsb, flipped_lsb, bit_pos);
                }
            }
            
            return analysis.getResults();
        };
        
        auto avalanche_results = avalanche_test();
        verifyAvalancheProperties("LSB extraction avalanche", avalanche_results);
    }
}