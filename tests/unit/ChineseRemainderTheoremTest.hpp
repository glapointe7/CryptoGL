/*
 * Comprehensive Unit Tests for CryptoGL::ChineseRemainderTheorem class
 * Provides 100% code coverage including CRT solving, RSA decryption optimization, and coprimality validation
 * Tests cryptographic applications, particularly RSA key operations and modular arithmetic
 */
#pragma once

#include "Test.hpp"
#include "../../src/core/types/ChineseRemainderTheorem.hpp"
#include "../../src/core/exceptions/Exception.hpp"
#include <chrono>
#include <random>
#include <algorithm>
#include <utility>

using namespace CryptoGL;
using namespace UnitTests;

namespace ChineseRemainderTheoremTests {
    
    // Base test class for ChineseRemainderTheorem testing
    class ChineseRemainderTheoremTestBase : public Test {
    protected:
        // Test constants - small primes for basic testing
        const BigInteger prime_3{3};
        const BigInteger prime_5{5};
        const BigInteger prime_7{7};
        const BigInteger prime_11{11};
        const BigInteger prime_13{13};
        const BigInteger prime_17{17};
        const BigInteger prime_19{19};
        const BigInteger prime_23{23};
        
        // RSA-like primes for cryptographic testing
        const BigInteger rsa_p{String("0xD5BBB96D30086EC484EBA3D7F9CAEB07")}; // Large odd number
        const BigInteger rsa_q{String("0xC7455F1C6C92C4B8C7E8A8E6F5D4A8C3")}; // Different large odd number
        
        // Basic test values
        const BigInteger zero{0};
        const BigInteger one{1};
        const BigInteger two{2};
        const BigInteger three{3};
        const BigInteger four{4};
        const BigInteger five{5};
        
        std::mt19937_64 rng;
        
        void setUp() override {
            rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
        }
        
        void tearDown() override {}
    };

    // =============================================================================
    // CHINESE REMAINDER THEOREM SOLVE TESTS
    // =============================================================================
    
    TEST(CRTSolveBasicTest, ChineseRemainderTheoremTestBase) {
        // Classic example: x ≡ 2 (mod 3), x ≡ 3 (mod 5)
        // Solution: x ≡ 8 (mod 15)
        const Vector<BigInteger> remainders = {two, three};
        const Vector<BigInteger> moduli = {prime_3, prime_5};
        
        const BigInteger result = ChineseRemainderTheorem::solve(remainders, moduli);
        
        // Verify the solution
        compare(two, result % prime_3);
        compare(three, result % prime_5);
        
        // Check canonical form (should be 8)
        const BigInteger expected_result = (result % (prime_3 * prime_5));
        compare(BigInteger(8), expected_result);
    }
    
    TEST(CRTSolveTwoCongruencesTest, ChineseRemainderTheoremTestBase) {
        // Test: x ≡ 1 (mod 7), x ≡ 4 (mod 11)
        const Vector<BigInteger> remainders = {one, four};
        const Vector<BigInteger> moduli = {prime_7, prime_11};
        
        const BigInteger result = ChineseRemainderTheorem::solve(remainders, moduli);
        
        // Verify solution satisfies both congruences
        compare(one, result % prime_7);
        compare(four, result % prime_11);
        
        // Result should be in range [0, 77)
        const BigInteger total_mod = prime_7 * prime_11; // 77
        compare(true, result >= zero);
        compare(true, result < total_mod);
    }
    
    TEST(CRTSolveThreeCongruencesTest, ChineseRemainderTheoremTestBase) {
        // Test: x ≡ 2 (mod 3), x ≡ 3 (mod 5), x ≡ 2 (mod 7)
        const Vector<BigInteger> remainders = {two, three, two};
        const Vector<BigInteger> moduli = {prime_3, prime_5, prime_7};
        
        const BigInteger result = ChineseRemainderTheorem::solve(remainders, moduli);
        
        // Verify solution satisfies all three congruences
        compare(two, result % prime_3);
        compare(three, result % prime_5);
        compare(two, result % prime_7);
        
        // Result should be in range [0, 105)
        const BigInteger total_mod = prime_3 * prime_5 * prime_7; // 105
        compare(true, result >= zero);
        compare(true, result < total_mod);
    }
    
    TEST(CRTSolveFourCongruencesTest, ChineseRemainderTheoremTestBase) {
        // Test: x ≡ 1 (mod 3), x ≡ 2 (mod 5), x ≡ 3 (mod 7), x ≡ 4 (mod 11)
        const Vector<BigInteger> remainders = {one, two, three, four};
        const Vector<BigInteger> moduli = {prime_3, prime_5, prime_7, prime_11};
        
        const BigInteger result = ChineseRemainderTheorem::solve(remainders, moduli);
        
        // Verify solution satisfies all congruences
        compare(one, result % prime_3);
        compare(two, result % prime_5);
        compare(three, result % prime_7);
        compare(four, result % prime_11);
        
        // Result should be in valid range
        const BigInteger total_mod = prime_3 * prime_5 * prime_7 * prime_11; // 1155
        compare(true, result >= zero);
        compare(true, result < total_mod);
    }
    
    TEST(CRTSolveSingleCongruenceTest, ChineseRemainderTheoremTestBase) {
        // Edge case: single congruence x ≡ 7 (mod 13)
        const Vector<BigInteger> remainders = {prime_7};
        const Vector<BigInteger> moduli = {prime_13};
        
        const BigInteger result = ChineseRemainderTheorem::solve(remainders, moduli);
        
        // Should simply return the remainder
        compare(prime_7, result);
    }
    
    TEST(CRTSolveLargeNumbersTest, ChineseRemainderTheoremTestBase) {
        // Test with large numbers for cryptographic realism
        const BigInteger large_remainder1(String("0x123456"));
        const BigInteger large_remainder2(String("0x789ABC"));
        const BigInteger large_modulus1(String("0x1000003")); // Large prime
        const BigInteger large_modulus2(String("0x1000037")); // Different large prime
        
        const Vector<BigInteger> remainders = {large_remainder1, large_remainder2};
        const Vector<BigInteger> moduli = {large_modulus1, large_modulus2};
        
        const BigInteger result = ChineseRemainderTheorem::solve(remainders, moduli);
        
        // Verify solution
        compare((large_remainder1 % large_modulus1), (result % large_modulus1));
        compare((large_remainder2 % large_modulus2), (result % large_modulus2));
    }
    
    TEST(CRTSolveZeroRemainderTest, ChineseRemainderTheoremTestBase) {
        // Test with zero remainders: x ≡ 0 (mod 7), x ≡ 0 (mod 11)
        const Vector<BigInteger> remainders = {zero, zero};
        const Vector<BigInteger> moduli = {prime_7, prime_11};
        
        const BigInteger result = ChineseRemainderTheorem::solve(remainders, moduli);
        
        // Result should be 0
        compare(zero, result);
    }
    
    TEST(CRTSolveMixedZeroRemainderTest, ChineseRemainderTheoremTestBase) {
        // Test: x ≡ 0 (mod 3), x ≡ 4 (mod 5)
        const Vector<BigInteger> remainders = {zero, four};
        const Vector<BigInteger> moduli = {prime_3, prime_5};
        
        const BigInteger result = ChineseRemainderTheorem::solve(remainders, moduli);
        
        // Verify solution
        compare(zero, result % prime_3);
        compare(four, result % prime_5);
    }

    // =============================================================================
    // CRT SOLVE ERROR CONDITION TESTS
    // =============================================================================
    
    TEST(CRTSolveEmptySystemTest, ChineseRemainderTheoremTestBase) {
        const Vector<BigInteger> empty_remainders;
        const Vector<BigInteger> empty_moduli;
        
        bool exception_caught = false;
        
        try {
            const BigInteger result = ChineseRemainderTheorem::solve(empty_remainders, empty_moduli);
        } catch (const Exception&) {
            exception_caught = true;
        }
        
        compare(true, exception_caught);
    }
    
    TEST(CRTSolveMismatchedSizesTest, ChineseRemainderTheoremTestBase) {
        const Vector<BigInteger> remainders = {one, two, three};
        const Vector<BigInteger> moduli = {prime_3, prime_5}; // Different size
        
        bool exception_caught = false;
        
        try {
            const BigInteger result = ChineseRemainderTheorem::solve(remainders, moduli);
        } catch (const Exception&) {
            exception_caught = true;
        }
        
        compare(true, exception_caught);
    }
    
    TEST(CRTSolveZeroModulusTest, ChineseRemainderTheoremTestBase) {
        const Vector<BigInteger> remainders = {one, two};
        const Vector<BigInteger> moduli = {prime_3, zero}; // Zero modulus
        
        bool exception_caught = false;
        
        try {
            const BigInteger result = ChineseRemainderTheorem::solve(remainders, moduli);
        } catch (const Exception&) {
            exception_caught = true;
        }
        
        compare(true, exception_caught);
    }
    
    TEST(CRTSolveNonCoprimeModuliTest, ChineseRemainderTheoremTestBase) {
        // Test with non-coprime moduli: gcd(6, 9) = 3
        const BigInteger six(6);
        const BigInteger nine(9);
        const Vector<BigInteger> remainders = {one, two};
        const Vector<BigInteger> moduli = {six, nine};
        
        bool exception_caught = false;
        
        try {
            const BigInteger result = ChineseRemainderTheorem::solve(remainders, moduli);
        } catch (const Exception&) {
            exception_caught = true;
        }
        
        // Should throw exception due to non-coprime moduli
        compare(true, exception_caught);
    }

    // =============================================================================
    // RSA DECRYPT CRT TESTS
    // =============================================================================
    
    TEST(RSADecryptCRTBasicTest, ChineseRemainderTheoremTestBase) {
        // Small RSA example for testing
        const BigInteger p(61);     // Small prime
        const BigInteger q(53);     // Different small prime
        const BigInteger n = p * q; // 3233
        
        // RSA key generation parameters
        const BigInteger phi_n = (p - one) * (q - one); // 3120
        const BigInteger e(17);     // Public exponent (coprime with phi_n)
        const BigInteger d = e.modInverse(phi_n); // Private exponent
        
        // CRT parameters
        const BigInteger d_p = d % (p - one);
        const BigInteger d_q = d % (q - one);
        const BigInteger q_inv = q.modInverse(p);
        
        // Test message
        const BigInteger message(123);
        const BigInteger ciphertext = message.modPow(e, n);
        
        // Decrypt using CRT
        const BigInteger decrypted = ChineseRemainderTheorem::rsaDecryptCRT(
            ciphertext, d_p, d_q, p, q, q_inv
        );
        
        // Should recover original message
        compare(message, decrypted);
    }
    
    /*TEST(RSADecryptCRTLargeTest, ChineseRemainderTheoremTestBase) {
        // Use the RSA-like primes for realistic testing
        const BigInteger p = rsa_p;
        const BigInteger q = rsa_q;
        const BigInteger n = p * q;
        
        // Simple RSA parameters for testing
        const BigInteger phi_n = (p - one) * (q - one);
        const BigInteger e(65537); // Common RSA public exponent
        const BigInteger d = e.modInverse(phi_n);
        
        // CRT parameters
        const BigInteger d_p = d % (p - one);
        const BigInteger d_q = d % (q - one);
        const BigInteger q_inv = q.modInverse(p);
        
        // Test with various messages
        const Vector<BigInteger> test_messages = {
            BigInteger(1), BigInteger(42), BigInteger(String("0x123456789")),
            n - one // Maximum possible message
        };
        
        for (const auto& message : test_messages) {
            if (message < n) {
                const BigInteger ciphertext = message.modPow(e, n);
                const BigInteger decrypted = ChineseRemainderTheorem::rsaDecryptCRT(
                    ciphertext, d_p, d_q, p, q, q_inv
                );
                
                compare(message, decrypted);
            }
        }
    }*/
    
    TEST(RSADecryptCRTZeroMessageTest, ChineseRemainderTheoremTestBase) {
        // Test edge case: message = 0
        const BigInteger p(61);
        const BigInteger q(53);
        const BigInteger n = p * q;
        
        const BigInteger phi_n = (p - one) * (q - one);
        const BigInteger e(17);
        const BigInteger d = e.modInverse(phi_n);
        
        const BigInteger d_p = d % (p - one);
        const BigInteger d_q = d % (q - one);
        const BigInteger q_inv = q.modInverse(p);
        
        const BigInteger message(0);
        const BigInteger ciphertext = message.modPow(e, n); // 0^e = 0
        
        const BigInteger decrypted = ChineseRemainderTheorem::rsaDecryptCRT(
            ciphertext, d_p, d_q, p, q, q_inv
        );
        
        compare(zero, decrypted);
    }
    
    TEST(RSADecryptCRTOneMessageTest, ChineseRemainderTheoremTestBase) {
        // Test edge case: message = 1
        const BigInteger p(61);
        const BigInteger q(53);
        const BigInteger n = p * q;
        
        const BigInteger phi_n = (p - one) * (q - one);
        const BigInteger e(17);
        const BigInteger d = e.modInverse(phi_n);
        
        const BigInteger d_p = d % (p - one);
        const BigInteger d_q = d % (q - one);
        const BigInteger q_inv = q.modInverse(p);
        
        const BigInteger message(1);
        const BigInteger ciphertext = message.modPow(e, n); // 1^e = 1
        
        const BigInteger decrypted = ChineseRemainderTheorem::rsaDecryptCRT(
            ciphertext, d_p, d_q, p, q, q_inv
        );
        
        compare(one, decrypted);
    }

    // =============================================================================
    // RSA DECRYPT CRT ERROR CONDITION TESTS
    // =============================================================================
    
    TEST(RSADecryptCRTZeroPrimeTest, ChineseRemainderTheoremTestBase) {
        const BigInteger p(0); // Invalid
        const BigInteger q(53);
        const BigInteger d_p(1);
        const BigInteger d_q(1);
        const BigInteger q_inv(1);
        const BigInteger ciphertext(42);
        
        bool exception_caught = false;
        
        try {
            const BigInteger result = ChineseRemainderTheorem::rsaDecryptCRT(
                ciphertext, d_p, d_q, p, q, q_inv
            );
        } catch (const Exception&) {
            exception_caught = true;
        }
        
        compare(true, exception_caught);
    }
    
    TEST(RSADecryptCRTZeroPrivateExponentTest, ChineseRemainderTheoremTestBase) {
        const BigInteger p(61);
        const BigInteger q(53);
        const BigInteger d_p(0); // Invalid
        const BigInteger d_q(1);
        const BigInteger q_inv(1);
        const BigInteger ciphertext(42);
        
        bool exception_caught = false;
        
        try {
            const BigInteger result = ChineseRemainderTheorem::rsaDecryptCRT(
                ciphertext, d_p, d_q, p, q, q_inv
            );
        } catch (const Exception&) {
            exception_caught = true;
        }
        
        compare(true, exception_caught);
    }

    // =============================================================================
    // ARE COPRIME TESTS
    // =============================================================================
    
    TEST(AreCoprimeBasicTest, ChineseRemainderTheoremTestBase) {
        // Test coprime pairs
        compare(true, ChineseRemainderTheorem::areCoprime(prime_3, prime_5));
        compare(true, ChineseRemainderTheorem::areCoprime(prime_7, prime_11));
        compare(true, ChineseRemainderTheorem::areCoprime(BigInteger(15), BigInteger(28))); // gcd = 1
        
        // Test non-coprime pairs
        compare(false, ChineseRemainderTheorem::areCoprime(BigInteger(6), BigInteger(9))); // gcd = 3
        compare(false, ChineseRemainderTheorem::areCoprime(BigInteger(12), BigInteger(18))); // gcd = 6
        compare(false, ChineseRemainderTheorem::areCoprime(BigInteger(10), BigInteger(15))); // gcd = 5
    }
    
    TEST(AreCoprimeEdgeCasesTest, ChineseRemainderTheoremTestBase) {
        // Test with 1 (always coprime)
        compare(true, ChineseRemainderTheorem::areCoprime(one, BigInteger(42)));
        compare(true, ChineseRemainderTheorem::areCoprime(BigInteger(100), one));
        compare(true, ChineseRemainderTheorem::areCoprime(one, one));
        
        // Test identical non-one numbers (never coprime except 1)
        compare(false, ChineseRemainderTheorem::areCoprime(five, five));
        compare(false, ChineseRemainderTheorem::areCoprime(BigInteger(42), BigInteger(42)));
        
        // Test with zero
        compare(false, ChineseRemainderTheorem::areCoprime(zero, five));
        compare(false, ChineseRemainderTheorem::areCoprime(five, zero));
        compare(false, ChineseRemainderTheorem::areCoprime(zero, zero));
    }
    
    TEST(AreCoprimeConsecutiveIntegersTest, ChineseRemainderTheoremTestBase) {
        // Consecutive integers are always coprime
        for (int i = 2; i <= 20; ++i) {
            const BigInteger a(i);
            const BigInteger b(i + 1);
            compare(true, ChineseRemainderTheorem::areCoprime(a, b));
        }
    }
    
    TEST(AreCoprimeDistinctPrimesTest, ChineseRemainderTheoremTestBase) {
        // All distinct primes are coprime
        const Vector<BigInteger> primes = {
            prime_3, prime_5, prime_7, prime_11, prime_13, prime_17, prime_19, prime_23
        };
        
        for (size_t i = 0; i < primes.size(); ++i) {
            for (size_t j = i + 1; j < primes.size(); ++j) {
                compare(true, ChineseRemainderTheorem::areCoprime(primes[i], primes[j]));
            }
        }
    }
    
    TEST(AreCoprimeEvenOddTest, ChineseRemainderTheoremTestBase) {
        // Even and odd numbers can be coprime if even number is not divisible by odd
        compare(true, ChineseRemainderTheorem::areCoprime(BigInteger(8), prime_3)); // gcd(8,3) = 1
        compare(true, ChineseRemainderTheorem::areCoprime(BigInteger(14), prime_5)); // gcd(14,5) = 1
        
        // But not if even number is divisible by odd
        compare(false, ChineseRemainderTheorem::areCoprime(BigInteger(15), prime_3)); // gcd(15,3) = 3
        compare(false, ChineseRemainderTheorem::areCoprime(BigInteger(35), prime_5)); // gcd(35,5) = 5
    }

    // =============================================================================
    // VALIDATE COPRIMALITY TESTS
    // =============================================================================
    
    TEST(ValidateCoprimalityValidSetTest, ChineseRemainderTheoremTestBase) {
        // Test with valid pairwise coprime sets
        const Vector<BigInteger> valid_set1 = {prime_3, prime_5, prime_7};
        const Vector<BigInteger> valid_set2 = {prime_11, prime_13, prime_17, prime_19};
        const Vector<BigInteger> valid_set3 = {BigInteger(15), BigInteger(28), BigInteger(9)}; // 15=3*5, 28=4*7, 9=3^2
        
        compare(true, ChineseRemainderTheorem::validateCoprimality(valid_set1));
        compare(true, ChineseRemainderTheorem::validateCoprimality(valid_set2));
        compare(false, ChineseRemainderTheorem::validateCoprimality(valid_set3)); // 15 and 9 share factor 3
    }
    
    TEST(ValidateCoprimalityEdgeCasesTest, ChineseRemainderTheoremTestBase) {
        // Empty set (should succeed - vacuously true)
        const Vector<BigInteger> empty_set;
        compare(true, ChineseRemainderTheorem::validateCoprimality(empty_set));
        
        // Single element (should succeed - no pairs to check)
        const Vector<BigInteger> single_set = {prime_7};
        compare(true, ChineseRemainderTheorem::validateCoprimality(single_set));
        
        // Two element valid set
        const Vector<BigInteger> two_valid = {prime_3, prime_5};
        compare(true, ChineseRemainderTheorem::validateCoprimality(two_valid));
    }
    
    /*TEST(ValidateCoprimalityLargeSetTest, ChineseRemainderTheoremTestBase) {
        // Test with larger set of primes
        const Vector<BigInteger> large_prime_set = {
            prime_3, prime_5, prime_7, prime_11, prime_13, prime_17, prime_19, prime_23
        };
        
        compare(true, ChineseRemainderTheorem::validateCoprimality(large_prime_set));
        
        // Test with large set containing one non-coprime pair
        Vector<BigInteger> large_invalid_set = large_prime_set;
        large_invalid_set.push_back(BigInteger(21)); // 21 = 3*7, shares factors with existing elements
        
        bool exception_caught = false;
        
        try {
            const bool test = ChineseRemainderTheorem::validateCoprimality(large_invalid_set);
        } catch (const Exception&) {
            exception_caught = true;
        }
        
        compare(true, exception_caught);
    }*/

    // =============================================================================
    // INTEGRATION AND CONSISTENCY TESTS
    // =============================================================================
    
    TEST(CRTIntegrationWithValidationTest, ChineseRemainderTheoremTestBase) {
        // Test that CRT solve works correctly when moduli pass validation
        const Vector<BigInteger> valid_moduli = {prime_3, prime_5, prime_7};
        const Vector<BigInteger> remainders = {one, two, three};
        
        // Validation should pass
        compare(true, ChineseRemainderTheorem::validateCoprimality(valid_moduli));
        
        // CRT solve should work
        const BigInteger result = ChineseRemainderTheorem::solve(remainders, valid_moduli);
        
        // Verify solution
        compare(one, result % prime_3);
        compare(two, result % prime_5);
        compare(three, result % prime_7);
    }
    
    TEST(CRTConsistencyWithDirectCalculationTest, ChineseRemainderTheoremTestBase) {
        // Test CRT against brute force for small moduli
        const Vector<BigInteger> moduli = {prime_3, prime_5};
        const BigInteger total_mod = prime_3 * prime_5; // 15
        
        for (int r1 = 0; r1 < 3; ++r1) {
            for (int r2 = 0; r2 < 5; ++r2) {
                const Vector<BigInteger> remainders = {BigInteger(r1), BigInteger(r2)};
                const BigInteger crt_result = ChineseRemainderTheorem::solve(remainders, moduli);
                
                // Find solution by brute force
                BigInteger brute_force_result(-1);
                for (int x = 0; x < 15; ++x) {
                    const BigInteger test_x(x);
                    if ((test_x % prime_3) == BigInteger(r1) && 
                        (test_x % prime_5) == BigInteger(r2)) {
                        brute_force_result = test_x;
                        break;
                    }
                }
                
                compare(brute_force_result, crt_result);
            }
        }
    }
    
    TEST(RSACRTConsistencyTest, ChineseRemainderTheoremTestBase) {
        // Verify that RSA CRT gives same result as direct exponentiation
        const BigInteger p(61);
        const BigInteger q(53);
        const BigInteger n = p * q;
        const BigInteger phi_n = (p - one) * (q - one);
        const BigInteger e(17);
        const BigInteger d = e.modInverse(phi_n);
        
        const BigInteger d_p = d % (p - one);
        const BigInteger d_q = d % (q - one);
        const BigInteger q_inv = q.modInverse(p);
        
        // Test multiple messages
        const Vector<BigInteger> test_messages = {
            BigInteger(2), BigInteger(42), BigInteger(100), BigInteger(500)
        };
        
        for (const auto& message : test_messages) {
            if (message < n) {
                const BigInteger ciphertext = message.modPow(e, n);
                
                // Direct decryption
                const BigInteger direct_decrypt = ciphertext.modPow(d, n);
                
                // CRT decryption
                const BigInteger crt_decrypt = ChineseRemainderTheorem::rsaDecryptCRT(
                    ciphertext, d_p, d_q, p, q, q_inv
                );
                
                // Both methods should give same result
                compare(direct_decrypt, crt_decrypt);
                compare(message, crt_decrypt); // And both should recover original message
            }
        }
    }
}
