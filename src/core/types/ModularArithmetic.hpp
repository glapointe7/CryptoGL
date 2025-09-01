#pragma once

#include "BigInteger.hpp"

namespace CryptoGL {

/**
 * @brief Specialized class for efficient modular arithmetic operations
 * 
 * Optimizes repeated modular operations with the same modulus by
 * pre-computing Montgomery parameters and other optimization constants.
 */
class ModularArithmetic {
private:
    BigInteger modulus;
    BigInteger r;           // R = 2^(k*LIMB_BITS) where k = modulus.limb_count()
    BigInteger r_squared;   // R^2 mod modulus
    BigInteger r_inverse;   // R^-1 mod modulus
    BigInteger n_prime;     // -modulus^-1 mod R
    size_t k;               // Number of limbs in modulus

public:
    /**
     * @brief Constructor with modulus pre-computation
     * @param mod The modulus for all operations
     */
    explicit ModularArithmetic(const BigInteger& mod);
    
    /**
     * @brief Montgomery modular multiplication
     * @param a First operand (should be in Montgomery form)
     * @param b Second operand (should be in Montgomery form)
     * @return (a * b) mod modulus in Montgomery form
     */
    [[nodiscard]] BigInteger montgomeryMultiply(const BigInteger& a, 
                                                 const BigInteger& b) const;
    
    /**
     * @brief Convert to Montgomery form
     * @param x Number to convert
     * @return x * R mod modulus
     */
    [[nodiscard]] BigInteger toMontgomeryForm(const BigInteger& x) const;
    
    /**
     * @brief Convert from Montgomery form
     * @param x Number in Montgomery form
     * @return x / R mod modulus
     */
    [[nodiscard]] BigInteger fromMontgomeryForm(const BigInteger& x) const;
    
    /**
     * @brief Fast modular exponentiation using Montgomery arithmetic
     * @param base Base number
     * @param exponent Exponent
     * @return base^exponent mod modulus
     */
    [[nodiscard]] BigInteger modularExponentiation(const BigInteger& base, 
                                                       const BigInteger& exponent) const;
};
}