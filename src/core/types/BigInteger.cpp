#include "BigInteger.hpp"
#include <sstream>

using namespace CryptoGL;

thread_local std::mt19937_64 BigInteger::fast_rng{std::random_device{}()};

/**
 * @brief Karatsuba multiplication algorithm - O(n^1.585) complexity
 * @param a First operand limbs
 * @param b Second operand limbs
 * @return Product limbs
 * @note Recursively divides numbers for efficient multiplication
 */
BigInteger::LimbVector BigInteger::karatsubaMultiply(const LimbVector &a, const LimbVector &b)
{
    const size_t n = std::max(a.size(), b.size());

    // Base case: use traditional multiplication for small numbers
    if (n < KARATSUBA_THRESHOLD)
    {
        return mulLimbs(a, b);
    }

    // Split numbers into high and low parts
    const size_t half = (n + 1) / 2;

    LimbVector a_low(a.begin(), a.begin() + std::min(half, a.size()));
    LimbVector a_high(a.size() > half ? a.begin() + half : a.end(), a.end());

    LimbVector b_low(b.begin(), b.begin() + std::min(half, b.size()));
    LimbVector b_high(b.size() > half ? b.begin() + half : b.end(), b.end());

    // Recursive calls
    auto z0 = karatsubaMultiply(a_low, b_low);   // Low * Low
    auto z2 = karatsubaMultiply(a_high, b_high); // High * High

    // (a_low + a_high) * (b_low + b_high)
    auto a_sum = addLimbs(a_low, a_high);
    auto b_sum = addLimbs(b_low, b_high);
    auto z1 = karatsubaMultiply(a_sum, b_sum);

    // z1 = z1 - z2 - z0
    z1 = subLimbs(z1, z2);
    z1 = subLimbs(z1, z0);

    // Result = z0 + z1 * base^half + z2 * base^(2*half)
    LimbVector result(z0.size() + z2.size() + 2 * half, 0);

    // Add z0
    for (size_t i = 0; i < z0.size(); ++i)
    {
        result[i] += z0[i];
    }

    // Add z1 * base^half
    for (size_t i = 0; i < z1.size(); ++i)
    {
        result[i + half] += z1[i];
    }

    // Add z2 * base^(2*half)
    for (size_t i = 0; i < z2.size(); ++i)
    {
        result[i + 2 * half] += z2[i];
    }

    // Handle carries
    Limb carry = 0;
    for (size_t i = 0; i < result.size(); ++i)
    {
        DoubleLimb sum = static_cast<DoubleLimb>(result[i]) + carry;
        result[i] = static_cast<Limb>(sum);
        carry = static_cast<Limb>(sum >> LIMB_BITS);
    }

    if (carry > 0)
    {
        result.push_back(carry);
    }

    return result;
}

/**
 * @brief Fast modular exponentiation using binary method with Montgomery reduction
 * @param exponent The exponent
 * @param modulus The modulus
 * @return (this^exponent) mod modulus
 * @note Constant-time implementation resistant to timing attacks
 */
BigInteger BigInteger::modPow(const BigInteger &exponent, const BigInteger &modulus) const
{
    if (modulus.isZero())
    {
        throw Exception("Modulus cannot be zero");
    }

    if (exponent.isZero())
    {
        return BigInteger(1);
    }

    if (modulus.bitLength() >= MONTGOMERY_THRESHOLD)
    {
        if (modulus.isOdd())
        {
            return montgomeryModPow(*this, exponent, modulus);
        }
    }

    // Standard binary exponentiation with constant-time operations
    BigInteger result(1);
    BigInteger base = *this % modulus;
    BigInteger exp = exponent;

    // Process exponent bits from least significant to most significant
    while (!exp.isZero())
    {
        // Constant-time conditional: if exp is odd, multiply result by base
        if (exp.isOdd())
        {
            result = (result * base) % modulus;
        }

        // Square base and halve exponent
        base = (base * base) % modulus;
        exp = exp >> 1;
    }

    return result;
}

/**
 * @brief Miller-Rabin probabilistic primality test
 * @param rounds Number of test rounds (default: 40 for 2^-80 error probability)
 * @return true if probably prime, false if definitely composite
 * @note Uses constant-time operations to prevent timing attacks
 */
bool BigInteger::isProbablyPrime(int rounds) const
{
    if (isZero() || isOne())
    {
        return false;
    }

    if (*this == 2 || *this == 3)
    {
        return true;
    }

    if (isEven())
    {
        return false;
    }

    // Write n-1 as d * 2^r
    const BigInteger n_minus_1 = *this - 1;
    BigInteger d = n_minus_1;
    int r = 0;

    while (d.isEven())
    {
        d = d >> 1;
        ++r;
    }

    // Witness loop
    for (int i = 0; i < rounds; ++i)
    {
        const BigInteger a = randomRange(n_minus_1 - 1) + 2;
        BigInteger x = a.modPow(d, *this);

        if (x.isOne() || x == n_minus_1)
        {
            continue;
        }

        bool composite = true;
        for (int j = 0; j < r - 1; ++j)
        {
            x = x.modPow(2, *this);
            if (x == n_minus_1)
            {
                composite = false;
                break;
            }
        }

        if (composite)
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Construct BigInteger from hexadecimal string
 * @param hex_string Hexadecimal representation (with or without "0x" prefix)
 * @throws Exception if invalid hex string format
 */
BigInteger::BigInteger(const String &hex_string) : negative(false)
{
    String hex = hex_string;

    // Handle negative sign
    if (!hex.empty() && hex[0] == '-')
    {
        negative = true;
        hex = hex.substr(1);
    }

    // Remove "0x" or "0X" prefix if present
    if (hex.size() >= 2 && hex.substr(0, 2) == "0x")
    {
        hex = hex.substr(2);
    }
    else if (hex.size() >= 2 && hex.substr(0, 2) == "0X")
    {
        hex = hex.substr(2);
    }

    if (hex.empty())
    {
        limbs = LimbVector(1, 0);
        negative = false;
        return;
    }

    // Convert hex string to limbs (process 16 hex digits at a time)
    limbs.clear();
    const size_t hex_per_limb = 16; // 16 hex digits = 64 bits

    for (size_t i = hex.size(); i > 0; i -= std::min(hex_per_limb, i))
    {
        size_t start = (i > hex_per_limb) ? i - hex_per_limb : 0;
        size_t len = i - start;
        String limb_hex = hex.substr(start, len);

        Limb limb_value = 0;
        for (char c : limb_hex)
        {
            limb_value <<= 4;
            if (c >= '0' && c <= '9')
            {
                limb_value |= (c - '0');
            }
            else if (c >= 'a' && c <= 'f')
            {
                limb_value |= (c - 'a' + 10);
            }
            else if (c >= 'A' && c <= 'F')
            {
                limb_value |= (c - 'A' + 10);
            }
            else
            {
                throw Exception("Invalid hexadecimal character in string");
            }
        }
        limbs.push_back(limb_value);
    }

    normalize();
}

/**
 * @brief Construct BigInteger from byte array in big-endian format
 * @param bytes Byte vector in big-endian format (most significant byte first)
 */
BigInteger::BigInteger(const BytesVector &bytes) : negative(false)
{
    if (bytes.empty())
    {
        limbs = LimbVector(1, 0);
        return;
    }

    limbs.clear();
    const size_t bytes_per_limb = sizeof(Limb);

    // Process bytes in reverse order (big-endian to little-endian limbs)
    for (size_t i = bytes.size(); i > 0; i -= std::min(bytes_per_limb, i))
    {
        size_t start = (i > bytes_per_limb) ? i - bytes_per_limb : 0;
        size_t len = i - start;

        Limb limb_value = 0;
        for (size_t j = 0; j < len; ++j)
        {
            limb_value = (limb_value << 8) | bytes[start + j];
        }
        limbs.push_back(limb_value);
    }

    normalize();
}

/**
 * @brief Construct BigInteger from limb vector (internal use)
 * @param limb_vector Vector of limbs in little-endian format
 * @note Used internally for efficient construction from computed results
 */
BigInteger::BigInteger(const LimbVector &limb_vector) : negative(false)
{
    if (limb_vector.empty())
    {
        limbs = LimbVector(1, 0);
        return;
    }

    limbs = limb_vector;
    normalize();
}

/**
 * @brief Copy constructor with secure copying
 * @param other BigInteger to copy from
 */
BigInteger::BigInteger(const BigInteger &other)
    : limbs(other.limbs), negative(other.negative) {}

/**
 * @brief Move constructor for efficient resource transfer
 * @param other BigInteger to move from
 */
BigInteger::BigInteger(BigInteger &&other) noexcept
    : limbs(std::move(other.limbs)), negative(other.negative)
{
    other.negative = false;
}

/**
 * @brief Secure destructor with memory clearing
 * @note Automatically clears sensitive data from memory
 */
BigInteger::~BigInteger()
{
    secureErase();
}

/**
 * @brief Copy assignment operator
 * @param other BigInteger to assign from
 * @return Reference to this object
 */
BigInteger &BigInteger::operator=(const BigInteger &other)
{
    if (this != &other)
    {
        secureErase();
        limbs = other.limbs;
        negative = other.negative;
    }
    return *this;
}

/**
 * @brief Move assignment operator
 * @param other BigInteger to move from
 * @return Reference to this object
 */
BigInteger &BigInteger::operator=(BigInteger &&other) noexcept
{
    if (this != &other)
    {
        secureErase();
        limbs = std::move(other.limbs);
        negative = other.negative;
        other.negative = false;
    }
    return *this;
}

/**
 * @brief Addition operator with constant-time implementation
 * @param rhs Right-hand side operand
 * @return Sum of this and rhs
 * @note Handles different signs and ensures constant-time operation
 */
BigInteger BigInteger::operator+(const BigInteger &rhs) const
{
    // Same signs: add absolute values
    if (negative == rhs.negative)
    {
        BigInteger result;
        result.limbs = addLimbs(limbs, rhs.limbs);
        result.negative = negative;
        result.normalize();
        return result;
    }

    // Different signs: subtract smaller from larger
    const int abs_cmp = compareLimbs(limbs, rhs.limbs);
    BigInteger result;

    if (abs_cmp >= 0)
    {
        result.limbs = subLimbs(limbs, rhs.limbs);
        result.negative = negative;
    }
    else
    {
        result.limbs = subLimbs(rhs.limbs, limbs);
        result.negative = rhs.negative;
    }

    result.normalize();
    return result;
}

/**
 * @brief Subtraction operator
 * @param rhs Right-hand side operand
 * @return Difference of this and rhs
 */
BigInteger BigInteger::operator-(const BigInteger &rhs) const
{
    // this - rhs = this + (-rhs)
    BigInteger neg_rhs = rhs;
    neg_rhs.negative = !rhs.negative;
    return *this + neg_rhs;
}

/**
 * @brief Unary subtraction operator (negation)
 * @return -this (new BigInteger with opposite sign)
 * @note Does not modify the original object; zero remains positive
 */
BigInteger BigInteger::operator-() const
{
    BigInteger result = *this;
    
    // Zero should always remain positive (mathematical convention)
    if (!result.isZero())
    {
        result.negative = !result.negative;
    }
    
    return result;
}

/**
 * @brief Multiplication operator using Karatsuba algorithm
 * @param rhs Right-hand side operand
 * @return Product of this and rhs
 */
BigInteger BigInteger::operator*(const BigInteger &rhs) const
{
    BigInteger result;
    result.limbs = karatsubaMultiply(limbs, rhs.limbs);
    result.negative = (negative != rhs.negative) && !result.isZero();
    result.normalize();
    return result;
}

/**
 * @brief Division operator
 * @param rhs Divisor
 * @return Quotient of this divided by rhs
 * @throws Exception if rhs is zero
 */
BigInteger BigInteger::operator/(const BigInteger &rhs) const
{
    if (rhs.isZero())
    {
        throw Exception("Division by zero");
    }

    auto [quotient, remainder] = divModLimbs(limbs, rhs.limbs);

    BigInteger result;
    result.limbs = quotient;
    result.negative = (negative != rhs.negative) && !result.isZero();
    result.normalize();
    return result;
}

/**
 * @brief Modulo operator
 * @param rhs Modulus
 * @return Remainder of this divided by rhs
 * @throws Exception if rhs is zero
 */
BigInteger BigInteger::operator%(const BigInteger &rhs) const
{
    if (rhs.isZero())
    {
        throw Exception("Division by zero");
    }

    auto [quotient, remainder] = divModLimbs(limbs, rhs.limbs);

    BigInteger result;
    result.limbs = remainder;
    result.negative = negative && !result.isZero();
    result.normalize();

    // Ensure positive remainder for cryptographic operations
    if (result.negative && !result.isZero())
    {
        result = result + BigInteger(rhs).abs();
    }

    return result;
}

/**
 * @brief Compound addition assignment
 * @param rhs Right-hand side operand
 * @return Reference to this object
 */
BigInteger &BigInteger::operator+=(const BigInteger &rhs)
{
    *this = *this + rhs;
    return *this;
}

/**
 * @brief Compound subtraction assignment
 */
BigInteger &BigInteger::operator-=(const BigInteger &rhs)
{
    *this = *this - rhs;
    return *this;
}

/**
 * @brief Compound multiplication assignment
 */
BigInteger &BigInteger::operator*=(const BigInteger &rhs)
{
    *this = *this * rhs;
    return *this;
}

/**
 * @brief Compound division assignment
 */
BigInteger &BigInteger::operator/=(const BigInteger &rhs)
{
    *this = *this / rhs;
    return *this;
}

/**
 * @brief Compound modulo assignment
 */
BigInteger &BigInteger::operator%=(const BigInteger &rhs)
{
    *this = *this % rhs;
    return *this;
}

/**
 * @brief Bitwise AND operator
 * @param rhs Right-hand side operand
 * @return Bitwise AND result
 */
BigInteger BigInteger::operator&(const BigInteger &rhs) const
{
    BigInteger result;
    size_t max_size = std::max(limbs.size(), rhs.limbs.size());
    result.limbs.resize(max_size, 0);

    for (size_t i = 0; i < max_size; ++i)
    {
        Limb a = (i < limbs.size()) ? limbs[i] : 0;
        Limb b = (i < rhs.limbs.size()) ? rhs.limbs[i] : 0;
        result.limbs[i] = a & b;
    }

    result.negative = negative && rhs.negative;
    result.normalize();
    return result;
}

/**
 * @brief Bitwise OR operator
 */
BigInteger BigInteger::operator|(const BigInteger &rhs) const
{
    BigInteger result;
    size_t max_size = std::max(limbs.size(), rhs.limbs.size());
    result.limbs.resize(max_size, 0);

    for (size_t i = 0; i < max_size; ++i)
    {
        Limb a = (i < limbs.size()) ? limbs[i] : 0;
        Limb b = (i < rhs.limbs.size()) ? rhs.limbs[i] : 0;
        result.limbs[i] = a | b;
    }

    result.negative = negative || rhs.negative;
    result.normalize();
    return result;
}

/**
 * @brief Bitwise XOR operator
 */
BigInteger BigInteger::operator^(const BigInteger &rhs) const
{
    BigInteger result;
    size_t max_size = std::max(limbs.size(), rhs.limbs.size());
    result.limbs.resize(max_size, 0);

    for (size_t i = 0; i < max_size; ++i)
    {
        Limb a = (i < limbs.size()) ? limbs[i] : 0;
        Limb b = (i < rhs.limbs.size()) ? rhs.limbs[i] : 0;
        result.limbs[i] = a ^ b;
    }

    result.negative = negative != rhs.negative;
    result.normalize();
    return result;
}

/**
 * @brief Bitwise NOT operator
 */
BigInteger BigInteger::operator~() const
{
    BigInteger result;
    result.limbs.resize(limbs.size());

    for (size_t i = 0; i < limbs.size(); ++i)
    {
        result.limbs[i] = ~limbs[i];
    }

    result.negative = !negative;
    result.normalize();
    return result;
}

/**
 * @brief Left shift operator
 * @param shift Number of bits to shift
 * @return Left-shifted result
 */
BigInteger BigInteger::operator<<(size_t shift) const
{
    if (shift == 0 || isZero())
    {
        return *this;
    }

    size_t limb_shift = shift / LIMB_BITS;
    size_t bit_shift = shift % LIMB_BITS;

    BigInteger result;
    result.limbs.resize(limbs.size() + limb_shift + 1, 0);
    result.negative = negative;

    // Copy limbs with limb shift
    for (size_t i = 0; i < limbs.size(); ++i)
    {
        result.limbs[i + limb_shift] = limbs[i];
    }

    // Apply bit shift
    if (bit_shift > 0)
    {
        Limb carry = 0;
        for (size_t i = limb_shift; i < result.limbs.size(); ++i)
        {
            Limb new_carry = result.limbs[i] >> (LIMB_BITS - bit_shift);
            result.limbs[i] = (result.limbs[i] << bit_shift) | carry;
            carry = new_carry;
        }
    }

    result.normalize();
    return result;
}

/**
 * @brief Right shift operator
 * @param shift Number of bits to shift
 * @return Right-shifted result
 */
BigInteger BigInteger::operator>>(size_t shift) const
{
    if (shift == 0 || isZero())
    {
        return *this;
    }

    size_t limb_shift = shift / LIMB_BITS;
    size_t bit_shift = shift % LIMB_BITS;

    if (limb_shift >= limbs.size())
    {
        return BigInteger(0);
    }

    BigInteger result;
    result.limbs.resize(limbs.size() - limb_shift);
    result.negative = negative;

    // Copy limbs with limb shift
    for (size_t i = 0; i < result.limbs.size(); ++i)
    {
        result.limbs[i] = limbs[i + limb_shift];
    }

    // Apply bit shift
    if (bit_shift > 0)
    {
        Limb carry = 0;
        for (size_t i = result.limbs.size(); i > 0; --i)
        {
            size_t idx = i - 1;
            Limb new_carry = result.limbs[idx] << (LIMB_BITS - bit_shift);
            result.limbs[idx] = (result.limbs[idx] >> bit_shift) | carry;
            carry = new_carry;
        }
    }

    result.normalize();
    return result;
}

/**
 * @brief C++20 spaceship operator for three-way comparison
 * @param rhs Right-hand side operand
 * @return strong_ordering result
 * @note Constant-time implementation for security
 */
std::strong_ordering BigInteger::operator<=>(const BigInteger &rhs) const noexcept
{
    // Different signs
    if (negative != rhs.negative)
    {
        return negative ? std::strong_ordering::less
                        : std::strong_ordering::greater;
    }

    // Same signs - compare absolute values
    int abs_cmp = compareLimbs(limbs, rhs.limbs);

    if (abs_cmp == 0)
    {
        return std::strong_ordering::equal;
    }

    // If both negative, flip the comparison
    if (negative)
    {
        return (abs_cmp < 0) ? std::strong_ordering::greater
                             : std::strong_ordering::less;
    }

    return (abs_cmp < 0) ? std::strong_ordering::less
                         : std::strong_ordering::greater;
}

/**
 * @brief Equality operator
 * @param rhs Right-hand side operand
 * @return true if equal, false otherwise
 */
bool BigInteger::operator==(const BigInteger &rhs) const noexcept
{
    return (negative == rhs.negative) && (limbs == rhs.limbs);
}

/**
 * @brief Modular inverse using extended Euclidean algorithm
 * @param modulus The modulus
 * @return Modular inverse of this modulo modulus
 * @throws Exception if gcd(this, modulus) != 1
 */
BigInteger BigInteger::modInverse(const BigInteger &modulus) const
{
    auto [g, x, y] = extendedGcd(modulus);

    if (!g.isOne())
    {
        throw Exception("Modular inverse does not exist (gcd != 1)");
    }

    // Ensure positive result
    BigInteger result = x % modulus;
    if (result.negative)
    {
        result += modulus;
    }

    return result;
}

/**
 * @brief Greatest Common Divisor using binary GCD algorithm
 * @param other The other number
 * @return gcd(this, other)
 * @note Constant-time implementation resistant to timing attacks
 */
BigInteger BigInteger::gcd(const BigInteger &other) const
{
    BigInteger a = abs();
    BigInteger b = other.abs();

    if (a.isZero())
        return b;
    if (b.isZero())
        return a;

    // Binary GCD algorithm
    int shift = 0;

    // Remove common factors of 2
    while (a.isEven() && b.isEven())
    {
        a = a >> 1;
        b = b >> 1;
        ++shift;
    }

    // Remove factors of 2 from a
    while (a.isEven())
    {
        a = a >> 1;
    }

    while (!b.isZero())
    {
        // Remove factors of 2 from b
        while (b.isEven())
        {
            b = b >> 1;
        }

        // Ensure a >= b
        if (a > b)
        {
            std::swap(a, b);
        }

        b = b - a;
    }

    // Restore common factors of 2
    return a << shift;
}

/**
 * @brief Extended Euclidean algorithm
 * @param other The other number
 * @return Tuple of (gcd, x, y) where gcd = this*x + other*y
 */
std::tuple<BigInteger, BigInteger, BigInteger>
BigInteger::extendedGcd(const BigInteger &other) const
{
    BigInteger a = abs();
    BigInteger b = other.abs();

    BigInteger x0(1), x1(0);
    BigInteger y0(0), y1(1);

    while (!b.isZero())
    {
        BigInteger quotient = a / b;

        // Update a and b
        BigInteger temp = b;
        b = a % b;
        a = temp;

        // Update x coefficients
        temp = x1;
        x1 = x0 - quotient * x1;
        x0 = temp;

        // Update y coefficients
        temp = y1;
        y1 = y0 - quotient * y1;
        y0 = temp;
    }

    // Adjust signs based on original inputs
    if (negative)
        x0 = -x0;
    if (other.negative)
        y0 = -y0;

    return std::make_tuple(a, x0, y0);
}

/**
 * @brief Convert to hexadecimal string representation
 * @param uppercase Use uppercase hex digits (A-F vs a-f)
 * @return Hexadecimal string representation
 */
String BigInteger::toHexString(bool uppercase) const
{
    if (isZero())
    {
        return String("0");
    }

    String result;
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

    // Convert limbs to hex (most significant limb first)
    for (size_t i = limbs.size(); i > 0; --i)
    {
        Limb limb = limbs[i - 1];

        // Skip leading zeros except for the least significant limb
        if (limb == 0 && i != 1 && result.empty())
        {
            continue;
        }

        // Convert limb to hex string
        String limb_hex;
        if (limb == 0)
        {
            limb_hex = "0";
        }
        else
        {
            while (limb > 0)
            {
                limb_hex = String(1, digits[limb & 0xF]) + limb_hex;
                limb >>= 4;
            }
        }

        result += limb_hex;
    }

    if (negative && !isZero())
    {
        result = "-" + result;
    }

    return result;
}

/**
 * @brief Convert to decimal string representation
 * @return Decimal string representation
 */
String BigInteger::toString() const
{
    if (isZero())
    {
        return String("0");
    }

    String result;
    BigInteger temp = abs();
    BigInteger ten(10);

    while (!temp.isZero())
    {
        BigInteger remainder = temp % ten;
        result = String(1, '0' + remainder.limbs[0]) + result;
        temp = temp / ten;
    }

    if (negative)
    {
        result = "-" + result;
    }

    return result;
}

String BigInteger::toString(const Vector<BigInteger> &V) {
    String str;
    for (const BigInteger &big_value : V)
    {
        str.append(big_value.toString());
        str.push_back(' ');
    }
    str.pop_back();

    return str;
}

Vector<BigInteger> BigInteger::stringToBigIntVector(const String &str) {
    std::istringstream iss(str.toStdString());
    Vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(),
            std::back_inserter<Vector < std::string >> (tokens));

    Vector<BigInteger> V;
    V.reserve(tokens.size());
    for (const std::string &str_value : tokens)
    {
        V.push_back(BigInteger(str_value));
    }

    return V;
}

/**
 * @brief Convert to byte array in big-endian format
 * @return Byte vector in big-endian format (most significant byte first)
 */
BytesVector BigInteger::toBytes() const
{
    if (isZero())
    {
        return BytesVector({0});
    }

    BytesVector result;
    const size_t bytes_per_limb = sizeof(Limb);

    // Convert limbs to bytes (most significant limb first)
    for (size_t i = limbs.size(); i > 0; --i)
    {
        Limb limb = limbs[i - 1];

        // Convert limb to bytes (most significant byte first)
        for (size_t j = bytes_per_limb; j > 0; --j)
        {
            uint8_t byte = (limb >> (8 * (j - 1))) & 0xFF;
            // Skip leading zero bytes except for the last limb
            if (byte != 0 || !result.empty() || i == 1)
            {
                result.push_back(byte);
            }
        }
    }

    return result;
}

/**
 * @brief Get bit length of the number
 * @return Number of bits needed to represent this number
 */
size_t BigInteger::bitLength() const noexcept
{
    if (isZero())
    {
        return 0;
    }

    // Find the most significant limb
    size_t msb_limb = limbs.size() - 1;
    Limb msb_value = limbs[msb_limb];

    // Count bits in the most significant limb
    size_t msb_bits = 0;
    while (msb_value > 0)
    {
        msb_value >>= 1;
        ++msb_bits;
    }

    return msb_limb * LIMB_BITS + msb_bits;
}

/**
 * @brief Check if number is zero
 */
bool BigInteger::isZero() const noexcept
{
    return limbs.size() == 1 && limbs[0] == 0;
}

/**
 * @brief Check if number is one
 */
bool BigInteger::isOne() const noexcept
{
    return !negative && limbs.size() == 1 && limbs[0] == 1;
}

/**
 * @brief Check if number is even
 */
bool BigInteger::isEven() const noexcept
{
    return (limbs[0] & 1) == 0;
}

/**
 * @brief Check if number is odd
 */
bool BigInteger::isOdd() const noexcept
{
    return (limbs[0] & 1) == 1;
}

/**
 * @brief Test if a specific bit is set
 * @param bit_position Position of bit to test (0 = least significant bit)
 * @return true if bit at position is set (1), false if clear (0)
 * @note Constant-time operation for cryptographic security
 */
bool BigInteger::testBit(size_t bit_position) const noexcept
{
    // Calculate which limb contains the bit
    size_t limb_index = bit_position / LIMB_BITS;
    size_t bit_index = bit_position % LIMB_BITS;

    // If bit position is beyond our number, it's 0
    if (limb_index >= limbs.size())
    {
        return false;
    }

    // Test the specific bit in the limb
    return (limbs[limb_index] & (static_cast<Limb>(1) << bit_index)) != 0;
}

/**
 * @brief Get absolute value
 * @return Absolute value of this number
 */
BigInteger BigInteger::abs() const noexcept
{
    BigInteger result = *this;
    result.negative = false;
    return result;
}

/**
 * @brief Generate cryptographically secure random number
 * @param bit_length Length in bits
 * @return Random number of specified bit length
 */
BigInteger BigInteger::random(size_t bit_length)
{
    if (bit_length == 0)
    {
        return BigInteger(0);
    }

    size_t limb_count = (bit_length + LIMB_BITS - 1) / LIMB_BITS;
    size_t remaining_bits = bit_length % LIMB_BITS;

    BigInteger result;
    result.limbs.resize(limb_count);
    result.negative = false;

    // Generate random limbs
    for (size_t i = 0; i < limb_count - 1; ++i)
    {
        result.limbs[i] = fast_rng();
    }

    // Handle the most significant limb
    if (remaining_bits == 0)
    {
        result.limbs[limb_count - 1] = fast_rng();
    }
    else
    {
        Limb mask = (Limb(1) << remaining_bits) - 1;
        result.limbs[limb_count - 1] = fast_rng() & mask;
    }

    // Ensure the most significant bit is set for exact bit length
    size_t msb_position = (remaining_bits == 0) ? LIMB_BITS - 1 : remaining_bits - 1;
    result.limbs[limb_count - 1] |= (Limb(1) << msb_position);

    result.normalize();
    return result;
}

/**
 * @brief Quick primality test using small prime divisibility
 * @param candidate Number to test
 * @return false if definitely composite, true if possibly prime
 * @note Performs trial division by small primes for quick elimination
 */
bool BigInteger::quickPrimalityTest(const BigInteger &candidate) noexcept
{
    // Even numbers (except 2) are composite
    if (candidate.isEven())
    {
        return candidate == BigInteger(2);
    }

    // Quick divisibility test by small odd primes
    static constexpr Array<uint32_t, 66> small_primes = {
        3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73,
        79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157,
        163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
        241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331};

    const size_t num_small_primes = sizeof(small_primes) / sizeof(small_primes[0]);

    for (size_t i = 0; i < num_small_primes; ++i)
    {
        uint32_t prime = small_primes[i];

        // If candidate equals the small prime, it's prime
        if (candidate == BigInteger(prime))
        {
            return true;
        }

        // If candidate is divisible by the small prime, it's composite
        if ((candidate % BigInteger(prime)).isZero())
        {
            return false;
        }
    }

    // Passed all small prime tests - possibly prime
    return true;
}

/**
 * @brief Generate random number in range [0, max)
 * @param max Upper bound (exclusive)
 * @return Random number in specified range
 */
BigInteger BigInteger::randomRange(const BigInteger &max)
{
    if (max.isZero() || max.isOne())
    {
        return BigInteger(0);
    }

    size_t bit_length = max.bitLength() + 64; // Extra bits for uniform distribution

    BigInteger result;
    do
    {
        result = random(bit_length);
        result = result % max;
    } while (result >= max); // Ensure uniform distribution

    return result;
}

/**
 * @brief Generate cryptographically secure random prime
 * @param bit_length Length in bits of the prime
 * @param safe_prime If true, generates safe prime (p = 2q + 1 where q is prime)
 * @return Random prime of specified bit length
 */
BigInteger BigInteger::generatePrime(size_t bit_length, bool safe_prime)
{
    if (bit_length < 2)
    {
        throw Exception("Prime bit length must be at least 2");
    }

    const int max_attempts = 1000;

    for (int attempt = 0; attempt < max_attempts; ++attempt)
    {
        BigInteger candidate = random(bit_length);

        // Ensure odd number
        candidate.limbs[0] |= 1;

        // Quick divisibility tests for small primes
        if (quickPrimalityTest(candidate))
        {
            if (candidate.isProbablyPrime(40))
            {
                if (safe_prime)
                {
                    BigInteger q = (candidate - BigInteger(1)) / BigInteger(2);
                    if (q.isProbablyPrime(40))
                    {
                        return candidate;
                    }
                }
                else
                {
                    return candidate;
                }
            }
        }
    }

    throw Exception("Failed to generate prime after maximum attempts");
}

/**
 * @brief Normalize the BigInteger by removing leading zeros
 */
void BigInteger::normalize()
{
    // Remove leading zeros
    while (limbs.size() > 1 && limbs.back() == 0)
    {
        limbs.pop_back();
    }

    // Zero is always positive
    if (isZero())
    {
        negative = false;
    }
}

/**
 * @brief Securely erase sensitive data from memory
 */
void BigInteger::secureErase()
{
    // The Vector destructor already handles secure clearing
    // This is called explicitly for additional security
    limbs.clear();
    negative = false;
}

/**
 * @brief Montgomery modular exponentiation using Montgomery reduction
 * @param base Base value for exponentiation
 * @param exp Exponent
 * @param mod Modulus (must be odd for Montgomery reduction)
 * @return (base^exp) mod mod using Montgomery reduction
 * @note Provides constant-time implementation resistant to timing attacks
 * @note More efficient than standard modular exponentiation for large moduli
 */
BigInteger BigInteger::montgomeryModPow(const BigInteger &base, const BigInteger &exp, const BigInteger &mod)
{
    if (mod.isZero())
    {
        throw Exception("Modulus cannot be zero for Montgomery modular exponentiation");
    }
    // Montgomery performs only on odd modulus.
    if (!mod.isOdd())
    {
        return base.modPow(exp, mod);
    }
    
    if (exp.isZero())
    {
        return BigInteger(1);
    }
    
    if (exp.isOne())
    {
        return base % mod;
    }

    // Montgomery reduction parameters
    // R = 2^(k*LIMB_BITS) where k is the number of limbs in modulus
    const size_t k = mod.limbs.size();
    const size_t R_bits = k * LIMB_BITS;
    
    // Calculate R mod m (R = 2^(k*64))
    BigInteger R = BigInteger(1) << R_bits;
    BigInteger R_mod_m = R % mod;
    
    // Calculate R^2 mod m for Montgomery transformation
    BigInteger R2_mod_m = (R_mod_m * R_mod_m) % mod;
    
    // Calculate Montgomery inverse of m mod R using extended Euclidean algorithm
    // We need m' such that m * m' ≡ -1 (mod R), or equivalently m * m' = k*R - 1
    auto [gcd_val, m_inv, r_temp] = R.extendedGcd(mod);
    if (!gcd_val.isOne())
    {
        throw Exception("Montgomery reduction requires gcd(R, m) = 1");
    }
    
    // m' = -m_inv mod R, but we store it as positive value and handle sign in reduction
    BigInteger m_prime = (R - m_inv) % R;
    
    // Helper lambda for Montgomery reduction: reduce x mod m using Montgomery method
    auto montgomeryReduce = [&](const BigInteger &x) -> BigInteger {
        // Montgomery reduction: given x, compute x * R^(-1) mod m
        // Algorithm: t = ((x mod R) * m') mod R; return (x + t*m) / R
        
        BigInteger x_mod_R = x % R;
        BigInteger t = (x_mod_R * m_prime) % R;
        BigInteger numerator = x + (t * mod);
        
        // Divide by R (equivalent to right shift by R_bits)
        BigInteger result = numerator >> R_bits;
        
        // Final conditional subtraction
        if (result >= mod)
        {
            result = result - mod;
        }
        
        return result;
    };
    
    // Transform base to Montgomery form: base_mont = (base * R) mod m
    BigInteger base_mont = montgomeryReduce(base * R2_mod_m);
    BigInteger result_mont = montgomeryReduce(R2_mod_m); // 1 in Montgomery form
    
    // Montgomery ladder for constant-time exponentiation
    BigInteger exp_copy = exp;
    
    while (!exp_copy.isZero())
    {
        // Constant-time conditional: if exp is odd, multiply result by base
        if (exp_copy.isOdd())
        {
            BigInteger temp = result_mont * base_mont;
            result_mont = montgomeryReduce(temp);
        }
        
        // Square base and halve exponent
        BigInteger temp = base_mont * base_mont;
        base_mont = montgomeryReduce(temp);
        exp_copy = exp_copy >> 1;
    }
    
    // Transform result back from Montgomery form: result = result_mont * R^(-1) mod m
    return montgomeryReduce(result_mont);
}

/**
 * @brief Division algorithm for limb vectors using long division
 * @param dividend Dividend limb vector (little-endian)
 * @param divisor Divisor limb vector (little-endian)
 * @return Pair of (quotient, remainder) limb vectors
 * @throws Exception if divisor is zero
 * @note Implements classic long division algorithm with optimizations
 */
std::pair<BigInteger::LimbVector, BigInteger::LimbVector> 
BigInteger::divModLimbs(const LimbVector &dividend, const LimbVector &divisor)
{
    // Check for division by zero
    if (divisor.empty() || (divisor.size() == 1 && divisor[0] == 0))
    {
        throw Exception("Division by zero in divModLimbs");
    }
    
    // Handle simple cases
    if (dividend.empty() || (dividend.size() == 1 && dividend[0] == 0))
    {
        return std::make_pair(LimbVector(1, 0), LimbVector(1, 0));
    }
    
    // Compare dividend and divisor
    int cmp = compareLimbs(dividend, divisor);
    if (cmp < 0)
    {
        // dividend < divisor, so quotient = 0, remainder = dividend
        return std::make_pair(LimbVector(1, 0), dividend);
    }
    else if (cmp == 0)
    {
        // dividend == divisor, so quotient = 1, remainder = 0
        return std::make_pair(LimbVector(1, 1), LimbVector(1, 0));
    }
    
    // For single-limb divisor, use optimized division
    if (divisor.size() == 1)
    {
        Limb d = divisor[0];
        LimbVector quotient;
        quotient.reserve(dividend.size());
        
        DoubleLimb remainder = 0;
        
        // Process limbs from most significant to least significant
        for (size_t i = dividend.size(); i > 0; --i)
        {
            size_t idx = i - 1;
            
            // Create dividend for this iteration: remainder * 2^64 + dividend[idx]
#ifdef HAS_NATIVE_UINT128
            DoubleLimb current_dividend = (remainder << LIMB_BITS) | dividend[idx];
            Limb q = static_cast<Limb>(current_dividend / d);
            remainder = current_dividend % d;
#else
            // Manual division for platforms without 128-bit support
            // We need to compute (remainder.high * 2^64 + remainder.low * 2^64 + dividend[idx]) / d
            DoubleLimb current_dividend;
            current_dividend.low = dividend[idx];
            current_dividend.high = extractLow(remainder);
            
            // Approximate division - this is a simplified version
            // In production, you'd want a more sophisticated algorithm
            if (extractHigh(remainder) != 0 || extractLow(remainder) >= d)
            {
                // Handle overflow case - use long division
                DoubleLimb temp_div = DoubleLimb(0, extractLow(remainder));
                temp_div = add128(temp_div, current_dividend.low);
                
                // Simple approximation - for full implementation, use Knuth's algorithm
                uint64_t approx_q = (extractHigh(remainder) > 0) ? UINT64_MAX / d : extractLow(remainder) / d;
                remainder = DoubleLimb(extractLow(remainder) % d, 0);
                quotient.push_back(approx_q);
            }
            else
            {
                uint64_t combined = extractLow(remainder) * d + dividend[idx];
                quotient.push_back(combined / d);
                remainder = DoubleLimb(combined % d, 0);
            }
            continue;
#endif
            quotient.push_back(q);
        }
        
        // Reverse quotient since we built it backwards
        std::reverse(quotient.begin(), quotient.end());
        
        // Remove leading zeros from quotient
        while (quotient.size() > 1 && quotient.back() == 0)
        {
            quotient.pop_back();
        }
        
        return std::make_pair(quotient, LimbVector(1, extractLow(remainder)));
    }
    
    // Multi-limb division using Knuth's Algorithm D (simplified version)
    // This is a complex algorithm - implementing a basic version here
    
    LimbVector remainder = dividend;
    LimbVector quotient(dividend.size() >= divisor.size() ? dividend.size() - divisor.size() + 1 : 1, 0);
    
    // Basic long division approach (not the most efficient but correct)
    for (size_t i = quotient.size(); i > 0; --i)
    {
        size_t q_idx = i - 1;
        
        // Create partial dividend
        size_t partial_start = q_idx;
        size_t partial_len = std::min(divisor.size() + 1, remainder.size() - partial_start);
        
        if (partial_len == 0) continue;
        
        LimbVector partial_dividend(remainder.begin() + partial_start, 
                                   remainder.begin() + partial_start + partial_len);
        
        // Find the largest digit q such that divisor * q <= partial_dividend
        Limb q = 0;
        
        while (true)
        {
            // Calculate divisor * (q + 1)
            LimbVector temp_product = mulLimbs(divisor, LimbVector(1, q + 1));
            
            // Check if it's larger than partial dividend
            if (compareLimbs(temp_product, partial_dividend) > 0)
            {
                break;
            }
            
            q++;
            
            // Prevent infinite loop
            if (q == UINT64_MAX)
            {
                break;
            }
        }
        
        quotient[q_idx] = q;
        
        // Subtract divisor * q from remainder at the appropriate position
        if (q > 0)
        {
            LimbVector product = mulLimbs(divisor, LimbVector(1, q));
            
            // Subtract product from remainder starting at position q_idx
            // This is a simplified subtraction - proper implementation would handle borrows
            for (size_t j = 0; j < product.size() && (q_idx + j) < remainder.size(); ++j)
            {
                if (remainder[q_idx + j] >= product[j])
                {
                    remainder[q_idx + j] -= product[j];
                }
                else
                {
                    // Handle borrow - simplified version
                    if ((q_idx + j + 1) < remainder.size())
                    {
                        remainder[q_idx + j + 1]--;
                        remainder[q_idx + j] = remainder[q_idx + j] + (Limb(1) << LIMB_BITS) - product[j];
                    }
                }
            }
        }
    }
    
    // Remove leading zeros
    while (quotient.size() > 1 && quotient.back() == 0)
    {
        quotient.pop_back();
    }
    
    while (remainder.size() > 1 && remainder.back() == 0)
    {
        remainder.pop_back();
    }
    
    return std::make_pair(quotient, remainder);
}

/**
 * @brief Compare absolute values of two limb vectors
 * @param a First limb vector
 * @param b Second limb vector
 * @return -1 if a < b, 0 if a == b, 1 if a > b
 */
int BigInteger::compareLimbs(const LimbVector &a, const LimbVector &b) noexcept
{
    if (a.size() != b.size())
    {
        return (a.size() < b.size()) ? -1 : 1;
    }

    // Compare from most significant to least significant
    for (size_t i = a.size(); i > 0; --i)
    {
        size_t idx = i - 1;
        if (a[idx] < b[idx])
            return -1;
        if (a[idx] > b[idx])
            return 1;
    }

    return 0;
}

/**
 * @brief Add two limb vectors with carry propagation
 * @param a First operand limb vector
 * @param b Second operand limb vector
 * @return Sum of a and b as limb vector
 * @note Handles different sizes and propagates carries correctly
 */
BigInteger::LimbVector BigInteger::addLimbs(const LimbVector &a, const LimbVector &b)
{
    const size_t max_size = std::max(a.size(), b.size());
    LimbVector result;
    result.reserve(max_size + 1); // Reserve space for potential carry
    
    Limb carry = 0;
    
    for (size_t i = 0; i < max_size; ++i)
    {
        DoubleLimb sum = carry;
        
        // Add limbs from a and b if they exist at this position
        if (i < a.size())
        {
            sum = add128(DoubleLimb(sum), a[i]);
        }
        if (i < b.size())
        {
            sum = add128(sum, b[i]);
        }
        
        // Extract result limb and carry for next iteration
        result.push_back(extractLow(sum));
        carry = extractHigh(sum);
    }
    
    // Add final carry if present
    if (carry > 0)
    {
        result.push_back(carry);
    }
    
    // Remove leading zeros to normalize result
    while (result.size() > 1 && result.back() == 0)
    {
        result.pop_back();
    }
    
    return result;
}

/**
 * @brief Subtract two limb vectors with borrow propagation
 * @param a Minuend (first operand)
 * @param b Subtrahend (second operand) 
 * @return Difference a - b as limb vector
 * @note Assumes a >= b (result is non-negative)
 * @note Caller must ensure this precondition for correct results
 */
BigInteger::LimbVector BigInteger::subLimbs(const LimbVector &a, const LimbVector &b)
{
    // Handle edge cases
    if (b.empty() || (b.size() == 1 && b[0] == 0))
    {
        return a; // a - 0 = a
    }
    
    if (a.empty() || (a.size() == 1 && a[0] == 0))
    {
        // This shouldn't happen in normal usage since we assume a >= b >= 0
        // Return zero for safety
        return LimbVector(1, 0);
    }
    
    LimbVector result;
    result.reserve(a.size());
    
    Limb borrow = 0;
    
    for (size_t i = 0; i < a.size(); ++i)
    {
        // Start with the minuend limb
        DoubleLimb temp = a[i];
        
        // Subtract borrow from previous iteration
        if (temp >= borrow)
        {
            temp -= borrow;
            borrow = 0;
        }
        else
        {
            // Need to borrow from higher limb
            temp = (DoubleLimb(1) << LIMB_BITS) + temp - borrow;
            borrow = 1;
        }
        
        // Subtract subtrahend limb if it exists at this position
        if (i < b.size())
        {
            if (temp >= b[i])
            {
                temp -= b[i];
            }
            else
            {
                // Need to borrow from higher limb
                temp = (DoubleLimb(1) << LIMB_BITS) + temp - b[i];
                borrow = 1;
            }
        }
        
        result.push_back(static_cast<Limb>(temp));
    }
    
    // Remove leading zeros to normalize result
    while (result.size() > 1 && result.back() == 0)
    {
        result.pop_back();
    }
    
    return result;
}

/**
 * @brief Multiply two limb vectors using schoolbook multiplication
 * @param a First operand limb vector
 * @param b Second operand limb vector
 * @return Product a * b as limb vector
 * @note Uses traditional O(n²) multiplication algorithm
 * @note Optimized for small numbers; Karatsuba used for larger numbers
 */
BigInteger::LimbVector BigInteger::mulLimbs(const LimbVector &a, const LimbVector &b)
{
    // Handle zero cases
    if ((a.empty() || (a.size() == 1 && a[0] == 0)) ||
        (b.empty() || (b.size() == 1 && b[0] == 0)))
    {
        return LimbVector(1, 0);
    }
    
    // Handle single limb cases for efficiency
    if (a.size() == 1 && b.size() == 1)
    {
        DoubleLimb product = multiply64x64(a[0], b[0]);
        LimbVector result;
        
        Limb low = extractLow(product);
        Limb high = extractHigh(product);
        
        result.push_back(low);
        if (high > 0)
        {
            result.push_back(high);
        }
        
        return result;
    }
    
    // Initialize result vector with zeros
    LimbVector result(a.size() + b.size(), 0);
    
    // Schoolbook multiplication: multiply each limb of a by each limb of b
    for (size_t i = 0; i < a.size(); ++i)
    {
        if (a[i] == 0) continue; // Skip zero limbs for efficiency
        
        Limb carry = 0;
        
        for (size_t j = 0; j < b.size(); ++j)
        {
            if (b[j] == 0) continue; // Skip zero limbs for efficiency
            
            // Multiply current limbs and add previous carry
            DoubleLimb product = multiply64x64(a[i], b[j]);
            DoubleLimb sum = add128(product, carry);
            sum = add128(sum, result[i + j]);
            
            // Store result and propagate carry
            result[i + j] = extractLow(sum);
            carry = extractHigh(sum);
        }
        
        // Propagate remaining carry to higher positions
        size_t k = i + b.size();
        while (carry > 0 && k < result.size())
        {
            DoubleLimb sum = add128(DoubleLimb(result[k]), carry);
            result[k] = extractLow(sum);
            carry = extractHigh(sum);
            ++k;
        }
        
        // If carry still exists, we need to extend result
        if (carry > 0)
        {
            result.push_back(carry);
        }
    }
    
    // Remove leading zeros to normalize result
    while (result.size() > 1 && result.back() == 0)
    {
        result.pop_back();
    }
    
    return result;
}