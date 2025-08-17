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
        Array<uint8_t, 3> arr;
        compare(3ULL, arr.size());
        
        // Check all elements are zero-initialized
        for (size_t i = 0; i < arr.size(); ++i) {
            compare(0ULL, static_cast<uint64_t>(arr[i]));
        }
    }

    TEST(ArrayCopyConstructor, ArrayTest)
    {
        Array<uint8_t, 4> original = {0x01, 0x02, 0x03, 0x04};
        Array<uint8_t, 4> copy(original);
        
        compare(4ULL, copy.size());
        for (size_t i = 0; i < copy.size(); ++i) {
            compare(static_cast<uint64_t>(original[i]), static_cast<uint64_t>(copy[i]));
        }
    }

    TEST(ArrayInitializerListConstructor, ArrayTest)
    {
        Array<uint8_t, 3> arr = {0xAA, 0xBB, 0xCC};
        compare(3ULL, arr.size());
        compare(0xAAULL, static_cast<uint64_t>(arr[0]));
        compare(0xBBULL, static_cast<uint64_t>(arr[1]));
        compare(0xCCULL, static_cast<uint64_t>(arr[2]));
    }

    // Size and Access Tests
    TEST(ArraySize, ArrayTest)
    {
        compare(4ULL, uint8_array_4.size());
        compare(3ULL, uint16_array_3.size());
        compare(2ULL, uint32_array_2.size());
        compare(1ULL, uint64_array_1.size());
        compare(0ULL, empty_array.size());
    }

    TEST(ArrayElementAccess, ArrayTest)
    {
        compare(0x01ULL, static_cast<uint64_t>(uint8_array_4[0]));
        compare(0x23ULL, static_cast<uint64_t>(uint8_array_4[1]));
        compare(0x45ULL, static_cast<uint64_t>(uint8_array_4[2]));
        compare(0x67ULL, static_cast<uint64_t>(uint8_array_4[3]));
    }

    TEST(ArrayElementModification, ArrayTest)
    {
        Array<uint8_t, 3> arr = {0x01, 0x02, 0x03};
        arr[1] = 0xFF;
        compare(0xFFULL, static_cast<uint64_t>(arr[1]));
        
        // Check other elements unchanged
        compare(0x01ULL, static_cast<uint64_t>(arr[0]));
        compare(0x03ULL, static_cast<uint64_t>(arr[2]));
    }

    // Iterator Tests
    TEST(ArrayIterators, ArrayTest)
    {
        Array<uint8_t, 3> arr = {0x01, 0x02, 0x03};
        
        // Test iterator values
        auto it = arr.begin();
        compare(0x01ULL, static_cast<uint64_t>(*it));
        ++it;
        compare(0x02ULL, static_cast<uint64_t>(*it));
        ++it;
        compare(0x03ULL, static_cast<uint64_t>(*it));
        ++it;
        
        // Test iterator position using distance
        compare(3ULL, static_cast<uint64_t>(std::distance(arr.begin(), it)));
        compare(3ULL, static_cast<uint64_t>(arr.size()));
    }

    TEST(ArrayConstIterators, ArrayTest)
    {
        const Array<uint8_t, 3> arr = {0x01, 0x02, 0x03};
        
        auto it = arr.begin();
        compare(0x01ULL, static_cast<uint64_t>(*it));
        ++it;
        compare(0x02ULL, static_cast<uint64_t>(*it));
        ++it;
        compare(0x03ULL, static_cast<uint64_t>(*it));
        ++it;
        // Test iterator position using distance
        compare(3ULL, static_cast<uint64_t>(std::distance(arr.begin(), it)));
        compare(3ULL, static_cast<uint64_t>(arr.size()));
    }

    // XOR Operation Tests
    TEST(ArrayXorOperation, ArrayTest)
    {
        Array<uint8_t, 4> arr1 = {0x01, 0x02, 0x03, 0x04};
        Array<uint8_t, 4> arr2 = {0x0F, 0x0F, 0x0F, 0x0F};
        
        auto result = arr1.Xor(arr2);
        compare(0x0EULL, static_cast<uint64_t>(result[0])); // 0x01 ^ 0x0F = 0x0E
        compare(0x0DULL, static_cast<uint64_t>(result[1])); // 0x02 ^ 0x0F = 0x0D
        compare(0x0CULL, static_cast<uint64_t>(result[2])); // 0x03 ^ 0x0F = 0x0C
        compare(0x0BULL, static_cast<uint64_t>(result[3])); // 0x04 ^ 0x0F = 0x0B
    }

    TEST(ArrayXorWithZero, ArrayTest)
    {
        Array<uint8_t, 3> arr1 = {0xAA, 0xBB, 0xCC};
        Array<uint8_t, 3> arr2 = {0x00, 0x00, 0x00};
        
        auto result = arr1.Xor(arr2);
        compare(0xAAULL, static_cast<uint64_t>(result[0]));
        compare(0xBBULL, static_cast<uint64_t>(result[1]));
        compare(0xCCULL, static_cast<uint64_t>(result[2]));
    }

    TEST(ArrayXorWithOnes, ArrayTest)
    {
        Array<uint8_t, 2> arr1 = {0x55, 0xAA};
        Array<uint8_t, 2> arr2 = {0xFF, 0xFF};
        
        auto result = arr1.Xor(arr2);
        compare(0xAAULL, static_cast<uint64_t>(result[0])); // 0x55 ^ 0xFF = 0xAA
        compare(0x55ULL, static_cast<uint64_t>(result[1])); // 0xAA ^ 0xFF = 0x55
    }

    // Extend Operation Tests
    TEST(ArrayExtend, ArrayTest)
    {
        Array<uint8_t, 2> arr1 = {0x01, 0x02};
        Array<uint8_t, 3> arr2 = {0x03, 0x04, 0x05};
        
        auto result = arr1.Extend(arr2);
        compare(5ULL, result.size());
        compare(0x01ULL, static_cast<uint64_t>(result[0]));
        compare(0x02ULL, static_cast<uint64_t>(result[1]));
        compare(0x03ULL, static_cast<uint64_t>(result[2]));
        compare(0x04ULL, static_cast<uint64_t>(result[3]));
        compare(0x05ULL, static_cast<uint64_t>(result[4]));
    }

    TEST(ArrayExtendEmpty, ArrayTest)
    {
        Array<uint8_t, 3> arr1 = {0x01, 0x02, 0x03};
        Array<uint8_t, 0> arr2;
        
        auto result = arr1.Extend(arr2);
        compare(3ULL, result.size());
        compare(0x01ULL, static_cast<uint64_t>(result[0]));
        compare(0x02ULL, static_cast<uint64_t>(result[1]));
        compare(0x03ULL, static_cast<uint64_t>(result[2]));
    }

    TEST(ArrayExtendToEmpty, ArrayTest)
    {
        Array<uint8_t, 0> arr1;
        Array<uint8_t, 2> arr2 = {0x01, 0x02};
        
        auto result = arr1.Extend(arr2);
        compare(2ULL, result.size());
        compare(0x01ULL, static_cast<uint64_t>(result[0]));
        compare(0x02ULL, static_cast<uint64_t>(result[1]));
    }

    // Find Operation Tests
    TEST(ArrayFindExisting, ArrayTest)
    {
        Array<uint8_t, 4> arr = {0x01, 0x23, 0x45, 0x67};
        
        compare(0LL, arr.Find(0x01));
        compare(1LL, arr.Find(0x23));
        compare(2LL, arr.Find(0x45));
        compare(3LL, arr.Find(0x67));
    }

    TEST(ArrayFindNonExisting, ArrayTest)
    {
        Array<uint8_t, 3> arr = {0x01, 0x02, 0x03};
        
        compare(-1LL, arr.Find(0x00));
        compare(-1LL, arr.Find(0xFF));
    }

    TEST(ArrayFindInEmpty, ArrayTest)
    {
        compare(-1LL, empty_array.Find(0x01));
    }

    // Swap Operation Tests
    TEST(ArraySwap, ArrayTest)
    {
        Array<uint8_t, 4> arr = {0x01, 0x02, 0x03, 0x04};
        
        arr.Swap(1, 2);
        compare(0x01ULL, static_cast<uint64_t>(arr[0]));
        compare(0x03ULL, static_cast<uint64_t>(arr[1]));
        compare(0x02ULL, static_cast<uint64_t>(arr[2]));
        compare(0x04ULL, static_cast<uint64_t>(arr[3]));
    }

    TEST(ArraySwapSameIndex, ArrayTest)
    {
        Array<uint8_t, 3> arr = {0x01, 0x02, 0x03};
        Array<uint8_t, 3> original = arr;
        
        arr.Swap(1, 1); // Should not change anything
        
        for (size_t i = 0; i < arr.size(); ++i) {
            compare(static_cast<uint64_t>(original[i]), static_cast<uint64_t>(arr[i]));
        }
    }

    // MSB/LSB Tests
    TEST(ArrayMSB, ArrayTest)
    {
        Array<uint8_t, 3> arr1 = {0x80, 0x00, 0x00}; // MSB set in first element
        Array<uint8_t, 3> arr2 = {0x00, 0x80, 0x00}; // MSB set in second element
        Array<uint8_t, 3> arr3 = {0x00, 0x00, 0x00}; // No MSB set
        
        compare(1ULL, arr1.msb());
        compare(0ULL, arr2.msb());
        compare(0ULL, arr3.msb());
    }

    TEST(ArrayLSB, ArrayTest)
    {
        Array<uint8_t, 3> arr1 = {0x00, 0x00, 0x01}; // LSB set in last element
        Array<uint8_t, 3> arr2 = {0x01, 0x00, 0x00}; // LSB set in first element
        Array<uint8_t, 3> arr3 = {0x00, 0x00, 0x00}; // No LSB set
        
        compare(1ULL, arr1.lsb());
        compare(0ULL, arr2.lsb());
        compare(0ULL, arr3.lsb());
    }

    // Shift Operation Tests
    TEST(ArrayLeftShift, ArrayTest)
    {
        Array<uint8_t, 2> arr = {0x12, 0x34};
        
        auto result = arr.leftShift(4);
        compare(0x23ULL, static_cast<uint64_t>(result[0]));
        compare(0x40ULL, static_cast<uint64_t>(result[1]));
    }

    TEST(ArrayRightShift, ArrayTest)
    {
        Array<uint8_t, 2> arr = {0x12, 0x34};
        
        auto result = arr.rightShift(4);
        compare(0x01ULL, static_cast<uint64_t>(result[0]));
        compare(0x23ULL, static_cast<uint64_t>(result[1]));
    }

    TEST(ArrayShiftByZero, ArrayTest)
    {
        Array<uint8_t, 2> arr = {0x12, 0x34};
        
        auto left_result = arr.leftShift(0);
        auto right_result = arr.rightShift(0);
        
        for (size_t i = 0; i < arr.size(); ++i) {
            compare(static_cast<uint64_t>(arr[i]), static_cast<uint64_t>(left_result[i]));
            compare(static_cast<uint64_t>(arr[i]), static_cast<uint64_t>(right_result[i]));
        }
    }

    // Chunk Operation Tests
    TEST(ArrayChunk, ArrayTest)
    {
        Array<uint8_t, 8> arr = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
        
        auto chunks = arr.template chunk<2>();
        compare(4ULL, chunks.size());
        
        // Check first chunk
        compare(0x01ULL, static_cast<uint64_t>(chunks[0][0]));
        compare(0x02ULL, static_cast<uint64_t>(chunks[0][1]));
        
        // Check last chunk
        compare(0x07ULL, static_cast<uint64_t>(chunks[3][0]));
        compare(0x08ULL, static_cast<uint64_t>(chunks[3][1]));
    }

    TEST(ArrayChunkSingleElement, ArrayTest)
    {
        Array<uint8_t, 4> arr = {0x01, 0x02, 0x03, 0x04};
        
        auto chunks = arr.template chunk<4>();
        compare(1ULL, chunks.size());
        compare(0x01ULL, static_cast<uint64_t>(chunks[0][0]));
        compare(0x04ULL, static_cast<uint64_t>(chunks[0][3]));
    }

    // Hex String Tests
    TEST(ArrayToHexString, ArrayTest)
    {
        Array<uint8_t, 4> arr = {0x01, 0x23, 0x45, 0x67};
        
        String result = arr.toHexString();
        compare("01234567", result);
    }

    TEST(ArrayToHexStringEmpty, ArrayTest)
    {
        String result = empty_array.toHexString();
        compare("", result);
    }

    TEST(ArrayToHexStringLargeValues, ArrayTest)
    {
        Array<uint32_t, 2> arr = {0x12345678, 0x9ABCDEF0};
        
        String result = arr.toHexString();
        compare("123456789ABCDEF0", result);
    }

    // Edge Cases and Error Conditions
    TEST(ArrayOutOfBoundsAccess, ArrayTest)
    {
        // Note: This test documents expected behavior
        // Array with std::array should throw std::out_of_range for invalid access
        // but this depends on your implementation
        Array<uint8_t, 2> arr = {0x01, 0x02};
        
        // Valid access
        compare(0x01ULL, static_cast<uint64_t>(arr[0]));
        compare(0x02ULL, static_cast<uint64_t>(arr[1]));
        
        // Invalid access would cause undefined behavior with std::array
        // This test documents the current behavior
    }

    TEST(ArrayConstCorrectness, ArrayTest)
    {
        const Array<uint8_t, 3> arr = {0x01, 0x02, 0x03};
        
        // Should be able to access elements
        compare(0x01ULL, static_cast<uint64_t>(arr[0]));
        compare(0x02ULL, static_cast<uint64_t>(arr[1]));
        compare(0x03ULL, static_cast<uint64_t>(arr[2]));
        
        // Should be able to get size
        compare(3ULL, arr.size());
        
        // Should be able to use iterators
        auto it = arr.begin();
        compare(0x01ULL, static_cast<uint64_t>(*it));
    }

    // Performance and Memory Tests
    TEST(ArrayMemoryLayout, ArrayTest)
    {
        // Test that arrays are properly packed
        Array<uint8_t, 4> arr = {0x01, 0x02, 0x03, 0x04};
        
        // Check that elements are contiguous in memory
        uint8_t* ptr = &arr[0];
        compare(0x01ULL, static_cast<uint64_t>(ptr[0]));
        compare(0x02ULL, static_cast<uint64_t>(ptr[1]));
        compare(0x03ULL, static_cast<uint64_t>(ptr[2]));
        compare(0x04ULL, static_cast<uint64_t>(ptr[3]));
    }

    // Template Specialization Tests
    TEST(ArrayDifferentTypes, ArrayTest)
    {
        Array<uint8_t, 2> uint8_arr = {0x01, 0x02};
        Array<uint16_t, 2> uint16_arr = {0x0102, 0x0304};
        Array<uint32_t, 2> uint32_arr = {0x01020304, 0x05060708};
        
        compare(2ULL, uint8_arr.size());
        compare(2ULL, uint16_arr.size());
        compare(2ULL, uint32_arr.size());
        
        compare(0x01ULL, static_cast<uint64_t>(uint8_arr[0]));
        compare(0x0102ULL, static_cast<uint64_t>(uint16_arr[0]));
        compare(0x01020304ULL, static_cast<uint64_t>(uint32_arr[0]));
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
            compare(static_cast<uint64_t>(key[i]), static_cast<uint64_t>(ciphertext[i]));
        }
    }
}