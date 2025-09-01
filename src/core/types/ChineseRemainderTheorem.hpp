#pragma once

#include "BigInteger.hpp"
#include "ModularArithmetic.hpp"

namespace CryptoGL {
    
/**
 * @brief Chinese Remainder Theorem solver for RSA optimization
 */
class ChineseRemainderTheorem {
public:
    /**
     * @brief Solve system of congruences using CRT
     * @param remainders Vector of remainders [r1, r2, ..., rk]
     * @param moduli Vector of pairwise coprime moduli [m1, m2, ..., mk]
     * @return x such that x ≡ ri (mod mi) for all i
     */
    [[nodiscard]] static BigInteger solve(const Vector<BigInteger>& remainders,
                                          const Vector<BigInteger>& moduli);
    
    /**
     * @brief RSA decryption using CRT optimization
     * @param ciphertext Ciphertext to decrypt
     * @param d_p d mod (p-1)
     * @param d_q d mod (q-1)
     * @param p First prime factor
     * @param q Second prime factor
     * @param q_inv q^-1 mod p
     * @return Decrypted plaintext
     */
    [[nodiscard]] static BigInteger rsaDecryptCRT(const BigInteger& ciphertext,
                                                    const BigInteger& d_p,
                                                    const BigInteger& d_q,
                                                    const BigInteger& p,
                                                    const BigInteger& q,
                                                    const BigInteger& q_inv);
    
    /**
     * @brief Check if two BigIntegers are coprime
     * @param a First number
     * @param b Second number
     * @return true if gcd(a, b) = 1
     */
    [[nodiscard]] static bool areCoprime(const BigInteger& a, const BigInteger& b);
    
    /**
     * @brief Validate that all moduli are pairwise coprime
     * @param moduli Vector of moduli to check
     * @return true if all pairs are coprime
     * @throws Exception if any pair is not coprime
     */
    [[nodiscard]] static bool validateCoprimality(const Vector<BigInteger>& moduli);
};
}