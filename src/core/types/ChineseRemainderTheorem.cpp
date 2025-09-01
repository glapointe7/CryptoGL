#include "ChineseRemainderTheorem.hpp"

using namespace CryptoGL;

/**
 * @brief Solve system of congruences using Chinese Remainder Theorem
 * @param remainders Vector of remainders [r1, r2, ..., rk]
 * @param moduli Vector of pairwise coprime moduli [m1, m2, ..., mk]
 * @return x such that x ≡ ri (mod mi) for all i
 * @throws Exception if moduli are not pairwise coprime or vectors have different sizes
 * 
 * Solves the system:
 * x ≡ r1 (mod m1)
 * x ≡ r2 (mod m2)
 * ...
 * x ≡ rk (mod mk)
 * 
 * Algorithm:
 * 1. Compute M = m1 * m2 * ... * mk
 * 2. For each i, compute Mi = M / mi
 * 3. Find yi such that Mi * yi ≡ 1 (mod mi) 
 * 4. Result: x = sum(ri * Mi * yi) mod M
 */
 BigInteger ChineseRemainderTheorem::solve(const Vector<BigInteger>& remainders, const Vector<BigInteger>& moduli) {
    if (remainders.size() != moduli.size()) {
        throw Exception("Remainders and moduli vectors must have same size");
    }

    if (remainders.empty()) {
        throw Exception("Cannot solve empty system");
    }

    const size_t n = remainders.size();

    // Special case: single congruence
    if (n == 1) {
        return remainders[0] % moduli[0];
    }

    // Step 1: Compute total modulus M = m1 * m2 * ... * mk
    BigInteger total_modulus(1);
    for (size_t i = 0; i < n; ++i) {
        if (moduli[i].isZero()) {
            throw Exception("Modulus cannot be zero");
        }
        total_modulus = total_modulus * moduli[i];
    }

    // Step 2-4: Compute solution using CRT formula
    BigInteger result(0);

    for (size_t i = 0; i < n; ++i) {
        // Mi = M / mi
        BigInteger Mi = total_modulus / moduli[i];

        // Find yi such that Mi * yi ≡ 1 (mod mi)
        BigInteger yi = Mi.modInverse(moduli[i]);

        // Add ri * Mi * yi to result
        BigInteger term = (remainders[i] * Mi) % total_modulus;
        term = (term * yi) % total_modulus;
        result = (result + term) % total_modulus;
    }

    // Ensure positive result
    if (result.isNegative()) {
        result = result + total_modulus;
    }

    return result;
}

/**
 * @brief RSA decryption using Chinese Remainder Theorem optimization
 * @param ciphertext Ciphertext to decrypt
 * @param d_p d mod (p-1) - private exponent modulo (p-1)
 * @param d_q d mod (q-1) - private exponent modulo (q-1)
 * @param p First prime factor of RSA modulus
 * @param q Second prime factor of RSA modulus  
 * @param q_inv q^-1 mod p - multiplicative inverse of q modulo p
 * @return Decrypted plaintext
 * 
 * CRT RSA Decryption Algorithm:
 * 1. Compute m1 = c^d_p mod p
 * 2. Compute m2 = c^d_q mod q  
 * 3. Compute h = q_inv * (m1 - m2) mod p
 * 4. Result: m = m2 + h * q
 * 
 * This is approximately 4x faster than direct modular exponentiation
 * because we work with half-size exponents and moduli.
 */
 BigInteger ChineseRemainderTheorem::rsaDecryptCRT(const BigInteger& ciphertext,
    const BigInteger& d_p,
    const BigInteger& d_q,
    const BigInteger& p,
    const BigInteger& q,
    const BigInteger& q_inv) {
    // Validate inputs
    if (p.isZero() || q.isZero()) {
        throw Exception("Prime factors cannot be zero");
    }

    if (d_p.isZero() || d_q.isZero()) {
        throw Exception("Private exponents cannot be zero");
    }

    // Create ModularArithmetic instances for efficient exponentiation
    ModularArithmetic mod_p(p);
    ModularArithmetic mod_q(q);

    // Step 1: Compute m1 = c^d_p mod p
    BigInteger c_mod_p = ciphertext % p;
    BigInteger m1 = mod_p.modularExponentiation(c_mod_p, d_p);

    // Step 2: Compute m2 = c^d_q mod q
    BigInteger c_mod_q = ciphertext % q;
    BigInteger m2 = mod_q.modularExponentiation(c_mod_q, d_q);

    // Step 3: Compute h = q_inv * (m1 - m2) mod p
    BigInteger diff = (m1 - m2 + p) % p; // Add p to ensure positive
    BigInteger h = (q_inv * diff) % p;

    // Step 4: Compute final result: m = m2 + h * q
    BigInteger result = m2 + h * q;

    // Verify result is in correct range [0, n) where n = p * q
    BigInteger n = p * q;
    result = result % n;
    if (result.isNegative()) {
        result = result + n;
    }

    return result;
}

/**
 * @brief Check if two BigIntegers are coprime
 * @param a First number
 * @param b Second number
 * @return true if gcd(a, b) = 1
 */
 bool ChineseRemainderTheorem::areCoprime(const BigInteger& a, const BigInteger& b) {
    return a.gcd(b).isOne();
}

/**
 * @brief Validate that all moduli are pairwise coprime
 * @param moduli Vector of moduli to check
 * @return true if all pairs are coprime, false otherwise
 */
bool ChineseRemainderTheorem::validateCoprimality(const Vector<BigInteger>& moduli) {
    for (size_t i = 0; i < moduli.size(); ++i) {
        for (size_t j = i + 1; j < moduli.size(); ++j) {
            if (!areCoprime(moduli[i], moduli[j])) {
                return false;
            }
        }
    }
    return true;
}