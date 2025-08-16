#include "BlockCipherModes.hpp"

using namespace CryptoGL;

/*
 * ECB Mode of encryption.
 */
BlockCipherModes::Block BlockCipherECBMode::encodeBlock(const Block &input_block)
{
    return encode(input_block);
}

BlockCipherModes::Block BlockCipherECBMode::decodeBlock(const Block &input_block)
{
    return decode(input_block);
}

/*
 * CBC Mode of encryption.
 */
BlockCipherModes::Block BlockCipherCBCMode::encodeBlock(const Block &input_block)
{
    const Block new_cipher_block = encode(input_block.Xor(previous_cipher_block));
    previous_cipher_block = new_cipher_block;

    return new_cipher_block;
}

BlockCipherModes::Block BlockCipherCBCMode::decodeBlock(const Block &input_block)
{
    const Block clear_block = decode(input_block).Xor(previous_cipher_block);
    previous_cipher_block = input_block;

    return clear_block;
}

/*
 * CFB Mode of encryption.
 */
BlockCipherModes::Block BlockCipherCFBMode::encodeBlock(const Block &input_block)
{
    const Block output = encode(next_input_block);
    const Block cipher = input_block.Xor(output);
    next_input_block = cipher;

    return cipher;
}

BlockCipherModes::Block BlockCipherCFBMode::decodeBlock(const Block &input_block)
{
    const Block output = encode(next_input_block);
    const Block clear = input_block.Xor(output);
    next_input_block = input_block;

    return clear;
}

/*
 * OFB Mode of encryption.
 */
BlockCipherModes::Block BlockCipherOFBMode::encodeBlock(const Block &input_block)
{
    const Block output = encode(next_input_block);
    const Block cipher = input_block.Xor(output);
    next_input_block = output;

    return cipher;
}

BlockCipherModes::Block BlockCipherOFBMode::decodeBlock(const Block &input_block)
{
    return encodeBlock(input_block);
}

/*
 * CTR Mode of encryption.
 */
BlockCipherModes::Block BlockCipherCTRMode::encodeBlock(const Block &input_block)
{
    const Block output = encode(new_IV);
    new_IV = increaseCounter();

    return input_block.Xor(output);
}

BlockCipherModes::Block BlockCipherCTRMode::decodeBlock(const Block &input_block)
{
    return encodeBlock(input_block);
}

BlockCipherModes::Block BlockCipherCTRMode::increaseCounter()
{
    ++counter;
    Block tmpIV = IV;
    tmpIV.extend(BigEndian64::toBytesVector(counter));
    return tmpIV;
}