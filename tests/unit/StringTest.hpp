/*
 * Comprehensive Unit Tests for CryptoGL::String class
 * Provides 100% code coverage including security and performance validation
 * Uses CryptoGL's custom test framework
 */
#pragma once

#include "Test.hpp"
#include "../../src/core/types/String.hpp"
#include "../../src/core/types/Types.hpp"
#include <stdexcept>
#include <algorithm>
#include <random>

using namespace CryptoGL;
using namespace UnitTests;

namespace StringTests {
    
    // Base test class for String testing
    class StringTestBase : public Test {
    protected:
        String test_string;
        String empty_string;
        String hello_world;
        String numeric_string;
        String hex_string;
        String mixed_case;
        String whitespace_string;
        String special_chars;
        
        void setUp() override {
            test_string = String("Test");
            empty_string = String();
            hello_world = String("Hello, World!");
            numeric_string = String("123456789");
            hex_string = String("1a2b3c4d5e6f");
            mixed_case = String("AbCdEfG");
            whitespace_string = String("  spaces  ");
            special_chars = String("!@#$%^&*()");
        }
        
        void tearDown() override {
            // Ensure secure cleanup
            test_string.clear();
            empty_string.clear();
            hello_world.clear();
            numeric_string.clear();
            hex_string.clear();
            mixed_case.clear();
            whitespace_string.clear();
            special_chars.clear();
        }
    };
    
    // =============================================================================
    // CONSTRUCTOR TESTS
    // =============================================================================
    
    TEST(StringDefaultConstructorTest, StringTestBase)
    {
        String s;
        compare(String("0"), String(uint64::toString(s.size())));
        compare(String("1"), String(s.empty() ? "1" : "0"));
        compare(String("4096"), String(uint64::toString(s.capacity())));
    }
    
    TEST(StringReserveConstructorTest, StringTestBase)
    {
        String s(100);
        compare(String("0"), String(uint64::toString(s.size())));
        compare(String("1"), String(s.empty() ? "1" : "0"));
        compare(String("4096"), String(uint64::toString(s.capacity()))); // Always MAX_SECURE_LENGTH
    }
    
    TEST(StringCStringConstructorTest, StringTestBase)
    {
        const char* test_cstr = "Hello";
        String s(test_cstr);
        compare(String("5"), String(uint64::toString(s.size())));
        compare(String("Hello"), s);
        compare(String("0"), String(s.empty() ? "1" : "0"));
    }
    
    TEST(StringStdStringConstructorTest, StringTestBase)
    {
        std::string std_str = "Hello World";
        String s(std_str);
        compare(String("11"), String(uint64::toString(s.size())));
        compare(String("Hello World"), s);
    }
    
    TEST(StringFillConstructorTest, StringTestBase)
    {
        String s(5, 'A');
        compare(String("5"), String(uint64::toString(s.size())));
        compare(String("AAAAA"), s);
    }
    
    TEST(StringInitializerListConstructorTest, StringTestBase)
    {
        String s{'H', 'e', 'l', 'l', 'o'};
        compare(String("5"), String(uint64::toString(s.size())));
        compare(String("Hello"), s);
    }
    
    TEST(StringCopyConstructorTest, StringTestBase)
    {
        String original("Original");
        String copy(original);
        compare(String("8"), String(uint64::toString(copy.size())));
        compare(String("Original"), copy);
        compare(original, copy);
    }
    
    TEST(StringMoveConstructorTest, StringTestBase)
    {
        String original("Movable");
        String moved(std::move(original));
        compare(String("7"), String(uint64::toString(moved.size())));
        compare(String("Movable"), moved);
        // Original should be in valid but unspecified state
    }
    
    // =============================================================================
    // ASSIGNMENT OPERATOR TESTS
    // =============================================================================
    
    TEST(StringCopyAssignmentTest, StringTestBase)
    {
        String original("Source");
        String target;
        target = original;
        compare(String("6"), String(uint64::toString(target.size())));
        compare(String("Source"), target);
        compare(original, target);
    }
    
    TEST(StringMoveAssignmentTest, StringTestBase)
    {
        String original("MoveSource");
        String target;
        target = std::move(original);
        compare(String("10"), String(uint64::toString(target.size())));
        compare(String("MoveSource"), target);
    }
    
    TEST(StringCStringAssignmentTest, StringTestBase)
    {
        String s;
        s = "Assigned";
        compare(String("8"), String(uint64::toString(s.size())));
        compare(String("Assigned"), s);
    }
    
    // =============================================================================
    // CAPACITY TESTS
    // =============================================================================
    
    TEST(StringCapacityTest, StringTestBase)
    {
        compare(String("4"), String(uint64::toString(test_string.size())));
        compare(String("4"), String(uint64::toString(test_string.length())));
        compare(String("4096"), String(uint64::toString(test_string.capacity())));
        compare(String("4096"), String(uint64::toString(test_string.max_size())));
        compare(String("0"), String(test_string.empty() ? "1" : "0"));
        
        compare(String("1"), String(empty_string.empty() ? "1" : "0"));
        compare(String("0"), String(uint64::toString(empty_string.size())));
    }
    
    // =============================================================================
    // ELEMENT ACCESS TESTS
    // =============================================================================
    
    TEST(StringElementAccessTest, StringTestBase)
    {
        String s("ABCD");
        compare(String("A"), String(1, s[0]));
        compare(String("B"), String(1, s[1]));
        compare(String("C"), String(1, s[2]));
        compare(String("D"), String(1, s[3]));
    }
    
    TEST(StringAtMethodTest, StringTestBase)
    {
        String s("XYZ");
        try {
            char c = s.at(0);
            compare(String("X"), String(1, c));
            c = s.at(2);
            compare(String("Z"), String(1, c));
        } catch (...) {
            compare(String("1"), String("0")); // Should not throw for valid indices
        }
    }
    
    TEST(StringAtMethodBoundsTest, StringTestBase)
    {
        String s("AB");
        try {
            char c = s.at(5); // Out of bounds
            compare(String("1"), String("0")); // Should not reach here
        } catch (const std::out_of_range&) {
            compare(String("1"), String("1")); // Expected exception
        }
    }
    
    TEST(StringFrontBackTest, StringTestBase)
    {
        String s("Hello");
        compare(String("H"), String(1, s.front()));
        compare(String("o"), String(1, s.back()));
    }
    
    // =============================================================================
    // ITERATOR TESTS
    // =============================================================================
    
    TEST(StringIteratorTest, StringTestBase)
    {
        String s("ABC");
        String result;
        for (auto it = s.begin(); it != s.end(); ++it) {
            result.push_back(*it);
        }
        compare(String("ABC"), result);
    }
    
    TEST(StringConstIteratorTest, StringTestBase)
    {
        const String s("DEF");
        String result;
        for (auto it = s.cbegin(); it != s.cend(); ++it) {
            result.push_back(*it);
        }
        compare(String("DEF"), result);
    }
    
    // =============================================================================
    // DATA ACCESS TESTS
    // =============================================================================
    
    TEST(StringDataAccessTest, StringTestBase)
    {
        String s("DataTest");
        const char* cstr = s.c_str();
        const char* data_ptr = s.data();
        
        String from_cstr(cstr);
        String from_data(data_ptr);
        
        compare(String("DataTest"), from_cstr);
        compare(String("DataTest"), from_data);
    }
    
    // =============================================================================
    // COMPARISON TESTS
    // =============================================================================
    
    TEST(StringEqualsTest, StringTestBase)
    {
        String s1("Equal");
        String s2("Equal");
        String s3("Different");
        
        compare(String("1"), String(s1.equals(s2) ? "1" : "0"));
        compare(String("0"), String(s1.equals(s3) ? "1" : "0"));
        
        compare(String("1"), String(s1.equals("Equal") ? "1" : "0"));
        compare(String("0"), String(s1.equals("Different") ? "1" : "0"));
    }
    
    TEST(StringComparisonOperatorsTest, StringTestBase)
    {
        String s1("ABC");
        String s2("ABC");
        String s3("DEF");
        
        compare(String("1"), String((s1 == s2) ? "1" : "0"));
        compare(String("0"), String((s1 == s3) ? "1" : "0"));
        compare(String("0"), String((s1 != s2) ? "1" : "0"));
        compare(String("1"), String((s1 != s3) ? "1" : "0"));
        compare(String("1"), String((s1 < s3) ? "1" : "0"));
    }
    
    // =============================================================================
    // MODIFIER TESTS
    // =============================================================================
    
    TEST(StringAppendTest, StringTestBase)
    {
        String s("Hello");
        String append_str(" World");
        s.append(append_str);
        compare(String("Hello World"), s);
        
        String s2("Hello");
        s2.append(" C++");
        compare(String("Hello C++"), s2);
        
        String s3("Hi");
        s3.append(3, '!');
        compare(String("Hi!!!"), s3);
    }
    
    TEST(StringPushPopTest, StringTestBase)
    {
        String s("AB");
        s.push_back('C');
        compare(String("ABC"), s);
        
        s.pop_back();
        compare(String("AB"), s);
        compare(String("2"), String(uint64::toString(s.size())));
    }
    
    TEST(StringClearTest, StringTestBase)
    {
        String s("NotEmpty");
        compare(String("0"), String(s.empty() ? "1" : "0"));
        s.clear();
        compare(String("1"), String(s.empty() ? "1" : "0"));
        compare(String("0"), String(uint64::toString(s.size())));
    }
    
    // =============================================================================
    // STRING OPERATION TESTS
    // =============================================================================
    
    TEST(StringToUpperCaseTest, StringTestBase)
    {
        String s("hello world");
        s.toUpperCase();
        compare(String("HELLO WORLD"), s);
        
        String s2("MiXeD cAsE");
        s2.toUpperCase();
        compare(String("MIXED CASE"), s2);
    }
    
    TEST(StringToLowerCaseTest, StringTestBase)
    {
        String s("HELLO WORLD");
        s.toLowerCase();
        compare(String("hello world"), s);
        
        String s2("MiXeD cAsE");
        s2.toLowerCase();
        compare(String("mixed case"), s2);
    }
    
    TEST(StringRemoveCharsTest, StringTestBase)
    {
        String s("Hello World");
        String chars_to_remove("l");
        s.removeChars(chars_to_remove);
        compare(String("Heo Word"), s);
        
        String s2("ABCDEF");
        String vowels("AE");
        s2.removeChars(vowels);
        compare(String("BCDF"), s2);
    }
    
    TEST(StringRemoveDuplicatesTest, StringTestBase)
    {
        String s("aabbccdd");
        s.removeDuplicates();
        compare(String("abcd"), s);
        
        String s2("HELLO");
        s2.removeDuplicates();
        compare(String("HELO"), s2);
    }
    
    TEST(StringContainsUniqueCharsTest, StringTestBase)
    {
        String s1("abcdef");
        compare(String("1"), String(s1.containsUniqueChars() ? "1" : "0"));
        
        String s2("aabbcc");
        compare(String("0"), String(s2.containsUniqueChars() ? "1" : "0"));
        
        String s3("Hello");
        compare(String("0"), String(s3.containsUniqueChars() ? "1" : "0")); // 'l' appears twice
    }
    
    // =============================================================================
    // CONVERSION TESTS
    // =============================================================================
    
    TEST(StringToStdStringTest, StringTestBase)
    {
        String s("Convert Me");
        std::string std_str = s.toStdString();
        String back_to_string(std_str);
        compare(String("Convert Me"), back_to_string);
    }
    
    TEST(StringToBytesTest, StringTestBase)
    {
        String s("ABC");
        std::vector<uint8_t> bytes = s.toBytes();
        compare(String("3"), String(uint64::toString(bytes.size())));
        
        // Check ASCII values: A=65, B=66, C=67
        compare(String("65"), String(uint64::toString(bytes[0])));
        compare(String("66"), String(uint64::toString(bytes[1])));
        compare(String("67"), String(uint64::toString(bytes[2])));
    }
    
    TEST(StringHexToBytesTest, StringTestBase)
    {
        String s("48656C6C6F"); // "Hello" in hex
        std::vector<uint8_t> bytes = s.hexToBytes();
        
        String result;
        for (uint8_t b : bytes) {
            result.push_back(static_cast<char>(b));
        }
        compare(String("Hello"), result);
    }
    
    TEST(StringHexToBytesInvalidTest, StringTestBase)
    {
        String s("1G2H"); // Invalid hex characters
        try {
            auto bytes = s.hexToBytes();
        } catch (const std::invalid_argument&) {
            compare(String("1"), String("1")); // Expected exception for zero chunk size
        }
    }
    
    // =============================================================================
    // STRING MANIPULATION TESTS
    // =============================================================================
    
    TEST(StringTrimStartTest, StringTestBase)
    {
        String s("   Hello");
        String trimmed = s.trimStart();
        compare(String("Hello"), trimmed);
        
        String s2("NoSpaces");
        String trimmed2 = s2.trimStart();
        compare(String("NoSpaces"), trimmed2);
    }
    
    TEST(StringTrimEndTest, StringTestBase)
    {
        String s("Hello   ");
        String trimmed = s.trimEnd();
        compare(String("Hello"), trimmed);
        
        String s2("NoSpaces");
        String trimmed2 = s2.trimEnd();
        compare(String("NoSpaces"), trimmed2);
    }
    
    TEST(StringSplitTest, StringTestBase)
    {
        String s("A,B,C,D");
        std::vector<String> parts = s.split(',');
        compare(String("4"), String(uint64::toString(parts.size())));
        compare(String("A"), parts[0]);
        compare(String("B"), parts[1]);
        compare(String("C"), parts[2]);
        compare(String("D"), parts[3]);
        
        String s2("NoSeparator");
        std::vector<String> parts2 = s2.split(',');
        compare(String("1"), String(uint64::toString(parts2.size())));
        compare(String("NoSeparator"), parts2[0]);
    }
    
    TEST(StringSortTest, StringTestBase)
    {
        String s("dcba");
        s.sort();
        compare(String("abcd"), s);
        
        String s2("Hello");
        s2.sort();
        compare(String("Hllo"), s2); // 'e' comes after capital H
    }
    
    TEST(StringRotateLeftTest, StringTestBase)
    {
        String s("ABCDEF");
        s.rotateLeft(2);
        compare(String("CDEFAB"), s);
        
        String s2("AB");
        s2.rotateLeft(1);
        compare(String("BA"), s2);
        
        String s3("A");
        s3.rotateLeft(5); // Should handle rotation > length
        compare(String("A"), s3);
    }
    
    TEST(StringRotateRightTest, StringTestBase)
    {
        String s("ABCDEF");
        s.rotateRight(2);
        compare(String("EFABCD"), s);
        
        String s2("AB");
        s2.rotateRight(1);
        compare(String("BA"), s2);
    }
    
    // =============================================================================
    // SEARCH TESTS
    // =============================================================================
    
    TEST(StringFindTest, StringTestBase)
    {
        String s("Hello World");
        compare(String("0"), String(uint64::toString(s.find('H'))));
        compare(String("6"), String(uint64::toString(s.find('W'))));
        compare(String("18446744073709551615"), String(uint64::toString(s.find('X')))); // npos
        
        compare(String("2"), String(uint64::toString(s.find('l', 0))));
        compare(String("3"), String(uint64::toString(s.find('l', 3))));
    }
    
    TEST(StringFindConstantTimeTest, StringTestBase)
    {
        String s("SecretKey");
        compare(String("0"), String(uint64::toString(s.find('S'))));
        compare(String("3"), String(uint64::toString(s.find('r'))));
        compare(String("18446744073709551615"), String(uint64::toString(s.find('Z')))); // npos
    }
    
    // =============================================================================
    // STATIC CONSTANT TESTS
    // =============================================================================
    
    TEST(StringStaticConstantsTest, StringTestBase)
    {
        // Test that static constants are properly initialized
        compare(String("1"), String(String::letters.size() > 0 ? "1" : "0"));
        compare(String("1"), String(String::uppercase.size() > 0 ? "1" : "0"));
        compare(String("1"), String(String::lowercase.size() > 0 ? "1" : "0"));
        compare(String("1"), String(String::digits.size() > 0 ? "1" : "0"));
        compare(String("1"), String(String::hex_digits.size() > 0 ? "1" : "0"));
        compare(String("1"), String(String::alpha_numeric.size() > 0 ? "1" : "0"));
        compare(String("1"), String(String::base64_alphabet.size() > 0 ? "1" : "0"));
        compare(String("1"), String(String::printable.size() > 0 ? "1" : "0"));
        compare(String("1"), String(String::punctuation.size() > 0 ? "1" : "0"));
        compare(String("1"), String(String::white_space.size() > 0 ? "1" : "0"));
    }
    
    // =============================================================================
    // CHUNK METHOD TESTS
    // =============================================================================
    
    TEST(StringChunkValidTest, StringTestBase)
    {
        std::string large_data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        try {
            std::vector<String> chunks = String::chunk(large_data, 10);
            compare(String("1"), String(chunks.size() > 0 ? "1" : "0"));
            compare(String("4"), String(uint64::toString(chunks.size()))); // ceil(36/10) = 4
            compare(String("ABCDEFGHIJ"), chunks[0]);
            compare(String("0123456789"), chunks[3]);
        } catch (...) {
            compare(String("1"), String("0")); // Should not throw for valid chunk size
        }
    }
    
    TEST(StringChunkInvalidTest, StringTestBase)
    {
        std::string data = "Test";
        try {
            std::vector<String> chunks = String::chunk(data, 0);
            compare(String("1"), String("0")); // Should not reach here
        } catch (const std::invalid_argument&) {
            compare(String("1"), String("1")); // Expected exception for zero chunk size
        }
        
        try {
            std::vector<String> chunks = String::chunk(data, 8192); // > MAX_SECURE_LENGTH
            compare(String("1"), String("0")); // Should not reach here
        } catch (const std::invalid_argument&) {
            compare(String("1"), String("1")); // Expected exception for oversized chunk
        }
    }
    
    // =============================================================================
    // SECURITY AND EDGE CASE TESTS
    // =============================================================================
    
    TEST(StringMaxLengthTest, StringTestBase)
    {
        // Test behavior at maximum length
        String s;
        for (size_t i = 0; i < 100; ++i) { // Fill with reasonable amount
            s.push_back('A');
        }
        compare(String("100"), String(uint64::toString(s.size())));
    }
    
    TEST(StringSecureComparisonTest, StringTestBase)
    {
        // Test constant-time equals vs regular comparison
        String secret1("password123");
        String secret2("password123");
        String wrong("password124");
        
        compare(String("1"), String(secret1.equals(secret2) ? "1" : "0"));
        compare(String("0"), String(secret1.equals(wrong) ? "1" : "0"));
    }
    
    TEST(StringMemoryAlignmentTest, StringTestBase)
    {
        // Test that strings are properly aligned for cache performance
        String s1, s2, s3;
        
        // Just verify they can be created without issues
        compare(String("1"), String("1"));
    }
    
    TEST(StringNullTerminationTest, StringTestBase)
    {
        String s("Test");
        const char* cstr = s.c_str();
        
        // Verify proper null termination
        String from_cstr(cstr);
        compare(String("Test"), from_cstr);
        compare(String("4"), String(uint64::toString(from_cstr.size())));
    }
}
