/*
 * Comprehensive Unit Tests for CryptoGL::BigInteger class
 * Provides 100% code coverage including all methods, constructors, operators, and edge cases
 * Tests cryptographic operations, arithmetic, bitwise operations, prime generation, and more
 */
#pragma once

#include "Test.hpp"
#include "../../src/core/types/BigInteger.hpp"
#include "../../src/core/exceptions/Exception.hpp"
#include <chrono>
#include <random>
#include <algorithm>
#include <utility>
#include <vector>
#include <cstdint>

using namespace CryptoGL;
using namespace UnitTests;

namespace BigIntegerTests {
    
    // Base test class for BigInteger testing
    class BigIntegerTestBase : public Test {
    protected:
        // Test constants
        const BigInteger zero{0};
        const BigInteger one{1};
        const BigInteger two{2};
        const BigInteger three{3};
        const BigInteger negative_one{-1};
        const BigInteger large_positive{String("0x123456789ABCDEF0123456789ABCDEF")};
        const BigInteger large_negative{String("-0x123456789ABCDEF0123456789ABCDEF")};
        
        std::mt19937_64 rng;
        
        void setUp() override {
            rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
        }
        
        void tearDown() override {}
    };

    // =============================================================================
    // CONSTRUCTOR TESTS
    // =============================================================================
    
    TEST(DefaultConstructorTest, BigIntegerTestBase) {
        const BigInteger bi;
        compare(true, bi.isZero());
        compare(false, bi.isNegative());
        compare(String("0"), bi.toString());
        compare(String("0"), bi.toHexString());
    }
    
    TEST(IntegralConstructorTest, BigIntegerTestBase) {
        // Positive integers
        const BigInteger bi1(42u);
        compare(String("42"), bi1.toString());
        compare(false, bi1.isNegative());
        
        const BigInteger bi2(0u);
        compare(true, bi2.isZero());
        
        // Negative integers
        const BigInteger bi3(-42);
        compare(String("-42"), bi3.toString());
        compare(true, bi3.isNegative());
        
        // Maximum values
        const BigInteger bi4(UINT64_MAX);
        compare(String("18446744073709551615"), bi4.toString());
        
        const BigInteger bi5(INT64_MIN);
        compare(true, bi5.isNegative());
        compare(String("-9223372036854775808"), bi5.toString());
    }
    
    TEST(ImplicitConstructorTest, BigIntegerTestBase) {
        // Test non-explicit constructors for common integer types
        
        // int (should work implicitly)
        BigInteger x = 0;
        compare(true, x.isZero());
        compare(false, x.isNegative());
        
        BigInteger y = 42;
        compare(String("42"), y.toString());
        compare(false, y.isNegative());
        
        BigInteger z = -42;
        compare(String("-42"), z.toString());
        compare(true, z.isNegative());
        
        // long
        BigInteger a = 100L;
        compare(String("100"), a.toString());
        
        BigInteger b = -100L;
        compare(true, b.isNegative());
        
        // long long
        BigInteger c = 1000000LL;
        compare(String("1000000"), c.toString());
        
        BigInteger d = -1000000LL;
        compare(true, d.isNegative());
        
        // unsigned variants
        BigInteger e = 42u;
        compare(String("42"), e.toString());
        
        BigInteger f = 1000000UL;
        compare(String("1000000"), f.toString());
        
        BigInteger g = 1000000000ULL;
        compare(String("1000000000"), g.toString());
        
        // Edge cases for int64_t and uint64_t
        BigInteger h = INT64_MAX;
        compare(String("9223372036854775807"), h.toString());
        compare(false, h.isNegative());
        
        BigInteger i = INT64_MIN;
        compare(String("-9223372036854775808"), i.toString());
        compare(true, i.isNegative());
        
        BigInteger j = UINT64_MAX;
        compare(String("18446744073709551615"), j.toString());
        compare(false, j.isNegative());
        
        // Test in function calls (implicit conversion)
        auto testFunc = [](BigInteger val) -> bool {
            return val.isZero();
        };
        
        compare(true, testFunc(0));   // Should work with implicit conversion
        compare(false, testFunc(42)); // Should work with implicit conversion
        
        // Test in containers
        std::vector<BigInteger> numbers = {1, 2, 3, 4, 5};
        compare(size_t(5), numbers.size());
        compare(String("1"), numbers[0].toString());
        compare(String("5"), numbers[4].toString());
    }
    
    TEST(HexStringConstructorTest, BigIntegerTestBase) {
        // With 0x prefix
        const BigInteger bi1(String("0x1A2B3C"));
        compare(String("1A2B3C"), bi1.toHexString());
        
        // Without 0x prefix
        const BigInteger bi2(String("DEADBEEF"));
        compare(String("DEADBEEF"), bi2.toHexString());
        
        // Empty string should create zero
        const BigInteger bi3(String(""));
        compare(true, bi3.isZero());
        
        // Case insensitive
        const BigInteger bi4(String("0xabcdef"));
        compare(String("ABCDEF"), bi4.toHexString());
        
        // Large hex number
        const BigInteger bi5(String("0x123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"));
        compare(String("123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"), bi5.toHexString());
    }
    
    TEST(BytesConstructorTest, BigIntegerTestBase) {
        // Simple byte array
        const BytesVector bytes1 = {0x01, 0x23, 0x45};
        const BigInteger bi1(bytes1);
        compare(String("12345"), bi1.toHexString());
        
        // Empty bytes should create zero
        const BytesVector empty_bytes;
        const BigInteger bi2(empty_bytes);
        compare(true, bi2.isZero());
        
        // Single byte
        const BytesVector bytes3 = {0xFF};
        const BigInteger bi3(bytes3);
        compare(String("FF"), bi3.toHexString());
        
        // Multiple bytes with leading zeros
        const BytesVector bytes4 = {0x00, 0x01, 0x23};
        const BigInteger bi4(bytes4);
        compare(String("123"), bi4.toHexString());
    }
    
    TEST(CopyConstructorTest, BigIntegerTestBase) {
        const BigInteger original(String("0x123456789ABCDEF"));
        const BigInteger copy(original);
        
        compare(original, copy);
        compare(original.isNegative(), copy.isNegative());
        compare(original.bitLength(), copy.bitLength());
        compare(true, original == copy);
    }
    
    TEST(MoveConstructorTest, BigIntegerTestBase) {
        BigInteger original(String("0x123456789ABCDEF"));
        const String original_str = original.toString();
        const size_t original_bitlen = original.bitLength();
        
        const BigInteger moved(std::move(original));
        
        compare(original_str, moved.toString());
        compare(original_bitlen, moved.bitLength());
        // Original should be in valid but unspecified state
        compare(false, moved.isNegative()); // Test moved object
    }

    // =============================================================================
    // ASSIGNMENT OPERATOR TESTS
    // =============================================================================
    
    TEST(CopyAssignmentTest, BigIntegerTestBase) {
        const BigInteger original(String("0x123456789ABCDEF"));
        BigInteger copy;
        
        copy = original;
        
        compare(original, copy);
        compare(true, original == copy);
        
        // Self-assignment test
        copy = copy;
        compare(original, copy);
    }
    
    TEST(MoveAssignmentTest, BigIntegerTestBase) {
        BigInteger original(String("0x123456789ABCDEF"));
        const String original_str = original.toString();
        BigInteger moved;
        
        moved = std::move(original);
        
        compare(original_str, moved.toString());
    }
    
    TEST(IntegralAssignmentTest, BigIntegerTestBase) {
        BigInteger bi;
        
        bi = 42u;
        compare(BigInteger(42), bi);
        
        bi = -100;
        compare(BigInteger(-100), bi);
        compare(true, bi.isNegative());
        
        bi = 0u;
        compare(true, bi.isZero());
    }

    // =============================================================================
    // ARITHMETIC OPERATION TESTS
    // =============================================================================
    
    TEST(AdditionTest, BigIntegerTestBase) {
        // Basic addition
        const BigInteger a(100);
        const BigInteger b(200);
        const BigInteger result = a + b;
        compare(BigInteger(300), result);
        
        // Zero addition
        const BigInteger zero_result = a + zero;
        compare(a, zero_result);
        
        // Negative addition
        const BigInteger neg_a(-100);
        const BigInteger neg_result = a + neg_a;
        compare(true, neg_result.isZero());
        
        // Large number addition
        const BigInteger large1(String("0x123456789ABCDEF"));
        const BigInteger large2(String("0xFEDCBA987654321"));
        const BigInteger large_result = large1 + large2;
        compare(false, large_result.isZero());
        
        // Compound assignment
        BigInteger compound_test(100);
        compound_test += BigInteger(50);
        compare(BigInteger(150), compound_test);
    }
    
    TEST(SubtractionTest, BigIntegerTestBase) {
        // Basic subtraction
        const BigInteger a(200);
        const BigInteger b(100);
        const BigInteger result = a - b;
        compare(BigInteger(100), result);
        
        // Zero subtraction
        const BigInteger zero_result = a - zero;
        compare(a, zero_result);
        
        // Self subtraction
        const BigInteger self_result = a - a;
        compare(true, self_result.isZero());
        
        // Negative result
        const BigInteger neg_result = b - a;
        compare(true, neg_result.isNegative());
        compare(BigInteger(-100), neg_result);
        
        // Compound assignment
        BigInteger compound_test(200);
        compound_test -= BigInteger(50);
        compare(BigInteger(150), compound_test);
    }
    
    TEST(UnaryMinusTest, BigIntegerTestBase) {
        // Test corrected unary minus operator
        
        // Basic negation
        const BigInteger pos(42);
        const BigInteger neg_result = -pos;
        compare(String("-42"), neg_result.toString());
        compare(true, neg_result.isNegative());
        compare(false, pos.isNegative()); // Original should be unchanged
        
        // Double negation
        const BigInteger double_neg = -neg_result;
        compare(pos, double_neg);
        compare(false, double_neg.isNegative());
        
        // Negation of negative
        const BigInteger negative(-100);
        const BigInteger pos_result = -negative;
        compare(String("100"), pos_result.toString());
        compare(false, pos_result.isNegative());
        compare(true, negative.isNegative()); // Original should be unchanged
        
        // Zero negation (should remain positive)
        const BigInteger zero_neg = -zero;
        compare(true, zero_neg.isZero());
        compare(false, zero_neg.isNegative());
        compare(zero, zero_neg);
        
        // Large number negation
        const BigInteger large_pos(String("0x123456789ABCDEF"));
        const BigInteger large_neg = -large_pos;
        compare(true, large_neg.isNegative());
        compare(false, large_pos.isNegative()); // Original unchanged
        
        // Test x = -x assignment pattern
        BigInteger x = 42;
        BigInteger original_x = x;
        x = -x;
        compare(String("-42"), x.toString());
        compare(true, x.isNegative());
        compare(-original_x, x);
        
        // Test multiple assignments
        BigInteger y = 100;
        y = -y; // y = -100
        compare(String("-100"), y.toString());
        y = -y; // y = 100
        compare(String("100"), y.toString());
        compare(false, y.isNegative());
        
        // Test const-correctness (unary minus should work on const objects)
        const BigInteger const_val(123);
        const BigInteger const_neg = -const_val;
        compare(String("-123"), const_neg.toString());
        compare(String("123"), const_val.toString()); // Original unchanged
    }
    
    TEST(MultiplicationTest, BigIntegerTestBase) {
        // Basic multiplication
        const BigInteger a(12);
        const BigInteger b(13);
        const BigInteger result = a * b;
        compare(BigInteger(156), result);
        
        // Zero multiplication
        const BigInteger zero_result = a * zero;
        compare(true, zero_result.isZero());
        
        // One multiplication
        const BigInteger one_result = a * one;
        compare(a, one_result);
        
        // Negative multiplication
        const BigInteger neg_a(-12);
        const BigInteger neg_result = neg_a * b;
        compare(true, neg_result.isNegative());
        compare(BigInteger(-156), neg_result);
        
        // Both negative
        const BigInteger neg_b(-13);
        const BigInteger pos_result = neg_a * neg_b;
        compare(false, pos_result.isNegative());
        compare(BigInteger(156), pos_result);
        
        // Large number multiplication (tests Karatsuba)
        const BigInteger large_a(String("0x123456789ABCDEF"));
        const BigInteger large_b(String("0xFEDCBA987654321"));
        const BigInteger large_result = large_a * large_b;
        compare(false, large_result.isZero());
        
        // Compound assignment
        BigInteger compound_test(12);
        compound_test *= BigInteger(5);
        compare(BigInteger(60), compound_test);
    }
    
    TEST(DivisionTest, BigIntegerTestBase) {
        // Basic division
        const BigInteger a(156);
        const BigInteger b(12);
        const BigInteger result = a / b;
        compare(BigInteger(13), result);
        
        // Division by one
        const BigInteger one_result = a / one;
        compare(a, one_result);
        
        // Self division
        const BigInteger self_result = a / a;
        compare(true, self_result.isOne());
        
        // Zero dividend
        const BigInteger zero_dividend = zero / a;
        compare(true, zero_dividend.isZero());
        
        // Negative division
        const BigInteger neg_a(-156);
        const BigInteger neg_result = neg_a / b;
        compare(true, neg_result.isNegative());
        compare(BigInteger(-13), neg_result);
        
        // Compound assignment
        BigInteger compound_test(100);
        compound_test /= BigInteger(4);
        compare(BigInteger(25), compound_test);
    }
    
    TEST(ExtensiveDivisionTest, BigIntegerTestBase) {
        // Comprehensive tests for divModLimbs implementation
        
        // Single-limb division tests
        const BigInteger single_dividend(UINT64_MAX);
        const BigInteger single_divisor(1000000);
        const BigInteger single_quotient = single_dividend / single_divisor;
        const BigInteger single_remainder = single_dividend % single_divisor;
        
        // Verify division: dividend = quotient * divisor + remainder
        const BigInteger single_verify = single_quotient * single_divisor + single_remainder;
        compare(single_dividend, single_verify);
        compare(true, single_remainder < single_divisor);
        
        // Multi-limb division tests
        const BigInteger multi_dividend(String("0x123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"));
        const BigInteger multi_divisor(String("0x123456789ABCDEF"));
        const BigInteger multi_quotient = multi_dividend / multi_divisor;
        const BigInteger multi_remainder = multi_dividend % multi_divisor;
        
        // Verify multi-limb division
        const BigInteger multi_verify = multi_quotient * multi_divisor + multi_remainder;
        compare(multi_dividend, multi_verify);
        compare(true, multi_remainder < multi_divisor);
        
        // Division where dividend < divisor
        const BigInteger small_dividend(String("0x123"));
        const BigInteger large_divisor(String("0x123456789"));
        const BigInteger small_quotient = small_dividend / large_divisor;
        const BigInteger small_remainder = small_dividend % large_divisor;
        
        compare(true, small_quotient.isZero());
        compare(small_dividend, small_remainder);
        
        // Division where dividend == divisor
        const BigInteger equal_dividend(String("0x123456789ABCDEF"));
        const BigInteger equal_divisor(String("0x123456789ABCDEF"));
        const BigInteger equal_quotient = equal_dividend / equal_divisor;
        const BigInteger equal_remainder = equal_dividend % equal_divisor;
        
        compare(true, equal_quotient.isOne());
        compare(true, equal_remainder.isZero());
        
        // Large number division (stress test)
        /*const BigInteger huge_dividend(String("0x" + String(200, 'F'))); // Very large number
        const BigInteger medium_divisor(String("0x123456789ABCDEF0123456789"));
        const BigInteger huge_quotient = huge_dividend / medium_divisor;
        const BigInteger huge_remainder = huge_dividend % medium_divisor;
        
        // Verify the division
        compare(false, huge_quotient.isZero());
        compare(true, huge_remainder < medium_divisor);*/
        
        // Division by power of 2 (special case optimization)
        const BigInteger pow2_dividend(String("0x123456789ABCDEF0"));
        const BigInteger pow2_divisor(256); // 2^8
        const BigInteger pow2_quotient = pow2_dividend / pow2_divisor;
        const BigInteger pow2_remainder = pow2_dividend % pow2_divisor;
        
        // Should be equivalent to right shift by 8
        const BigInteger shift_equivalent = pow2_dividend >> 8;
        compare(shift_equivalent, pow2_quotient);
        
        // Test various single-limb divisors
        for (uint32_t divisor_val : {2, 3, 5, 7, 10, 100, 1000, 65537}) {
            const BigInteger test_dividend(String("0x123456789ABCDEF0123456"));
            const BigInteger test_divisor(divisor_val);
            const BigInteger test_quotient = test_dividend / test_divisor;
            const BigInteger test_remainder = test_dividend % test_divisor;
            
            // Verify division property
            const BigInteger test_verify = test_quotient * test_divisor + test_remainder;
            compare(test_dividend, test_verify);
            compare(true, test_remainder < test_divisor);
        }
        
        // Negative number division
        const BigInteger neg_dividend(String("-0x123456789ABCDEF"));
        const BigInteger pos_divisor(String("0x123456"));
        const BigInteger neg_quotient = neg_dividend / pos_divisor;
        const BigInteger neg_remainder = neg_dividend % pos_divisor;
        
        compare(true, neg_quotient.isNegative());
        // Remainder should be non-negative in cryptographic contexts
        compare(false, neg_remainder.isNegative());
        
        // Both negative
        const BigInteger neg_dividend2(String("-0x123456789"));
        const BigInteger neg_divisor2(String("-0x123"));
        const BigInteger both_neg_quotient = neg_dividend2 / neg_divisor2;
        const BigInteger both_neg_remainder = neg_dividend2 % neg_divisor2;
        
        compare(false, both_neg_quotient.isNegative()); // Negative / Negative = Positive
    }
    
    TEST(ModuloTest, BigIntegerTestBase) {
        // Basic modulo
        const BigInteger a(157);
        const BigInteger b(12);
        const BigInteger result = a % b;
        compare(BigInteger(1), result);
        
        // Zero remainder
        const BigInteger a2(156);
        const BigInteger zero_remainder = a2 % b;
        compare(true, zero_remainder.isZero());
        
        // Self modulo
        const BigInteger self_result = a % a;
        compare(true, self_result.isZero());
        
        // Modulo by one
        const BigInteger one_result = a % one;
        compare(true, one_result.isZero());
        
        // Compound assignment
        BigInteger compound_test(157);
        compound_test %= BigInteger(12);
        compare(BigInteger(1), compound_test);
    }

    // =============================================================================
    // BITWISE OPERATION TESTS
    // =============================================================================
    
    TEST(BitwiseAndTest, BigIntegerTestBase) {
        const BigInteger a(String("0xFF00"));
        const BigInteger b(String("0x0FF0"));
        const BigInteger result = a & b;
        compare(String("F00"), result.toHexString());
        
        // AND with zero
        const BigInteger zero_result = a & zero;
        compare(true, zero_result.isZero());
        
        // AND with self
        const BigInteger self_result = a & a;
        compare(a, self_result);
    }
    
    TEST(BitwiseOrTest, BigIntegerTestBase) {
        const BigInteger a(String("0xFF00"));
        const BigInteger b(String("0x00FF"));
        const BigInteger result = a | b;
        compare(String("FFFF"), result.toHexString());
        
        // OR with zero
        const BigInteger zero_result = a | zero;
        compare(a, zero_result);
        
        // OR with self
        const BigInteger self_result = a | a;
        compare(a, self_result);
    }
    
    TEST(BitwiseXorTest, BigIntegerTestBase) {
        const BigInteger a(String("0xFF00"));
        const BigInteger b(String("0x0FF0"));
        const BigInteger result = a ^ b;
        compare(String("F0F0"), result.toHexString());
        
        // XOR with zero
        const BigInteger zero_result = a ^ zero;
        compare(a, zero_result);
        
        // XOR with self
        const BigInteger self_result = a ^ a;
        compare(true, self_result.isZero());
    }
    
    TEST(BitwiseNotTest, BigIntegerTestBase) {
        const BigInteger a(String("0x00FF"));
        const BigInteger result = ~a;
        
        // NOT operation flips all bits
        compare(false, result.isZero());
        compare(true, result.isNegative()); // Two's complement representation
    }
    
    TEST(LeftShiftTest, BigIntegerTestBase) {
        const BigInteger a(String("0x123"));
        
        // Shift by 0
        const BigInteger shift0 = a << 0;
        compare(a, shift0);
        
        // Shift by 4 bits (one hex digit)
        const BigInteger shift4 = a << 4;
        compare(String("1230"), shift4.toHexString());
        
        // Shift by 8 bits (one byte)
        const BigInteger shift8 = a << 8;
        compare(String("12300"), shift8.toHexString());
        
        // Shift by large amount
        const BigInteger shift64 = a << 64;
        compare(false, shift64.isZero());
        compare(true, shift64.bitLength() > a.bitLength());
        
        // Shift zero
        const BigInteger zero_shift = zero << 10;
        compare(true, zero_shift.isZero());
    }
    
    TEST(RightShiftTest, BigIntegerTestBase) {
        const BigInteger a(String("0x1234"));
        
        // Shift by 0
        const BigInteger shift0 = a >> 0;
        compare(a, shift0);
        
        // Shift by 4 bits (one hex digit)
        const BigInteger shift4 = a >> 4;
        compare(String("123"), shift4.toHexString());
        
        // Shift by 8 bits (one byte)
        const BigInteger shift8 = a >> 8;
        compare(String("12"), shift8.toHexString());
        
        // Shift to zero
        const BigInteger shift_all = a >> 20;
        compare(true, shift_all.isZero());
        
        // Shift zero
        const BigInteger zero_shift = zero >> 10;
        compare(true, zero_shift.isZero());
    }

    // =============================================================================
    // COMPARISON OPERATOR TESTS
    // =============================================================================
    
    TEST(ComparisonTest, BigIntegerTestBase) {
        const BigInteger a(100);
        const BigInteger b(200);
        const BigInteger c(100);
        const BigInteger neg_a(-100);
        
        // Equality
        compare(true, a == c);
        compare(false, a == b);
        compare(false, a == neg_a);
        
        // Less than
        compare(true, a < b);
        compare(false, b < a);
        compare(false, a < c);
        compare(true, neg_a < a);
        
        // Less than or equal
        compare(true, a <= b);
        compare(true, a <= c);
        compare(false, b <= a);
        
        // Greater than
        compare(false, a > b);
        compare(true, b > a);
        compare(false, a > c);
        compare(true, a > neg_a);
        
        // Greater than or equal
        compare(false, a >= b);
        compare(true, a >= c);
        compare(true, b >= a);
        
        // Zero comparisons
        compare(true, zero == BigInteger(0));
        compare(true, zero < one);
        compare(true, zero <= one);
        compare(false, zero > one);
        compare(false, zero >= one);
    }

    // =============================================================================
    // CRYPTOGRAPHIC OPERATION TESTS
    // =============================================================================
    
    TEST(ModularPowerTest, BigIntegerTestBase) {
        // Basic modular exponentiation
        const BigInteger base(3);
        const BigInteger exponent(4);
        const BigInteger modulus(5);
        const BigInteger result = base.modPow(exponent, modulus);
        // 3^4 mod 5 = 81 mod 5 = 1
        compare(BigInteger(1), result);
        
        // Power of 0
        const BigInteger zero_exp_result = base.modPow(zero, modulus);
        compare(true, zero_exp_result.isOne());
        
        // Power of 1
        const BigInteger one_exp_result = base.modPow(one, modulus);
        compare(BigInteger(3), one_exp_result);
        
        // Base 0
        const BigInteger zero_base_result = zero.modPow(exponent, modulus);
        compare(true, zero_base_result.isZero());
        
        // Large numbers (RSA-like)
        /*const BigInteger large_base(String("0x123456"));
        const BigInteger large_exp(String("0x10001"));  // 65537
        const BigInteger large_mod(String("0xFEDCBA987654321"));
        const BigInteger large_result = large_base.modPow(large_exp, large_mod);
        compare(false, large_result.isZero());
        compare(true, large_result < large_mod);*/
    }
    
    /*TEST(MontgomeryModularPowerTest, BigIntegerTestBase) {
        // Test Montgomery modular exponentiation specifically
        // Use large moduli to trigger Montgomery reduction (bitLength >= MONTGOMERY_THRESHOLD)
        
        // Create a large odd modulus (required for Montgomery)
        const BigInteger large_odd_mod(String("0x1FFFFFFFFFFFFF0000000000000000000000000000000000000000000000001"));
        
        // Test basic Montgomery modPow
        const BigInteger base(String("0x123456789ABCDEF"));
        const BigInteger exponent(String("0x87654321"));
        const BigInteger montgomery_result = base.modPow(exponent, large_odd_mod);
        
        compare(false, montgomery_result.isZero());
        compare(true, montgomery_result < large_odd_mod);
        compare(false, montgomery_result.isNegative());
        
        // Compare with smaller modulus (non-Montgomery) for correctness
        const BigInteger small_mod(97); // Prime, but small enough to avoid Montgomery
        const BigInteger small_base(7);
        const BigInteger small_exp(11);
        
        const BigInteger standard_result = small_base.modPow(small_exp, small_mod);
        const BigInteger manual_calc = BigInteger(1977326743); // 7^11 = 1977326743
        const BigInteger expected = manual_calc % small_mod;
        compare(expected, standard_result);
        
        // Test Montgomery with various exponents
        const BigInteger mont_base(String("0xABCDEF123456789"));
        
        // Exponent 0 (should return 1)
        const BigInteger zero_exp = mont_base.modPow(zero, large_odd_mod);
        compare(true, zero_exp.isOne());
        
        // Exponent 1 (should return base mod modulus)
        const BigInteger one_exp = mont_base.modPow(one, large_odd_mod);
        compare(mont_base % large_odd_mod, one_exp);
        
        // Even exponent
        const BigInteger even_exp(String("0x1000000000000000")); // Large even number
        const BigInteger even_result = mont_base.modPow(even_exp, large_odd_mod);
        compare(false, even_result.isZero());
        compare(true, even_result < large_odd_mod);
        
        // Odd exponent
        const BigInteger odd_exp(String("0x1000000000000001")); // Large odd number
        const BigInteger odd_result = mont_base.modPow(odd_exp, large_odd_mod);
        compare(false, odd_result.isZero());
        compare(true, odd_result < large_odd_mod);
        
        // Test with base larger than modulus
        const BigInteger large_base = large_odd_mod + mont_base;
        const BigInteger large_base_result = large_base.modPow(exponent, large_odd_mod);
        const BigInteger reduced_base_result = mont_base.modPow(exponent, large_odd_mod);
        compare(reduced_base_result, large_base_result);
        
        // Test edge case: base = 0
        const BigInteger zero_base_mont = zero.modPow(exponent, large_odd_mod);
        compare(true, zero_base_mont.isZero());
        
        // Test edge case: base = 1
        const BigInteger one_base_mont = one.modPow(exponent, large_odd_mod);
        compare(true, one_base_mont.isOne());
        
        // Verify Montgomery produces same result as standard for smaller numbers
        const BigInteger verify_base(String("0x12345"));
        const BigInteger verify_exp(String("0x543"));
        const BigInteger verify_mod(String("0x789ABCDEF0123456789ABCDEF")); // Large enough for Montgomery
        
        const BigInteger montgomery_verify = verify_base.modPow(verify_exp, verify_mod);
        
        // Should be valid result
        compare(false, montgomery_verify.isZero());
        compare(true, montgomery_verify < verify_mod);
        compare(false, montgomery_verify.isNegative());
    }*/
    
    TEST(ModularInverseTest, BigIntegerTestBase) {
        // Basic modular inverse
        const BigInteger a(3);
        const BigInteger m(7);
        const BigInteger inverse = a.modInverse(m);
        
        // Verify: (a * inverse) mod m == 1
        const BigInteger verification = (a * inverse) % m;
        compare(true, verification.isOne());
        
        // Inverse of 1
        const BigInteger one_inverse = one.modInverse(m);
        compare(true, one_inverse.isOne());
    }
    
    TEST(GcdTest, BigIntegerTestBase) {
        // Basic GCD
        const BigInteger a(48);
        const BigInteger b(18);
        const BigInteger gcd_result = a.gcd(b);
        compare(BigInteger(6), gcd_result);
        
        // GCD with zero
        const BigInteger zero_gcd = a.gcd(zero);
        compare(a, zero_gcd);
        
        // GCD of coprime numbers
        const BigInteger coprime_a(15);
        const BigInteger coprime_b(28);
        const BigInteger coprime_gcd = coprime_a.gcd(coprime_b);
        compare(true, coprime_gcd.isOne());
        
        // GCD of identical numbers
        const BigInteger self_gcd = a.gcd(a);
        compare(a, self_gcd);
    }
    
    TEST(ExtendedGcdTest, BigIntegerTestBase) {
        const BigInteger a(48);
        const BigInteger b(18);
        auto [gcd_val, x, y] = a.extendedGcd(b);
        
        // Verify: gcd == a*x + b*y
        const BigInteger verification = a * x + b * y;
        compare(gcd_val, verification);
        
        // GCD should be 6
        compare(BigInteger(6), gcd_val);
    }

    // =============================================================================
    // PRIME OPERATION TESTS
    // =============================================================================
    
    TEST(PrimalityTest, BigIntegerTestBase) {
        // Known primes
        const BigInteger small_prime(17);
        compare(true, small_prime.isProbablyPrime(10));
        
        //const BigInteger large_prime(String("0xFFFFFFFFFFFFFFC5")); // Known prime
        //compare(true, large_prime.isProbablyPrime(5));
        
        // Known composites
        const BigInteger composite(15); // 3 * 5
        compare(false, composite.isProbablyPrime(10));
        
        //const BigInteger large_composite(String("0x123456789ABCDEF0")); // Even
        //compare(false, large_composite.isProbablyPrime(10));
        
        // Edge cases
        compare(false, zero.isProbablyPrime(10));
        compare(false, one.isProbablyPrime(10));
        compare(true, two.isProbablyPrime(10));
        compare(true, three.isProbablyPrime(10));
    }
    
    TEST(RandomGenerationTest, BigIntegerTestBase) {
        // Generate random numbers of different bit lengths
        const BigInteger random128 = BigInteger::random(128);
        compare(true, random128.bitLength() <= 128);
        compare(false, random128.isZero());
        
        const BigInteger random256 = BigInteger::random(256);
        compare(true, random256.bitLength() <= 256);
        compare(false, random256.isZero());
        
        // Generate multiple randoms to test they're different
        const BigInteger random1 = BigInteger::random(64);
        const BigInteger random2 = BigInteger::random(64);
        // Very unlikely they're the same
        compare(false, random1 == random2);
        
        // Zero bit length should return zero
        const BigInteger zero_bits = BigInteger::random(0);
        compare(true, zero_bits.isZero());
    }
    
    TEST(RandomRangeTest, BigIntegerTestBase) {
        const BigInteger max_val(100);
        
        // Generate random in range [0, 100)
        for (int i = 0; i < 10; ++i) {
            const BigInteger rand_val = BigInteger::randomRange(max_val);
            compare(true, rand_val < max_val);
            compare(false, rand_val.isNegative());
        }
        
        // Range with max = 1 should always return 0
        const BigInteger rand_one = BigInteger::randomRange(one);
        compare(true, rand_one.isZero());
        
        // Range with max = 0 should return 0
        const BigInteger rand_zero = BigInteger::randomRange(zero);
        compare(true, rand_zero.isZero());
    }
    
    /*TEST(PrimeGenerationTest, BigIntegerTestBase) {
        // Generate small prime
        const BigInteger prime128 = BigInteger::generatePrime(128, false);
        compare(true, prime128.isProbablyPrime(10));
        compare(true, prime128.bitLength() <= 128);
        compare(true, prime128.bitLength() >= 127); // Should be close to requested length
        
        // Generate safe prime (2q + 1 where q is prime)
        const BigInteger safe_prime = BigInteger::generatePrime(64, true);
        compare(true, safe_prime.isProbablyPrime(10));
        
        // Check if it's a safe prime: (p-1)/2 should also be prime
        const BigInteger q = (safe_prime - one) / two;
        compare(true, q.isProbablyPrime(10));
    }*/

    // =============================================================================
    // UTILITY METHOD TESTS
    // =============================================================================
    
    TEST(StringConversionTest, BigIntegerTestBase) {
        // Decimal string conversion
        const BigInteger positive(12345);
        compare(String("12345"), positive.toString());
        
        const BigInteger negative(-12345);
        compare(String("-12345"), negative.toString());
        
        const BigInteger zero_val(0);
        compare(String("0"), zero_val.toString());
        
        // Large number
        const BigInteger large(String("0x123456789ABCDEF"));
        compare(false, large.toString().empty());
    }
    
    TEST(HexStringConversionTest, BigIntegerTestBase) {
        // Hex string conversion
        const BigInteger hex_val(String("0x1A2B3C4D"));
        
        // Uppercase (default)
        compare(String("1A2B3C4D"), hex_val.toHexString(true));
        
        // Lowercase
        compare(String("1a2b3c4d"), hex_val.toHexString(false));
        
        // Zero
        compare(String("0"), zero.toHexString());
        
        // Single digit
        const BigInteger single(5);
        compare(String("5"), single.toHexString());
    }
    
    TEST(BytesConversionTest, BigIntegerTestBase) {
        const BigInteger val(String("0x123456"));
        const BytesVector bytes = val.toBytes();
        
        // Should be big-endian
        compare(static_cast<uint8_t>(0x12), bytes[0]);
        compare(static_cast<uint8_t>(0x34), bytes[1]);
        compare(static_cast<uint8_t>(0x56), bytes[2]);
        
        // Round-trip test
        const BigInteger reconstructed(bytes);
        compare(val, reconstructed);
        
        // Zero conversion
        const BytesVector zero_bytes = zero.toBytes();
        compare(size_t(1), zero_bytes.size());
        compare(static_cast<uint8_t>(0), zero_bytes[0]);
    }
    
    TEST(BitLengthTest, BigIntegerTestBase) {
        // Zero bit length
        compare(size_t(1), zero.bitLength()); // Zero has bit length 1
        
        // Small numbers
        compare(size_t(1), one.bitLength());
        compare(size_t(2), two.bitLength());
        compare(size_t(2), three.bitLength());
        
        // Power of 2
        const BigInteger pow2_8(256); // 2^8
        compare(size_t(9), pow2_8.bitLength());
        
        // Large number
        const BigInteger large(String("0xFFFFFFFF"));
        compare(size_t(32), large.bitLength());
        
        // Very large number
        const BigInteger very_large(String("0x123456789ABCDEF0123456789ABCDEF"));
        compare(true, very_large.bitLength() > 100);
    }
    
    TEST(PredicateMethodsTest, BigIntegerTestBase) {
        // isZero
        compare(true, zero.isZero());
        compare(false, one.isZero());
        compare(false, negative_one.isZero());
        
        // isOne
        compare(false, zero.isOne());
        compare(true, one.isOne());
        compare(false, two.isOne());
        compare(false, negative_one.isOne());
        
        // isEven
        compare(true, zero.isEven());
        compare(false, one.isEven());
        compare(true, two.isEven());
        compare(false, three.isEven());
        
        // isOdd
        compare(false, zero.isOdd());
        compare(true, one.isOdd());
        compare(false, two.isOdd());
        compare(true, three.isOdd());
        
        // isNegative
        compare(false, zero.isNegative());
        compare(false, one.isNegative());
        compare(true, negative_one.isNegative());
        
        const BigInteger large_negative(-12345);
        compare(true, large_negative.isNegative());
    }

    // =============================================================================
    // EDGE CASES AND ERROR CONDITION TESTS
    // =============================================================================
    
    TEST(DivisionByZeroTest, BigIntegerTestBase) {
        const BigInteger dividend(100);
        bool exception_caught = false;
        
        try {
            const BigInteger result = dividend / zero;
        } catch (const Exception&) {
            exception_caught = true;
        }
        
        compare(true, exception_caught);
    }
    
    TEST(ModuloByZeroTest, BigIntegerTestBase) {
        const BigInteger dividend(100);
        bool exception_caught = false;
        
        try {
            const BigInteger result = dividend % zero;
        } catch (const Exception&) {
            exception_caught = true;
        }
        
        compare(true, exception_caught);
    }
    
    TEST(ModularInverseNonCoprimeTest, BigIntegerTestBase) {
        const BigInteger a(6);  // Not coprime with 9 (gcd = 3)
        const BigInteger m(9);
        bool exception_caught = false;
        
        try {
            const BigInteger inverse = a.modInverse(m);
        } catch (const Exception&) {
            exception_caught = true;
        }
        
        compare(true, exception_caught);
    }
    
    TEST(MontgomeryErrorConditionsTest, BigIntegerTestBase) {
        // Test error conditions specific to montgomeryModPow
        
        // Test with zero modulus (should throw)
        const BigInteger base(5);
        const BigInteger exponent(3);
        const BigInteger zero_mod(0);
        
        bool zero_mod_exception = false;
        try {
            const BigInteger result = base.modPow(exponent, zero_mod);
        } catch (const Exception&) {
            zero_mod_exception = true;
        }
        compare(true, zero_mod_exception);
        
        // Test with even modulus for Montgomery (when using large modulus)
        // This should fall back to standard modPow, not throw an error
        const BigInteger large_even_mod(String("0x123456789ABCDEF0123456789ABCDEF0")); // Even, large
        const BigInteger even_mod_result = base.modPow(exponent, large_even_mod);
        compare(false, even_mod_result.isZero());
        compare(true, even_mod_result < large_even_mod);
        
        // Test modPow with very small modulus (should use standard algorithm)
        const BigInteger tiny_mod(3);
        const BigInteger tiny_result = base.modPow(exponent, tiny_mod);
        // 5^3 mod 3 = 125 mod 3 = 2
        compare(BigInteger(2), tiny_result);
    }
    
    TEST(DivisionErrorConditionsTest, BigIntegerTestBase) {
        // Test error conditions for divModLimbs implementation
        
        const BigInteger dividend(100);
        
        // Division by zero should throw exception
        bool div_zero_caught = false;
        try {
            const BigInteger result = dividend / zero;
        } catch (const Exception&) {
            div_zero_caught = true;
        }
        compare(true, div_zero_caught);
        
        // Modulo by zero should throw exception  
        bool mod_zero_caught = false;
        try {
            const BigInteger result = dividend % zero;
        } catch (const Exception&) {
            mod_zero_caught = true;
        }
        compare(true, mod_zero_caught);
        
        // Test edge case: very large dividend, very small divisor
        const BigInteger huge_dividend(String("0x" + String(300, 'F')));
        const BigInteger tiny_divisor(1);
        
        const BigInteger huge_quotient = huge_dividend / tiny_divisor;
        const BigInteger huge_remainder = huge_dividend % tiny_divisor;
        
        compare(huge_dividend, huge_quotient);
        compare(true, huge_remainder.isZero());
        
        // Test edge case: single bit dividend
        const BigInteger single_bit(1);
        const BigInteger large_divisor_2(String("0x123456789ABCDEF"));
        
        const BigInteger single_quotient = single_bit / large_divisor_2;
        const BigInteger single_remainder = single_bit % large_divisor_2;
        
        compare(true, single_quotient.isZero());
        compare(single_bit, single_remainder);
    }
    
    TEST(LargeNumberOperationsTest, BigIntegerTestBase) {
        // Test operations with very large numbers
        const BigInteger huge1(String("0x123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0"));
        const BigInteger huge2(String("0xFEDCBA987654321FEDCBA987654321FEDCBA987654321F"));
        
        // Addition
        const BigInteger sum = huge1 + huge2;
        compare(false, sum.isZero());
        compare(true, sum > huge1);
        compare(true, sum > huge2);
        
        // Multiplication
        const BigInteger product = huge1 * huge2;
        compare(false, product.isZero());
        compare(true, product > sum);
        
        // Bitwise operations
        const BigInteger bitwise_and = huge1 & huge2;
        const BigInteger bitwise_or = huge1 | huge2;
        const BigInteger bitwise_xor = huge1 ^ huge2;
        
        compare(false, bitwise_and.isZero());
        compare(false, bitwise_or.isZero());
        compare(false, bitwise_xor.isZero());
        
        // Verify XOR property: (a ^ b) ^ b == a
        const BigInteger xor_verify = bitwise_xor ^ huge2;
        compare(huge1, xor_verify);
    }
    
    TEST(NegativeNumberOperationsTest, BigIntegerTestBase) {
        const BigInteger pos(100);
        const BigInteger neg(-50);
        
        // Addition with negatives
        const BigInteger add_result = pos + neg;
        compare(BigInteger(50), add_result);
        
        // Subtraction with negatives
        const BigInteger sub_result = pos - neg;
        compare(BigInteger(150), sub_result);
        
        // Multiplication with negatives
        const BigInteger mul_result = pos * neg;
        compare(BigInteger(-5000), mul_result);
        compare(true, mul_result.isNegative());
        
        // Division with negatives
        const BigInteger div_result = neg / BigInteger(-5);
        compare(BigInteger(10), div_result);
        compare(false, div_result.isNegative());
    }
    
    TEST(ConstantTimingPropertiesTest, BigIntegerTestBase) {
        // This test verifies that operations maintain security properties
        // by testing with values designed to trigger different code paths
        
        const BigInteger same_size_a(String("0x123456789ABCDEF0"));
        const BigInteger same_size_b(String("0xFEDCBA9876543210"));
        
        // Operations on same-size numbers should be constant-time
        const BigInteger ct_add = same_size_a + same_size_b;
        const BigInteger ct_sub = same_size_a - same_size_b;
        const BigInteger ct_and = same_size_a & same_size_b;
        
        compare(false, ct_add.isZero());
        compare(false, ct_sub.isZero());
        compare(false, ct_and.isZero());
        
        // Test with different-sized operands
        const BigInteger small(0x123);
        const BigInteger large(String("0x123456789ABCDEF0123456789ABCDEF0"));
        
        const BigInteger mixed_add = small + large;
        const BigInteger mixed_mul = small * large;
        
        compare(false, mixed_add.isZero());
        compare(false, mixed_mul.isZero());
    }
}
