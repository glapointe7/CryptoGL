#pragma once

#include "Test.hpp"
#include "../../src/core/types/Integer.hpp"

namespace UnitTests
{
    class IntegerTest : public Test
    {
    protected:
        void setUp() override
        {
            // No setup required for static methods
        }

        void tearDown() override
        {
            // No cleanup required for static methods
        }
    };

    // ========================================
    // LSB (Least Significant Bits) Tests
    // ========================================
    
    TEST(IntegerLSBBasic, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test with uint8_t
        compare(0x0FULL, static_cast<uint64_t>(uint8::lsb(0xFF, 4)));
        compare(0x03ULL, static_cast<uint64_t>(uint8::lsb(0x53, 2)));
        compare(0x01ULL, static_cast<uint64_t>(uint8::lsb(0x55, 1)));
        
        // Test with uint16_t
        compare(0x00FFULL, static_cast<uint64_t>(uint16::lsb(0x12FF, 8)));
        compare(0x1234ULL, static_cast<uint64_t>(uint16::lsb(0x1234, 16)));
        
        // Test with uint32_t
        compare(0x0000FFFFULL, static_cast<uint64_t>(uint32::lsb(0x12345678, 16)));
        compare(0x12345678ULL, static_cast<uint64_t>(uint32::lsb(0x12345678, 32)));
        
        // Test with uint64_t
        compare(0x12345678ULL, static_cast<uint64_t>(uint64::lsb(0x123456789ABCDEF0, 32)));
    }
    
    TEST(IntegerLSBEdgeCases, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Extract 0 bits should return 0
        compare(0ULL, static_cast<uint64_t>(uint8::lsb(0xFF, 0)));
        compare(0ULL, static_cast<uint64_t>(uint16::lsb(0xFFFF, 0)));
        
        // Extract all bits
        compare(0xFFULL, static_cast<uint64_t>(uint8::lsb(0xFF, 8)));
        compare(0xFFFFULL, static_cast<uint64_t>(uint16::lsb(0xFFFF, 16)));
        compare(0xFFFFFFFFULL, static_cast<uint64_t>(uint32::lsb(0xFFFFFFFF, 32)));
        
        // Extract from zero
        compare(0ULL, static_cast<uint64_t>(uint8::lsb(0x00, 4)));
        compare(0ULL, static_cast<uint64_t>(uint16::lsb(0x0000, 8)));
    }
    
    // ========================================
    // MSB (Most Significant Bits) Tests  
    // ========================================
    
    TEST(IntegerMSBBasic, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test with uint8_t (8 bits)
        compare(0x0FULL, static_cast<uint64_t>(uint8::msb(0xFF, 4)));
        compare(0x05ULL, static_cast<uint64_t>(uint8::msb(0x53, 2))); // 0x53 = 01010011, top 2 bits = 01 = 0x1, but 0x53 >> 6 = 0x1
        compare(0x00ULL, static_cast<uint64_t>(uint8::msb(0x55, 1))); // 0x55 = 01010101, top 1 bit = 0
        
        // Test with uint16_t (16 bits)
        compare(0x12ULL, static_cast<uint64_t>(uint16::msb(0x12FF, 8)));
        compare(0x1234ULL, static_cast<uint64_t>(uint16::msb(0x1234, 16)));
        
        // Test with uint32_t (32 bits)
        compare(0x1234ULL, static_cast<uint64_t>(uint32::msb(0x12345678, 16)));
        compare(0x12345678ULL, static_cast<uint64_t>(uint32::msb(0x12345678, 32)));
        
        // Test with uint64_t (64 bits)
        compare(0x123456789ABCDEF0ULL, static_cast<uint64_t>(uint64::msb(0x123456789ABCDEF0, 64)));
        compare(0x12345678ULL, static_cast<uint64_t>(uint64::msb(0x123456789ABCDEF0, 32)));
    }
    
    TEST(IntegerMSBEdgeCases, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Extract 0 bits should return 0
        compare(0ULL, static_cast<uint64_t>(uint8::msb(0xFF, 0)));
        compare(0ULL, static_cast<uint64_t>(uint16::msb(0xFFFF, 0)));
        
        // Extract all bits
        compare(0xFFULL, static_cast<uint64_t>(uint8::msb(0xFF, 8)));
        compare(0xFFFFULL, static_cast<uint64_t>(uint16::msb(0xFFFF, 16)));
        
        // Extract from zero
        compare(0ULL, static_cast<uint64_t>(uint8::msb(0x00, 4)));
        compare(0ULL, static_cast<uint64_t>(uint16::msb(0x0000, 8)));
    }
    
    // ========================================
    // Rotate Left Tests
    // ========================================
    
    TEST(IntegerRotateLeftBasic, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test basic rotation with uint8_t
        compare(0xAAULL, static_cast<uint64_t>(uint8::rotateLeft(0x55, 1))); // 01010101 -> 10101010
        compare(0x55ULL, static_cast<uint64_t>(uint8::rotateLeft(0xAA, 1))); // 10101010 -> 01010101 (with wrap)
        
        // Test with uint16_t  
        compare(0x2468ULL, static_cast<uint64_t>(uint16::rotateLeft(0x1234, 1))); // Shift left by 1
        
        // Test with uint32_t
        compare(0x2468ACF0ULL, static_cast<uint64_t>(uint32::rotateLeft(0x12345678, 1)));
        
        // Test rotation by 0 (should return original value)
        compare(0x1234ULL, static_cast<uint64_t>(uint16::rotateLeft(0x1234, 0)));
    }
    
    TEST(IntegerRotateLeftWithMax, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test rotate left with specified max bits
        compare(0x02ULL, static_cast<uint64_t>(uint8::rotateLeft(0x01, 1, 4))); // Within 4 bits: 0001 -> 0010
        compare(0x01ULL, static_cast<uint64_t>(uint8::rotateLeft(0x08, 1, 4))); // Within 4 bits: 1000 -> 0001 (wrap)
        
        // Test with different max values
        compare(0x04ULL, static_cast<uint64_t>(uint16::rotateLeft(0x02, 1, 8))); // Within 8 bits
        compare(0x01ULL, static_cast<uint64_t>(uint16::rotateLeft(0x80, 1, 8))); // Within 8 bits with wrap
    }
    
    TEST(IntegerRotateLeftEdgeCases, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Rotate by full width should return original
        compare(0x12ULL, static_cast<uint64_t>(uint8::rotateLeft(0x12, 8)));
        compare(0x1234ULL, static_cast<uint64_t>(uint16::rotateLeft(0x1234, 16)));
        
        // Rotate zero
        compare(0ULL, static_cast<uint64_t>(uint8::rotateLeft(0x00, 4)));
        compare(0ULL, static_cast<uint64_t>(uint16::rotateLeft(0x0000, 8)));
    }
    
    // ========================================
    // Rotate Right Tests
    // ========================================
    
    TEST(IntegerRotateRightBasic, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test basic rotation with uint8_t
        compare(0xAAULL, static_cast<uint64_t>(uint8::rotateRight(0x55, 1, 8))); // 01010101 -> 10101010 (with wrap)
        compare(0x55ULL, static_cast<uint64_t>(uint8::rotateRight(0xAA, 1, 8))); // 10101010 -> 01010101
        
        // Test with uint16_t
        compare(0x091AULL, static_cast<uint64_t>(uint16::rotateRight(0x1234, 1, 16))); // Right rotate by 1
        
        // Test with uint32_t
        compare(0x091A2B3CULL, static_cast<uint64_t>(uint32::rotateRight(0x12345678, 1, 32)));
        
        // Test rotation by 0 (should return original value)
        compare(0x1234ULL, static_cast<uint64_t>(uint16::rotateRight(0x1234, 0, 16)));
    }
    
    TEST(IntegerRotateRightWithDifferentMax, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test rotate right with specified max bits
        compare(0x08ULL, static_cast<uint64_t>(uint8::rotateRight(0x01, 1, 4))); // Within 4 bits: 0001 -> 1000 (wrap)
        compare(0x04ULL, static_cast<uint64_t>(uint8::rotateRight(0x08, 1, 4))); // Within 4 bits: 1000 -> 0100
        
        // Test with 8 bits
        compare(0x80ULL, static_cast<uint64_t>(uint16::rotateRight(0x01, 1, 8))); // Within 8 bits with wrap
        compare(0x40ULL, static_cast<uint64_t>(uint16::rotateRight(0x80, 1, 8))); // Within 8 bits
    }
    
    TEST(IntegerRotateRightDefaultMax, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test default max parameter (should use full bit width)
        compare(0x091A2B3CULL, static_cast<uint64_t>(uint32::rotateRight(0x12345678, 1))); // Default max = 32
        compare(0x80000000ULL, static_cast<uint64_t>(uint32::rotateRight(0x00000001, 1))); // Wrap around
    }
    
    // ========================================
    // Bit Position Tests
    // ========================================
    
    TEST(IntegerGetBitAtPosition, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test with uint8_t
        compare(true, uint8::getBitAtPosition(0x55, 0)); // 01010101, bit 0 = 1
        compare(false, uint8::getBitAtPosition(0x55, 1)); // 01010101, bit 1 = 0
        compare(true, uint8::getBitAtPosition(0x55, 2)); // 01010101, bit 2 = 1
        compare(false, uint8::getBitAtPosition(0x55, 7)); // 01010101, bit 7 = 0
        
        // Test with uint16_t
        compare(true, uint16::getBitAtPosition(0x8000, 15)); // Bit 15 = 1
        compare(false, uint16::getBitAtPosition(0x7FFF, 15)); // Bit 15 = 0
        
        // Test with uint32_t
        compare(true, uint32::getBitAtPosition(0x80000000, 31)); // Bit 31 = 1
        compare(false, uint32::getBitAtPosition(0x7FFFFFFF, 31)); // Bit 31 = 0
        
        // Test with zero
        compare(false, uint8::getBitAtPosition(0x00, 0));
        compare(false, uint8::getBitAtPosition(0x00, 7));
    }
    
    TEST(IntegerSetBitAtPosition, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test with uint8_t
        compare(0x01ULL, static_cast<uint64_t>(uint8::setBitAtPosition(0x00, 0))); // Set bit 0
        compare(0x80ULL, static_cast<uint64_t>(uint8::setBitAtPosition(0x00, 7))); // Set bit 7
        compare(0x55ULL, static_cast<uint64_t>(uint8::setBitAtPosition(0x54, 0))); // Set bit 0 in 01010100
        
        // Test with uint16_t
        compare(0x8000ULL, static_cast<uint64_t>(uint16::setBitAtPosition(0x0000, 15))); // Set bit 15
        compare(0x8001ULL, static_cast<uint64_t>(uint16::setBitAtPosition(0x8000, 0))); // Set bit 0
        
        // Test with uint32_t
        compare(0x80000000ULL, static_cast<uint64_t>(uint32::setBitAtPosition(0x00000000, 31))); // Set bit 31
        
        // Setting already set bit should not change value
        compare(0xFFULL, static_cast<uint64_t>(uint8::setBitAtPosition(0xFF, 0)));
        compare(0xFFULL, static_cast<uint64_t>(uint8::setBitAtPosition(0xFF, 7)));
    }
    
    // ========================================
    // Byte Operations Tests
    // ========================================
    
    TEST(IntegerGetByteAtPosition, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test with uint16_t
        compare(0x34ULL, static_cast<uint64_t>(uint16::getByteAtPosition(0x1234, 0))); // Low byte
        compare(0x12ULL, static_cast<uint64_t>(uint16::getByteAtPosition(0x1234, 1))); // High byte
        
        // Test with uint32_t
        compare(0x78ULL, static_cast<uint64_t>(uint32::getByteAtPosition(0x12345678, 0))); // Byte 0
        compare(0x56ULL, static_cast<uint64_t>(uint32::getByteAtPosition(0x12345678, 1))); // Byte 1
        compare(0x34ULL, static_cast<uint64_t>(uint32::getByteAtPosition(0x12345678, 2))); // Byte 2
        compare(0x12ULL, static_cast<uint64_t>(uint32::getByteAtPosition(0x12345678, 3))); // Byte 3
        
        // Test with uint64_t
        compare(0xF0ULL, static_cast<uint64_t>(uint64::getByteAtPosition(0x123456789ABCDEF0, 0))); // Byte 0
        compare(0xDEULL, static_cast<uint64_t>(uint64::getByteAtPosition(0x123456789ABCDEF0, 1))); // Byte 1
        compare(0x12ULL, static_cast<uint64_t>(uint64::getByteAtPosition(0x123456789ABCDEF0, 7))); // Byte 7
    }
    
    TEST(IntegerBytesSwap, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test with uint16_t
        compare(0x3412ULL, static_cast<uint64_t>(uint16::bytesSwap(0x1234)));
        compare(0x0001ULL, static_cast<uint64_t>(uint16::bytesSwap(0x0100)));
        
        // Test with uint32_t
        compare(0x78563412ULL, static_cast<uint64_t>(uint32::bytesSwap(0x12345678)));
        compare(0x01000000ULL, static_cast<uint64_t>(uint32::bytesSwap(0x00000001)));
        
        // Test with uint64_t
        compare(0xF0DEBC9A78563412ULL, static_cast<uint64_t>(uint64::bytesSwap(0x123456789ABCDEF0)));
        
        // Test with uint8_t (should return same value)
        compare(0x12ULL, static_cast<uint64_t>(uint8::bytesSwap(0x12)));
        compare(0xFFULL, static_cast<uint64_t>(uint8::bytesSwap(0xFF)));
        
        // Test swapping zero
        compare(0ULL, static_cast<uint64_t>(uint16::bytesSwap(0x0000)));
        compare(0ULL, static_cast<uint64_t>(uint32::bytesSwap(0x00000000)));
    }
    
    TEST(IntegerBytesSwapSymmetry, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test that double swap returns original
        uint16_t value16 = 0x1234;
        compare(static_cast<uint64_t>(value16), static_cast<uint64_t>(uint16::bytesSwap(uint16::bytesSwap(value16))));
        
        uint32_t value32 = 0x12345678;
        compare(static_cast<uint64_t>(value32), static_cast<uint64_t>(uint32::bytesSwap(uint32::bytesSwap(value32))));
        
        uint64_t value64 = 0x123456789ABCDEF0;
        compare(value64, uint64::bytesSwap(uint64::bytesSwap(value64)));
    }
    
    // ========================================
    // String Conversion Tests
    // ========================================
    
    TEST(IntegerToStringBasic, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test basic numbers
        compare("0", uint8::toString(0));
        compare("1", uint8::toString(1));
        compare("255", uint8::toString(255));
        
        compare("0", uint16::toString(0));
        compare("65535", uint16::toString(65535));
        compare("1234", uint16::toString(1234));
        
        compare("0", uint32::toString(0));
        compare("4294967295", uint32::toString(4294967295U));
        compare("12345678", uint32::toString(12345678));
        
        compare("0", uint64::toString(0));
        compare("18446744073709551615", uint64::toString(18446744073709551615ULL));
        compare("123456789", uint64::toString(123456789));
    }
    
    TEST(IntegerToStringEdgeCases, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test maximum values for each type
        compare("255", uint8::toString(std::numeric_limits<uint8_t>::max()));
        compare("65535", uint16::toString(std::numeric_limits<uint16_t>::max()));
        compare("4294967295", uint32::toString(std::numeric_limits<uint32_t>::max()));
        compare("18446744073709551615", uint64::toString(std::numeric_limits<uint64_t>::max()));
        
        // Test minimum values (should all be 0)
        compare("0", uint8::toString(std::numeric_limits<uint8_t>::min()));
        compare("0", uint16::toString(std::numeric_limits<uint16_t>::min()));
        compare("0", uint32::toString(std::numeric_limits<uint32_t>::min()));
        compare("0", uint64::toString(std::numeric_limits<uint64_t>::min()));
    }
    
    TEST(IntegerToStringPowerOfTwo, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test powers of 2
        compare("1", uint8::toString(1));
        compare("2", uint8::toString(2));
        compare("4", uint8::toString(4));
        compare("8", uint8::toString(8));
        compare("16", uint8::toString(16));
        compare("32", uint8::toString(32));
        compare("64", uint8::toString(64));
        compare("128", uint8::toString(128));
        
        // Test larger powers of 2
        compare("256", uint16::toString(256));
        compare("512", uint16::toString(512));
        compare("1024", uint16::toString(1024));
        compare("65536", uint32::toString(65536));
        compare("4294967296", uint64::toString(4294967296ULL));
    }
    
    // ========================================
    // Comprehensive Edge Cases
    // ========================================
    
    TEST(IntegerComprehensiveEdgeCases, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test all operations with zero
        compare(0ULL, static_cast<uint64_t>(uint8::lsb(0, 4)));
        compare(0ULL, static_cast<uint64_t>(uint8::msb(0, 4)));
        compare(0ULL, static_cast<uint64_t>(uint8::rotateLeft(0, 2)));
        compare(0ULL, static_cast<uint64_t>(uint8::rotateRight(0, 2, 8)));
        compare(false, uint8::getBitAtPosition(0, 0));
        compare(0x01ULL, static_cast<uint64_t>(uint8::setBitAtPosition(0, 0)));
        compare(0ULL, static_cast<uint64_t>(uint8::getByteAtPosition(0, 0)));
        compare(0ULL, static_cast<uint64_t>(uint8::bytesSwap(0)));
        compare("0", uint8::toString(0));
        
        // Test all operations with maximum values
        compare(0xFFULL, static_cast<uint64_t>(uint8::lsb(0xFF, 8)));
        compare(0xFFULL, static_cast<uint64_t>(uint8::msb(0xFF, 8)));
        compare(0xFFULL, static_cast<uint64_t>(uint8::rotateLeft(0xFF, 2)));
        compare(0xFFULL, static_cast<uint64_t>(uint8::rotateRight(0xFF, 2, 8)));
        compare(true, uint8::getBitAtPosition(0xFF, 0));
        compare(true, uint8::getBitAtPosition(0xFF, 7));
        compare(0xFFULL, static_cast<uint64_t>(uint8::setBitAtPosition(0xFF, 0)));
    }
    
    TEST(IntegerTypeConsistency, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test that different integer types behave consistently
        uint8_t val8 = 0x12;
        uint16_t val16 = 0x1234;
        uint32_t val32 = 0x12345678;
        uint64_t val64 = 0x123456789ABCDEF0;
        
        // Test LSB extraction
        compare(static_cast<uint64_t>(val8 & 0x0F), static_cast<uint64_t>(uint8::lsb(val8, 4)));
        compare(static_cast<uint64_t>(val16 & 0x0F), static_cast<uint64_t>(uint16::lsb(val16, 4)));
        compare(static_cast<uint64_t>(val32 & 0x0F), static_cast<uint64_t>(uint32::lsb(val32, 4)));
        compare(static_cast<uint64_t>(val64 & 0x0F), static_cast<uint64_t>(uint64::lsb(val64, 4)));
        
        // Test bit operations consistency
        compare(uint8::getBitAtPosition(val8, 0), (val8 & 1) != 0);
        compare(uint16::getBitAtPosition(val16, 0), (val16 & 1) != 0);
        compare(uint32::getBitAtPosition(val32, 0), (val32 & 1) != 0);
        compare(uint64::getBitAtPosition(val64, 0), (val64 & 1) != 0);
    }
    
    // ========================================
    // Cryptographic Scenario Tests
    // ========================================
    
    TEST(IntegerCryptographicScenarios, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test common cryptographic bit manipulations
        
        // Simulate DES-like operations
        uint32_t des_block = 0x12345678;
        uint32_t left_half = uint32::msb(des_block, 16);  // Upper 16 bits
        uint32_t right_half = uint32::lsb(des_block, 16); // Lower 16 bits
        compare(0x1234ULL, static_cast<uint64_t>(left_half));
        compare(0x5678ULL, static_cast<uint64_t>(right_half));
        
        // Simulate AES-like byte operations
        uint32_t aes_word = 0x12345678;
        compare(0x78ULL, static_cast<uint64_t>(uint32::getByteAtPosition(aes_word, 0)));
        compare(0x56ULL, static_cast<uint64_t>(uint32::getByteAtPosition(aes_word, 1)));
        compare(0x34ULL, static_cast<uint64_t>(uint32::getByteAtPosition(aes_word, 2)));
        compare(0x12ULL, static_cast<uint64_t>(uint32::getByteAtPosition(aes_word, 3)));
        
        // Test byte swapping for endianness conversion
        uint32_t little_endian = 0x12345678;
        uint32_t big_endian = uint32::bytesSwap(little_endian);
        compare(0x78563412ULL, static_cast<uint64_t>(big_endian));
        
        // Test bit rotations for stream ciphers
        uint32_t cipher_state = 0x80000001;
        uint32_t rotated = uint32::rotateLeft(cipher_state, 1);
        compare(0x00000003ULL, static_cast<uint64_t>(rotated)); // MSB wraps to LSB
    }
    
    TEST(IntegerBitPatternTests, IntegerTest)
    {
        using namespace CryptoGL;
        
        // Test alternating bit patterns
        uint8_t pattern_55 = 0x55; // 01010101
        uint8_t pattern_AA = 0xAA; // 10101010
        
        // Test LSB extraction on patterns
        compare(0x05ULL, static_cast<uint64_t>(uint8::lsb(pattern_55, 4))); // 0101
        compare(0x0AULL, static_cast<uint64_t>(uint8::lsb(pattern_AA, 4))); // 1010
        
        // Test MSB extraction on patterns
        compare(0x05ULL, static_cast<uint64_t>(uint8::msb(pattern_55, 4))); // 0101
        compare(0x0AULL, static_cast<uint64_t>(uint8::msb(pattern_AA, 4))); // 1010
        
        // Test rotation on patterns
        compare(static_cast<uint64_t>(pattern_AA), static_cast<uint64_t>(uint8::rotateLeft(pattern_55, 1)));
        compare(static_cast<uint64_t>(pattern_55), static_cast<uint64_t>(uint8::rotateRight(pattern_AA, 1, 8)));
    }
}
