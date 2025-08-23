#pragma once

#include "Test.hpp"
#include "../../src/core/types/Array.hpp"
#include <array>

namespace UnitTests
{
    class ArrayTest : public Test
    {
    protected:
        // Test arrays of different types and sizes
        Array<uint8_t, 4> uint8_array_4;
        Array<uint16_t, 3> uint16_array_3;
        Array<uint32_t, 2> uint32_array_2;
        Array<uint64_t, 1> uint64_array_1;
        Array<uint8_t, 0> empty_array;
        Array<uint8_t, 8> uint8_array_8;

        void setUp() override
        {
            // Initialize test arrays with known values
            uint8_array_4 = {0x01, 0x23, 0x45, 0x67};
            uint16_array_3 = {0x0123, 0x4567, 0x89AB};
            uint32_array_2 = {0x01234567, 0x89ABCDEF};
            uint64_array_1 = {0x0123456789ABCDEF};
            uint8_array_8 = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
        }

        void tearDown() override
        {
            // No dynamic allocation to clean up
        }
    };

    // Constructor Tests
    TEST(ArrayDefaultConstructor, ArrayTest)
    {
        const Array<uint8_t, 3> arr;
        compare(3, arr.size());
        
        // Check all elements are zero-initialized
        for (size_t i = 0; i < arr.size(); ++i) {
            compare(uint8_t(0), arr[i]);
        }
    }

    TEST(ArrayCopyConstructor, ArrayTest)
    {
        const Array<uint8_t, 4> original = {0x01, 0x02, 0x03, 0x04};
        const Array<uint8_t, 4> copy(original);
        
        compare(4, copy.size());
        for (size_t i = 0; i < copy.size(); ++i) {
            compare(original[i], copy[i]);
        }
    }

    TEST(ArrayInitializerListConstructor, ArrayTest)
    {
        const Array<uint8_t, 3> arr = {0xAA, 0xBB, 0xCC};
        compare(3, arr.size());
        compare(uint8_t(0xAA), arr[0]);
        compare(uint8_t(0xBB), arr[1]);
        compare(uint8_t(0xCC), arr[2]);
    }

    // Size and Access Tests
    TEST(ArraySize, ArrayTest)
    {
        compare(4, uint8_array_4.size());
        compare(3, uint16_array_3.size());
        compare(2, uint32_array_2.size());
        compare(1, uint64_array_1.size());
        compare(0, empty_array.size());
    }

    TEST(ArrayElementAccess, ArrayTest)
    {
        compare(uint8_t(0x01), uint8_array_4[0]);
        compare(uint8_t(0x23), uint8_array_4[1]);
        compare(uint8_t(0x45), uint8_array_4[2]);
        compare(uint8_t(0x67), uint8_array_4[3]);
    }

    TEST(ArrayElementModification, ArrayTest)
    {
        Array<uint8_t, 3> arr = {0x01, 0x02, 0x03};
        arr[1] = 0xFF;
        compare(uint8_t(0xFF), arr[1]);
        
        // Check other elements unchanged
        compare(uint8_t(0x01), arr[0]);
        compare(uint8_t(0x03), arr[2]);
    }

    // Iterator Tests
    TEST(ArrayIterators, ArrayTest)
    {
        Array<uint8_t, 3> arr = {0x01, 0x02, 0x03};
        
        // Test iterator values
        auto it = arr.begin();
        compare(uint8_t(0x01), *it);
        ++it;
        compare(uint8_t(0x02), *it);
        ++it;
        compare(uint8_t(0x03), *it);
        ++it;
        
        // Test iterator position using distance
        compare(3, static_cast<int>(std::distance(arr.begin(), it)));
        compare(3, arr.size());
    }

    // XOR Operation Tests
    TEST(ArrayXorOperation, ArrayTest)
    {
        const Array<uint8_t, 4> arr1 = {0x01, 0x02, 0x03, 0x04};
        const Array<uint8_t, 4> arr2 = {0x0F, 0x0F, 0x0F, 0x0F};
        
        const auto result = arr1.Xor(arr2);
        compare(uint8_t(0x0E), result[0]); // 0x01 ^ 0x0F = 0x0E
        compare(uint8_t(0x0D), result[1]); // 0x02 ^ 0x0F = 0x0D
        compare(uint8_t(0x0C), result[2]); // 0x03 ^ 0x0F = 0x0C
        compare(uint8_t(0x0B), result[3]); // 0x04 ^ 0x0F = 0x0B
    }

    TEST(ArrayXorWithZero, ArrayTest)
    {
        const Array<uint8_t, 3> arr1 = {0xAA, 0xBB, 0xCC};
        const Array<uint8_t, 3> arr2 = {0x00, 0x00, 0x00};
        
        const auto result = arr1.Xor(arr2);
        compare(uint8_t(0xAA), result[0]);
        compare(uint8_t(0xBB), result[1]);
        compare(uint8_t(0xCC), result[2]);
    }

    TEST(ArrayXorWithOnes, ArrayTest)
    {
        const Array<uint8_t, 2> arr1 = {0x55, 0xAA};
        const Array<uint8_t, 2> arr2 = {0xFF, 0xFF};
        
        const auto result = arr1.Xor(arr2);
        compare(uint8_t(0xAA), result[0]); // 0x55 ^ 0xFF = 0xAA
        compare(uint8_t(0x55), result[1]); // 0xAA ^ 0xFF = 0x55
    }

    // Extend Operation Tests
    TEST(ArrayExtend, ArrayTest)
    {
        const Array<uint8_t, 2> arr1 = {0x01, 0x02};
        const Array<uint8_t, 3> arr2 = {0x03, 0x04, 0x05};
        
        const auto result = arr1.Extend(arr2);
        compare(5, result.size());
        compare(uint8_t(0x01), result[0]);
        compare(uint8_t(0x02), result[1]);
        compare(uint8_t(0x03), result[2]);
        compare(uint8_t(0x04), result[3]);
        compare(uint8_t(0x05), result[4]);
    }

    TEST(ArrayExtendEmpty, ArrayTest)
    {
        const Array<uint8_t, 3> arr1 = {0x01, 0x02, 0x03};
        const Array<uint8_t, 0> arr2;
        
        const auto result = arr1.Extend(arr2);
        compare(3, result.size());
        compare(uint8_t(0x01), result[0]);
        compare(uint8_t(0x02), result[1]);
        compare(uint8_t(0x03), result[2]);
    }

    TEST(ArrayExtendToEmpty, ArrayTest)
    {
        const Array<uint8_t, 0> arr1;
        const Array<uint8_t, 2> arr2 = {0x01, 0x02};
        
        const auto result = arr1.Extend(arr2);
        compare(2, result.size());
        compare(uint8_t(0x01), result[0]);
        compare(uint8_t(0x02), result[1]);
    }

    // Find Operation Tests
    TEST(ArrayFindExisting, ArrayTest)
    {
        Array<uint8_t, 4> arr = {0x01, 0x23, 0x45, 0x67};
        
        compare(0, static_cast<int>(arr.Find(0x01)));
        compare(1, static_cast<int>(arr.Find(0x23)));
        compare(2, static_cast<int>(arr.Find(0x45)));
        compare(3, static_cast<int>(arr.Find(0x67)));
    }

    TEST(ArrayFindNonExisting, ArrayTest)
    {
        Array<uint8_t, 3> arr = {0x01, 0x02, 0x03};
        
        compare(-1, static_cast<int>(arr.Find(0x00)));
        compare(-1, static_cast<int>(arr.Find(0xFF)));
    }

    TEST(ArrayFindInEmpty, ArrayTest)
    {
        compare(-1, static_cast<int>(empty_array.Find(0x01)));
    }

    // Swap Operation Tests
    TEST(ArraySwap, ArrayTest)
    {
        Array<uint8_t, 4> arr = {0x01, 0x02, 0x03, 0x04};
        
        arr.Swap(1, 2);
        compare(uint8_t(0x01), arr[0]);
        compare(uint8_t(0x03), arr[1]);
        compare(uint8_t(0x02), arr[2]);
        compare(uint8_t(0x04), arr[3]);
    }

    TEST(ArraySwapSameIndex, ArrayTest)
    {
        Array<uint8_t, 3> arr = {0x01, 0x02, 0x03};
        Array<uint8_t, 3> original = arr;
        
        arr.Swap(1, 1); // Should not change anything
        
        for (size_t i = 0; i < arr.size(); ++i) {
            compare(original[i], arr[i]);
        }
    }

    // MSB/LSB Tests
    TEST(ArrayMSB, ArrayTest)
    {
        Array<uint8_t, 3> arr1 = {0x80, 0x00, 0x00}; // MSB set in first element
        Array<uint8_t, 3> arr2 = {0x00, 0x80, 0x00}; // MSB set in second element
        Array<uint8_t, 3> arr3 = {0x00, 0x00, 0x00}; // No MSB set
        
        compare(uint8_t(1), arr1.msb());
        compare(uint8_t(0), arr2.msb());
        compare(uint8_t(0), arr3.msb());
    }

    TEST(ArrayLSB, ArrayTest)
    {
        Array<uint8_t, 3> arr1 = {0x00, 0x00, 0x01}; // LSB set in last element
        Array<uint8_t, 3> arr2 = {0x01, 0x00, 0x00}; // LSB set in first element
        Array<uint8_t, 3> arr3 = {0x00, 0x00, 0x00}; // No LSB set
        
        compare(uint8_t(1), arr1.lsb());
        compare(uint8_t(0), arr2.lsb());
        compare(uint8_t(0), arr3.lsb());
    }

    // Shift Operation Tests
    TEST(ArrayLeftShift, ArrayTest)
    {
        const Array<uint8_t, 2> arr = {0x12, 0x34};
        
        const auto result = arr.leftShift(4);
        compare(uint8_t(0x23), result[0]);
        compare(uint8_t(0x40), result[1]);
    }

    TEST(ArrayRightShift, ArrayTest)
    {
        const Array<uint8_t, 2> arr = {0x12, 0x34};
        
        const auto result = arr.rightShift(4);
        compare(uint8_t(0x01), result[0]);
        compare(uint8_t(0x23), result[1]);
    }

    TEST(ArrayShiftByZero, ArrayTest)
    {
        const Array<uint8_t, 2> arr = {0x12, 0x34};
        
        const auto left_result = arr.leftShift(0);
        const auto right_result = arr.rightShift(0);
        
        for (size_t i = 0; i < arr.size(); ++i) {
            compare(arr[i], left_result[i]);
            compare(arr[i], right_result[i]);
        }
    }

    // Chunk Operation Tests
    TEST(ArrayChunk, ArrayTest)
    {
        const Array<uint8_t, 8> arr = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
        
        const auto chunks = arr.template chunk<2>();
        compare(4, chunks.size());
        
        // Check first chunk
        compare(uint8_t(0x01), chunks[0][0]);
        compare(uint8_t(0x02), chunks[0][1]);
        
        // Check last chunk
        compare(uint8_t(0x07), chunks[3][0]);
        compare(uint8_t(0x08), chunks[3][1]);
    }

    TEST(ArrayChunkSingleElement, ArrayTest)
    {
        const Array<uint8_t, 4> arr = {0x01, 0x02, 0x03, 0x04};
        
        const auto chunks = arr.template chunk<4>();
        compare(1, chunks.size());
        compare(uint8_t(0x01), chunks[0][0]);
        compare(uint8_t(0x04), chunks[0][3]);
    }

    // Hex String Tests
    TEST(ArrayToHexString, ArrayTest)
    {
        const Array<uint8_t, 4> arr = {0x01, 0x23, 0x45, 0x67};
        
        const String result = arr.toHexString();
        compare("01234567", result);
    }

    TEST(ArrayToHexStringEmpty, ArrayTest)
    {
        const String result = empty_array.toHexString();
        compare("", result);
    }

    TEST(ArrayToHexStringLargeValues, ArrayTest)
    {
        const Array<uint32_t, 2> arr = {0x12345678, 0x9ABCDEF0};
        
        const String result = arr.toHexString();
        compare("123456789ABCDEF0", result);
    }

    // Edge Cases and Error Conditions
    TEST(ArrayOutOfBoundsAccess, ArrayTest)
    {
        // Note: This test documents expected behavior
        // Array with std::array should throw std::out_of_range for invalid access
        // but this depends on your implementation
        const Array<uint8_t, 2> arr = {0x01, 0x02};
        
        // Valid access
        compare(uint8_t(0x01), arr[0]);
        compare(uint8_t(0x02), arr[1]);
        
        // Invalid access would cause undefined behavior with std::array
        // This test documents the current behavior
    }

    TEST(ArrayConstCorrectness, ArrayTest)
    {
        const Array<uint8_t, 3> arr = {0x01, 0x02, 0x03};
        
        // Should be able to access elements
        compare(uint8_t(0x01), arr[0]);
        compare(uint8_t(0x02), arr[1]);
        compare(uint8_t(0x03), arr[2]);
        
        // Should be able to get size
        compare(3, arr.size());
        
        // Should be able to use iterators
        auto it = arr.begin();
        compare(uint8_t(0x01), *it);
    }

    // Performance and Memory Tests
    TEST(ArrayMemoryLayout, ArrayTest)
    {
        // Test that arrays are properly packed
        const Array<uint8_t, 4> arr = {0x01, 0x02, 0x03, 0x04};
        
        // Check that elements are contiguous in memory
        const uint8_t* ptr = &arr[0];
        compare(uint8_t(0x01), ptr[0]);
        compare(uint8_t(0x02), ptr[1]);
        compare(uint8_t(0x03), ptr[2]);
        compare(uint8_t(0x04), ptr[3]);
    }

    // Template Specialization Tests
    TEST(ArrayDifferentTypes, ArrayTest)
    {
        const Array<uint8_t, 2> uint8_arr = {0x01, 0x02};
        const Array<uint16_t, 2> uint16_arr = {0x0102, 0x0304};
        const Array<uint32_t, 2> uint32_arr = {0x01020304, 0x05060708};
        
        compare(2, uint8_arr.size());
        compare(2, uint16_arr.size());
        compare(2, uint32_arr.size());
        
        compare(uint8_t(0x01), uint8_arr[0]);
        compare(uint16_t(0x0102), uint16_arr[0]);
        compare(uint32_t(0x01020304), uint32_arr[0]);
    }

    // Cryptographic Operation Tests
    TEST(ArrayCryptographicOperations, ArrayTest)
    {
        // Test XOR with cryptographic-like data
        Array<uint8_t, 16> key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                  0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
        Array<uint8_t, 16> plaintext = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        
        auto ciphertext = key.Xor(plaintext);
        
        // Ciphertext should equal key when XORed with zero plaintext
        for (size_t i = 0; i < key.size(); ++i) {
            compare(key[i], ciphertext[i]);
        }
    }
}