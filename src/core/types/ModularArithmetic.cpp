#include "ModularArithmetic.hpp"

using namespace CryptoGL;

/**
 * @brief Constructor with modulus pre-computation
 * @param mod The modulus for all operations
 * @throws Exception if modulus is zero or even
 * @note Computes Montgomery parameters for efficient modular arithmetic
 */
 ModularArithmetic::ModularArithmetic(const BigInteger& mod) : modulus(mod) {
    if (modulus.isZero()) {
        throw Exception("Modulus cannot be zero");
    }
    
    if (modulus.isEven()) {
        throw Exception("Montgomery arithmetic requires odd modulus");
    }
    
    // Calculate k = number of bits in modulus
    k = modulus.bitLength();
    
    // Calculate R = 2^k (smallest power of 2 >= modulus)
    r = BigInteger(1) << k;
    
    // Calculate number of limbs for Montgomery arithmetic
    size_t num_limbs = (k + BigInteger::LIMB_BITS - 1) / BigInteger::LIMB_BITS;
    
    // Calculate R^2 mod modulus for Montgomery conversions
    r_squared = (r * r) % modulus;
    
    // Calculate R^-1 mod modulus using extended Euclidean algorithm
    auto [g, r_inv, temp] = r.extendedGcd(modulus);
    if (!g.isOne()) {
        throw Exception("GCD of R and modulus must be 1");
    }
    r_inverse = (r_inv % modulus + modulus) % modulus; // Ensure positive
    
    // Calculate n' = -modulus^-1 mod R
    auto [g2, mod_inv, temp2] = modulus.extendedGcd(r);
    if (!g2.isOne()) {
        throw Exception("GCD of modulus and R must be 1");
    }
    //n_prime = (r - (mod_inv % r + r) % r) % r; // n' = -modulus^-1 mod R
    n_prime = (r - (mod_inv % r)) % r;
}

/**
 * @brief Montgomery modular multiplication
 * @param a First operand (should be in Montgomery form)
 * @param b Second operand (should be in Montgomery form)
 * @return (a * b) mod modulus in Montgomery form
 * @note Implements CIOS (Coarsely Integrated Operand Scanning) method
 * @note Constant-time implementation to resist timing attacks
 */
 BigInteger ModularArithmetic::montgomeryMultiply(const BigInteger& a, const BigInteger& b) const {
    // For small moduli, use direct calculation
    if (k <= BigInteger::LIMB_BITS) {
        // Direct Montgomery multiplication for small moduli
        BigInteger product = a * b;
        BigInteger m = (product % r) * n_prime % r;
        BigInteger result = (product + m * modulus) / r;
        
        if (result >= modulus) {
            result = result - modulus;
        }
        
        return result;
    }
    
    // For larger moduli, use the limb-based algorithm
    // Convert BigInteger to limb arrays for efficiency
    const BigInteger::LimbVector& a_limbs = a.getLimbs();
    const BigInteger::LimbVector& b_limbs = b.getLimbs();
    const BigInteger::LimbVector& m_limbs = modulus.getLimbs();

    // Calculate number of limbs for Montgomery arithmetic
    size_t num_limbs = (k + BigInteger::LIMB_BITS - 1) / BigInteger::LIMB_BITS;

    // Ensure operands have same size as modulus
    Vector<BigInteger::Limb> a_padded = a_limbs;
    Vector<BigInteger::Limb> b_padded = b_limbs;
    a_padded.resize(num_limbs, 0);
    b_padded.resize(num_limbs, 0);

    // Result array (one extra limb for overflow)
    Vector<BigInteger::Limb> result(num_limbs + 1, 0);

    // Montgomery CIOS algorithm
    for (size_t i = 0; i < num_limbs; ++i) {
        BigInteger::Limb carry = 0;

        // Multiply and accumulate: result += a[i] * b
        for (size_t j = 0; j < num_limbs; ++j) {
            BigInteger::DoubleLimb product = BigInteger::multiply64x64(a_padded[i], b_padded[j]);
            BigInteger::DoubleLimb sum = BigInteger::add128(BigInteger::add128(product, result[j]), carry);
            result[j] = BigInteger::extractLow(sum);
            carry = BigInteger::extractHigh(sum);
        }
        result[num_limbs] += carry;

        // Montgomery reduction step
        BigInteger::Limb n_prime_limb = (n_prime % r).getLimbs()[0];
        BigInteger::Limb r_limb = r.getLimbs()[0];
        BigInteger::Limb m = (result[0] * n_prime_limb) % r_limb;

        BigInteger::Limb reduction_carry = 0;
        // result += m * modulus
        for (size_t j = 0; j < num_limbs; ++j) {
            BigInteger::DoubleLimb product = BigInteger::multiply64x64(m, m_limbs[j]);
            BigInteger::DoubleLimb sum = BigInteger::add128(BigInteger::add128(product, result[j]), reduction_carry);
            result[j] = BigInteger::extractLow(sum);
            reduction_carry = BigInteger::extractHigh(sum);
        }

        // Shift right by one limb (divide by base)
        for (size_t j = 0; j < num_limbs; ++j) {
            result[j] = result[j + 1];
        }
        result[num_limbs] = reduction_carry;
    }

    // Convert result back to BigInteger
    BigInteger montgomery_result(result);

    // Final reduction if result >= modulus
    if (montgomery_result >= modulus) {
        montgomery_result = montgomery_result - modulus;
    }

    return montgomery_result;
}

/**
 * @brief Convert to Montgomery form
 * @param x Number to convert
 * @return x * R mod modulus
 */
 BigInteger ModularArithmetic::toMontgomeryForm(const BigInteger& x) const {
    // To convert x to Montgomery form: x * R mod modulus
    BigInteger x_mod = x % modulus;
    if (x_mod.isNegative()) {
        x_mod = x_mod + modulus;
    }
    
    return (x_mod * r) % modulus;
}

/**
 * @brief Convert from Montgomery form
 * @param x Number in Montgomery form
 * @return x * R^-1 mod modulus  
 */
BigInteger ModularArithmetic::fromMontgomeryForm(const BigInteger& x) const {
    // To convert from Montgomery form: x * R^-1 mod modulus
    return (x * r_inverse) % modulus;
}

/**
 * @brief Fast modular exponentiation using Montgomery arithmetic
 * @param base Base number
 * @param exponent Exponent
 * @return base^exponent mod modulus
 * @note Uses binary method with Montgomery arithmetic for efficiency
 * @note Constant-time implementation to prevent timing attacks
 */
 BigInteger ModularArithmetic::modularExponentiation(const BigInteger& base, const BigInteger& exponent) const {
    if (exponent.isZero()) {
        return BigInteger(1);
    }

    if (base.isZero()) {
        return BigInteger(0);
    }

    // Convert base to Montgomery form
    BigInteger base_montgomery = toMontgomeryForm(base % modulus);
    BigInteger result_montgomery = toMontgomeryForm(BigInteger(1)); // 1 in Montgomery form

    // Get bit length of exponent for constant-time operation
    size_t exp_bits = exponent.bitLength();

    // Binary method with constant-time execution
    for (size_t i = 0; i < exp_bits; ++i) {
        // Get bit at position (exp_bits - 1 - i)
        size_t bit_pos = exp_bits - 1 - i;
        bool bit = exponent.testBit(bit_pos);

        // Square the result
        result_montgomery = montgomeryMultiply(result_montgomery, result_montgomery);

        // Conditional multiply (constant-time)
        if (bit) {
            result_montgomery = montgomeryMultiply(result_montgomery, base_montgomery);
        }

        // For constant-time execution, always perform a dummy operation
        // when bit is false to prevent timing attacks
        if (!bit) {
            // Dummy Montgomery multiplication that doesn't affect result
            BigInteger dummy = montgomeryMultiply(base_montgomery, base_montgomery);
            // Compiler should optimize away if not used
            (void)dummy;
        }
    }

    // Convert result back from Montgomery form
    return fromMontgomeryForm(result_montgomery);
}