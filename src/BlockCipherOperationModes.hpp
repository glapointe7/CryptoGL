
#ifndef BLOCKCIPHEROPERATIONMODES_HPP
#define BLOCKCIPHEROPERATIONMODES_HPP

/* MODES OF OPERATION
 * 
 * ECB : C_i = encode(P_i).
 *       P_i = decode(C_i).
 *
 * CBC : C_i = encode(P_i ^ C_i-1) with IV = C_0.
 *       P_i = decode(C_i) ^ C_i-1 with IV = C_0.
 * 
 * CFB : C_i = encode(C_i-1) ^ P_i with IV = C_0.
 *       P_i = encode(C_i-1) ^ C_i ^ C_i with IV = C_0.
 * 
 * OFB : C_i = P_i ^ O_i with IV = I_0 and O_i = encode(I_i) and I_i = O_i-1.
 *       P_i = C_i ^ O_i with IV = I_0.and O_i = encode(I_i) and I_i = O_i-1.
 */

// Modes of operation to encode / decode a block cipher.
// Default to ECB mode.
enum class OperationModes : uint8_t
{
   ECB,     // electronic codebook
   CBC,     // Cipher-block chaining
   //CFB,     // Cipher feedback
   //OFB,     // Output feedback
   //CTR      // Counter
};

#endif