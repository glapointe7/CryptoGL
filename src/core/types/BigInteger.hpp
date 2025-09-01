#pragma once

#include "../types/Types.hpp"
#include "../types/Vector.hpp"
#include "../types/String.hpp"
#include "../types/Array.hpp"
#include "../exceptions/Exception.hpp"
#include <random>
#include <compare>
#include <concepts>
#include <cstdint>

// Platform-specific includes for intrinsics
#if defined(_MSC_VER) && defined(_WIN64)
#include <intrin.h>
#endif

namespace CryptoGL
{

    /**
     * @brief High-performance cryptographic big integer implementation
     *
     * Provides secure, constant-time operations for cryptographic applications
     * including RSA, DSA, ECC, and other public-key cryptosystems.
     *
     * Features:
     * - Constant-time operations to prevent timing attacks
     * - Montgomery arithmetic for efficient modular operations
     * - Karatsuba multiplication for O(n^1.585) complexity
     * - Secure memory management with automatic clearing
     * - Integration with CryptoGL's Vector and String types
     * - Thread-safe operations
     *
     * @note All operations are designed to be side-channel resistant
     */
    class BigInteger
    {
    public:
        // Type definitions for clarity
        using Limb = uint64_t;
        using LimbVector = Vector<Limb>;

        // Platform-specific 128-bit type handling
#if defined(__GNUC__) || defined(__clang__)
        using DoubleLimb = __uint128_t;
#define HAS_NATIVE_UINT128
#elif defined(_MSC_VER) && defined(_WIN64)
        // MSVC doesn't have __uint128_t, we'll use manual arithmetic
        struct DoubleLimb
        {
            uint64_t low, high;
            DoubleLimb() : low(0), high(0) {}
            DoubleLimb(uint64_t l, uint64_t h) : low(l), high(h) {}
            explicit DoubleLimb(uint64_t val) : low(val), high(0) {}
        };
#define HAS_MANUAL_UINT128
#else
        // Fallback for other compilers
        struct DoubleLimb
        {
            uint64_t low, high;
            DoubleLimb() : low(0), high(0) {}
            DoubleLimb(uint64_t l, uint64_t h) : low(l), high(h) {}
            explicit DoubleLimb(uint64_t val) : low(val), high(0) {}
        };
#define HAS_MANUAL_UINT128
#endif

        static constexpr size_t LIMB_BITS = 64;
        static constexpr size_t KARATSUBA_THRESHOLD = 32;
        static constexpr size_t MONTGOMERY_THRESHOLD = 16;

        // Platform-specific arithmetic helper functions
        static inline DoubleLimb multiply64x64(uint64_t a, uint64_t b) noexcept
        {
#ifdef HAS_NATIVE_UINT128
            DoubleLimb result = static_cast<DoubleLimb>(a) * b;
            return result;
#elif defined(_MSC_VER) && defined(_WIN64)
            uint64_t high;
            uint64_t low = _umul128(a, b, &high);
            return DoubleLimb(low, high);
#else
            // Manual 64x64 -> 128 multiplication using 32-bit chunks
            uint64_t a_lo = a & 0xFFFFFFFFULL;
            uint64_t a_hi = a >> 32;
            uint64_t b_lo = b & 0xFFFFFFFFULL;
            uint64_t b_hi = b >> 32;

            uint64_t p0 = a_lo * b_lo;
            uint64_t p1 = a_lo * b_hi;
            uint64_t p2 = a_hi * b_lo;
            uint64_t p3 = a_hi * b_hi;

            uint64_t carry = ((p0 >> 32) + (p1 & 0xFFFFFFFFULL) + (p2 & 0xFFFFFFFFULL)) >> 32;
            uint64_t low = p0 + ((p1 + p2) << 32);
            uint64_t high = p3 + (p1 >> 32) + (p2 >> 32) + carry;

            return DoubleLimb(low, high);
#endif
        }

        static inline DoubleLimb add128(const DoubleLimb &a, const DoubleLimb &b) noexcept
        {
#ifdef HAS_NATIVE_UINT128
            return a + b;
#else
            uint64_t low = a.low + b.low;
            uint64_t carry = (low < a.low) ? 1 : 0;
            uint64_t high = a.high + b.high + carry;
            return DoubleLimb(low, high);
#endif
        }

        static inline DoubleLimb add128(const DoubleLimb &a, uint64_t b) noexcept
        {
#ifdef HAS_NATIVE_UINT128
            return a + b;
#else
            uint64_t low = a.low + b;
            uint64_t carry = (low < a.low) ? 1 : 0;
            uint64_t high = a.high + carry;
            return DoubleLimb(low, high);
#endif
        }

        static inline uint64_t extractLow(const DoubleLimb &val) noexcept
        {
#ifdef HAS_NATIVE_UINT128
            return static_cast<uint64_t>(val);
#else
            return val.low;
#endif
        }

        static inline uint64_t extractHigh(const DoubleLimb &val) noexcept
        {
#ifdef HAS_NATIVE_UINT128
            return static_cast<uint64_t>(val >> LIMB_BITS);
#else
            return val.high;
#endif
        }

    private:
        LimbVector limbs; // Little-endian limb storage
        bool negative;    // Sign flag

        // Security: Secure random number generator
        static thread_local std::random_device secure_rng;
        static thread_local std::mt19937_64 fast_rng;

    public:
        // =============================================================================
        // CONSTRUCTORS AND DESTRUCTORS
        // =============================================================================

        /**
         * @brief Default constructor - creates zero
         */
        BigInteger() : limbs(1, 0), negative(false) {}

        /**
         * @brief Non-explicit constructors for common integer types
         * @note Allows natural syntax: BigInteger x = 0
         */
        // BigInteger(int value) : BigInteger(static_cast<int64_t>(value)) {}
        // BigInteger(long value) : BigInteger(static_cast<int64_t>(value)) {}
        // BigInteger(long long value) : BigInteger(static_cast<int64_t>(value)) {}
        // BigInteger(unsigned int value) : BigInteger(static_cast<uint64_t>(value)) {}
        // BigInteger(unsigned long value) : BigInteger(static_cast<uint64_t>(value)) {}
        // BigInteger(unsigned long long value) : BigInteger(static_cast<uint64_t>(value)) {}

        /**
         * @brief Core constructors for 64-bit signed and unsigned integers
         */
        // BigInteger(int64_t value) 
        // {
        //     if (value == 0)
        //     {
        //         negative = false;
        //         limbs = LimbVector(1, 0);
        //     }
        //     else if (value == INT64_MIN)
        //     {
        //         // Special case: INT64_MIN can't be negated in int64_t
        //         negative = true;
        //         limbs = LimbVector(1, static_cast<uint64_t>(INT64_MAX) + 1);
        //     }
        //     else
        //     {
        //         negative = (value < 0);
        //         limbs = LimbVector(1, value < 0 ? static_cast<uint64_t>(-value) : static_cast<uint64_t>(value));
        //     }
        // }
        
        // BigInteger(uint64_t value) : negative(false), limbs(1, value) 
        // {
        //     if (value == 0)
        //     {
        //         negative = false; // Ensure zero is always positive
        //     }
        // }

        /**
         * @brief Construct from other integer types (explicit)
         * @tparam T Integer type (excluding common types above)
         * @param value Initial value
         * @note Explicit to prevent unintended conversions from unusual integer types
         */
        template <typename T>
        BigInteger(T value) requires std::integral<T>
        {
            if constexpr (std::is_signed_v<T>)
            {
                negative = (value < 0);
                // if (negative) {
                //     value = -value;
                // }
            }
            else
            {
                negative = false;
            }

            if (value == 0)
            {
                limbs = LimbVector(1, 0);
                return;
            }

            constexpr unsigned width = std::numeric_limits<T>::digits;
            if constexpr (LIMB_BITS >= width)
            {
                limbs = LimbVector(1, value);
                return;
            }
            else
            {
                limbs.clear();
                while (value > 0)
                {
                    limbs.push_back(static_cast<Limb>(value & UINT64_MAX));
                    value >>= LIMB_BITS;
                }
            }
        }

        /**
         * @brief Construct from hex string
         * @param hex_string Hexadecimal representation (with or without "0x")
         */
        explicit BigInteger(const String &hex_string);

        /**
         * @brief Construct from byte array (big-endian)
         * @param bytes Byte vector in big-endian format
         */
        explicit BigInteger(const BytesVector &bytes);

        /**
         * @brief Construct from limb vector (internal use)
         * @param limb_vector Vector of limbs in little-endian format
         * @note Used internally for efficient construction from computed results
         */
        explicit BigInteger(const LimbVector &limb_vector);

        /**
         * @brief Copy constructor with secure copying
         */
        BigInteger(const BigInteger &other);

        /**
         * @brief Move constructor
         */
        BigInteger(BigInteger &&other) noexcept;

        /**
         * @brief Secure destructor with memory clearing
         */
        ~BigInteger();

        [[nodiscard]] LimbVector getLimbs() const noexcept { return limbs; }

        // =============================================================================
        // ASSIGNMENT OPERATORS
        // =============================================================================

        BigInteger &operator=(const BigInteger &other);
        BigInteger &operator=(BigInteger &&other) noexcept;

        /**
         * @brief Template assignment from integral types
         * @tparam T Integral type
         * @param value Value to assign
         * @return Reference to this object
         */
        template <typename T>
        BigInteger &operator=(T value)
            requires std::integral<T>
        {
            *this = BigInteger(value);
            return *this;
        }

        // =============================================================================
        // ARITHMETIC OPERATIONS
        // =============================================================================

        /**
         * @brief Addition operator
         * @param rhs Right-hand side operand
         * @return Sum of this and rhs
         * @note Constant-time operation for same-size operands
         */
        BigInteger operator+(const BigInteger &rhs) const;

        /**
         * @brief Subtraction operator
         * @param rhs Right-hand side operand
         * @return Difference of this and rhs
         * @note Constant-time operation for same-size operands
         */
        BigInteger operator-(const BigInteger &rhs) const;

        /**
         * @brief Unary subtraction operator (negation)
         * @return -this (new BigInteger with opposite sign)
         * @note Does not modify the original object
         */
        BigInteger operator-() const;

        /**
         * @brief Multiplication operator (Karatsuba algorithm)
         * @param rhs Right-hand side operand
         * @return Product of this and rhs
         * @note Uses Karatsuba for large numbers (O(n^1.585))
         */
        BigInteger operator*(const BigInteger &rhs) const;

        /**
         * @brief Division operator
         * @param rhs Divisor
         * @return Quotient of this divided by rhs
         * @throws Exception if rhs is zero
         */
        BigInteger operator/(const BigInteger &rhs) const;

        /**
         * @brief Modulo operator
         * @param rhs Modulus
         * @return Remainder of this divided by rhs
         * @throws Exception if rhs is zero
         */
        BigInteger operator%(const BigInteger &rhs) const;

        /**
         * @brief Get absolute value
         * @return Absolute value of this number
         */
        [[nodiscard]] BigInteger abs() const noexcept;

        // Compound assignment operators
        BigInteger &operator+=(const BigInteger &rhs);
        BigInteger &operator-=(const BigInteger &rhs);
        BigInteger &operator*=(const BigInteger &rhs);
        BigInteger &operator/=(const BigInteger &rhs);
        BigInteger &operator%=(const BigInteger &rhs);

        // =============================================================================
        // BITWISE OPERATIONS
        // =============================================================================

        BigInteger operator&(const BigInteger &rhs) const;
        BigInteger operator|(const BigInteger &rhs) const;
        BigInteger operator^(const BigInteger &rhs) const;
        BigInteger operator~() const;
        BigInteger operator<<(size_t shift) const;
        BigInteger operator>>(size_t shift) const;

        // =============================================================================
        // COMPARISON OPERATORS (C++20 spaceship operator)
        // =============================================================================

        std::strong_ordering operator<=>(const BigInteger &rhs) const noexcept;
        bool operator==(const BigInteger &rhs) const noexcept;

        // =============================================================================
        // CRYPTOGRAPHIC OPERATIONS
        // =============================================================================

        /**
         * @brief Fast modular exponentiation using binary method
         * @param exponent The exponent
         * @param modulus The modulus
         * @return (this^exponent) mod modulus
         * @note Uses Montgomery reduction for large moduli
         * @note Constant-time implementation to prevent timing attacks
         */
        [[nodiscard]] BigInteger modPow(const BigInteger &exponent,
                                        const BigInteger &modulus) const;

        /**
         * @brief Modular inverse using extended Euclidean algorithm
         * @param modulus The modulus
         * @return Modular inverse of this modulo modulus
         * @throws Exception if gcd(this, modulus) != 1
         */
        [[nodiscard]] BigInteger modInverse(const BigInteger &modulus) const;

        /**
         * @brief Greatest Common Divisor using binary GCD
         * @param other The other number
         * @return gcd(this, other)
         * @note Constant-time implementation
         */
        [[nodiscard]] BigInteger gcd(const BigInteger &other) const;

        /**
         * @brief Extended Euclidean algorithm
         * @param other The other number
         * @return Tuple of (gcd, x, y) where gcd = this*x + other*y
         */
        [[nodiscard]] std::tuple<BigInteger, BigInteger, BigInteger>
        extendedGcd(const BigInteger &other) const;

        // =============================================================================
        // PRIME GENERATION AND TESTING
        // =============================================================================

        /**
         * @brief Miller-Rabin probabilistic primality test
         * @param rounds Number of test rounds (higher = more accurate)
         * @return true if probably prime, false if definitely composite
         * @note Security: Uses constant-time operations
         */
        [[nodiscard]] bool isProbablyPrime(int rounds = 40) const;

        /**
         * @brief Generate cryptographically secure random prime
         * @param bit_length Length in bits of the prime
         * @param safe_prime If true, generates safe prime (p = 2q + 1 where q is prime)
         * @return Random prime of specified bit length
         */
        [[nodiscard]] static BigInteger generatePrime(size_t bit_length,
                                                      bool safe_prime = false);

        /**
         * @brief Generate cryptographically secure random number
         * @param bit_length Length in bits
         * @return Random number of specified bit length
         */
        [[nodiscard]] static BigInteger random(size_t bit_length);

        /**
         * @brief Generate random number in range [0, max)
         * @param max Upper bound (exclusive)
         * @return Random number in specified range
         */
        [[nodiscard]] static BigInteger randomRange(const BigInteger &max);

        // =============================================================================
        // UTILITY METHODS
        // =============================================================================

        /**
         * @brief Convert to hexadecimal string
         * @param uppercase Use uppercase hex digits
         * @return Hexadecimal representation
         */
        [[nodiscard]] String toHexString(bool uppercase = true) const;

        /**
         * @brief Convert to decimal string
         * @return Decimal representation
         */
        [[nodiscard]] String toString() const;

        [[nodiscard]] static String toString(const Vector<BigInteger> &V);

        [[nodiscard]] static Vector<BigInteger> stringToBigIntVector(const String &str);

        /**
         * @brief Convert to byte array (big-endian)
         * @return Byte representation in big-endian format
         */
        [[nodiscard]] BytesVector toBytes() const;

        /**
         * @brief Get bit length of the number
         * @return Number of bits needed to represent this number
         */
        [[nodiscard]] size_t bitLength() const noexcept;

        /**
         * @brief Check if number is zero
         * @return true if zero
         */
        [[nodiscard]] bool isZero() const noexcept;

        /**
         * @brief Check if number is one
         * @return true if one
         */
        [[nodiscard]] bool isOne() const noexcept;

        /**
         * @brief Check if number is even
         * @return true if even
         */
        [[nodiscard]] bool isEven() const noexcept;

        /**
         * @brief Check if number is odd
         * @return true if odd
         */
        [[nodiscard]] bool isOdd() const noexcept;

        /**
         * @brief Check if number is negative
         * @return true if negative
         */
        [[nodiscard]] bool isNegative() const noexcept { return negative; }

        /**
         * @brief Test if a specific bit is set
         * @param bit_position Position of bit to test (0 = least significant bit)
         * @return true if bit at position is set (1), false if clear (0)
         * @note Constant-time operation for cryptographic security
         */
        [[nodiscard]] bool testBit(size_t bit_position) const noexcept;

    private:
        // =============================================================================
        // INTERNAL HELPER METHODS
        // =============================================================================

        void normalize();
        void secureErase();

        // Low-level arithmetic
        static LimbVector addLimbs(const LimbVector &a, const LimbVector &b);
        static LimbVector subLimbs(const LimbVector &a, const LimbVector &b);
        static LimbVector mulLimbs(const LimbVector &a, const LimbVector &b);
        static LimbVector karatsubaMultiply(const LimbVector &a, const LimbVector &b);

        // Montgomery arithmetic for modular operations
        static BigInteger montgomeryModPow(const BigInteger &base,
                                           const BigInteger &exp,
                                           const BigInteger &mod);

        // Division algorithm
        static std::pair<LimbVector, LimbVector> divModLimbs(const LimbVector &dividend,
                                                             const LimbVector &divisor);

        // Comparison
        static int compareLimbs(const LimbVector &a, const LimbVector &b) noexcept;

        // Prime generation helpers
        static bool quickPrimalityTest(const BigInteger &candidate) noexcept;
    };

}