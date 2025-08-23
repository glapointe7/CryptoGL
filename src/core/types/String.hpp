#pragma once

#include <vector>
#include <cstdint>
#include <string_view>
#include <initializer_list>
#include <atomic>

namespace CryptoGL
{
    /**
     * @brief Secure String class designed for cryptographic applications
     * 
     * Features:
     * - Constant-time operations to prevent timing attacks
     * - Automatic secure memory clearing
     * - Fixed-size buffer to prevent allocation-based side channels
     * - Cache-line aligned memory layout
     * - No inheritance from std::string for complete security control
     */
    class String
    {
    public:
        // Type definitions
        using size_type = std::size_t;
        using value_type = char;
        using reference = char&;
        using const_reference = const char&;
        using iterator = char*;
        using const_iterator = const char*;

        // Constants for maximum security buffer size
        static constexpr size_type MAX_SECURE_LENGTH = 4096;
        static constexpr size_type npos = static_cast<size_type>(-1);

        // Useful constants for alpha string.
        static const String letters;
        static const String uppercase;
        static const String lowercase;
        static const String grid_uppercase_fr;
        static const String grid_lowercase_fr;
        static const String grid_uppercase_en;
        static const String grid_lowercase_en;
        static const String digits;
        static const String uppercase_digits;
        static const String lowercase_digits;
        static const String hex_digits;
        static const String alpha_numeric;
        static const String ascii_ordered_alpha_numeric;
        static const String lower_ordered_alpha_numeric;
        static const String base64_alphabet;
        static const String printable;
        static const String punctuation;
        static const String white_space;

        // Constructors
        String() noexcept;
        explicit String(size_type to_reserve) noexcept;
        String(const char* str) noexcept;
        String(const std::string& str) noexcept;
        String(size_type n, char c) noexcept;
        String(std::initializer_list<char> init) noexcept;
        
        template<typename InputIterator>
        String(InputIterator first, InputIterator last) noexcept
            : m_data{}, m_length(0)
        {
            size_type pos = 0;
            for (auto it = first; it != last && pos < MAX_SECURE_LENGTH; ++it, ++pos) {
                m_data[pos] = *it;
            }
            m_length = pos;
            ensure_null_terminated();
        }

        // Copy/Move constructors and assignment
        String(const String& other) noexcept;
        String(String&& other) noexcept;
        String& operator=(const String& other) noexcept;
        String& operator=(String&& other) noexcept;
        String& operator=(const char* str) noexcept;

        // Destructor with guaranteed secure cleanup
        ~String() noexcept;

        // Capacity operations
        [[nodiscard]] size_type size() const noexcept { return m_length; }
        [[nodiscard]] size_type length() const noexcept { return m_length; }
        [[nodiscard]] size_type capacity() const noexcept { return MAX_SECURE_LENGTH; }
        [[nodiscard]] constexpr size_type max_size() const noexcept { return MAX_SECURE_LENGTH; }
        [[nodiscard]] bool empty() const noexcept { return m_length == 0; }

        // Element access (bounds checking available but use sparingly for timing reasons)
        [[nodiscard]] reference operator[](size_type pos) noexcept { return m_data[pos]; }
        [[nodiscard]] const_reference operator[](size_type pos) const noexcept { return m_data[pos]; }
        [[nodiscard]] reference at(size_type pos);
        [[nodiscard]] const_reference at(size_type pos) const;
        [[nodiscard]] reference front() noexcept { return m_data[0]; }
        [[nodiscard]] const_reference front() const noexcept { return m_data[0]; }
        [[nodiscard]] reference back() noexcept { return m_data[m_length - 1]; }
        [[nodiscard]] const_reference back() const noexcept { return m_data[m_length - 1]; }

        // Iterator support
        [[nodiscard]] iterator begin() noexcept { return m_data; }
        [[nodiscard]] const_iterator begin() const noexcept { return m_data; }
        [[nodiscard]] const_iterator cbegin() const noexcept { return m_data; }
        [[nodiscard]] iterator end() noexcept { return m_data + m_length; }
        [[nodiscard]] const_iterator end() const noexcept { return m_data + m_length; }
        [[nodiscard]] const_iterator cend() const noexcept { return m_data + m_length; }

        // String data access
        [[nodiscard]] const char* c_str() const noexcept;
        [[nodiscard]] const char* data() const noexcept { return m_data; }
        [[nodiscard]] char* data() noexcept { return m_data; }

        // Security-focused comparison operations (constant-time)
        [[nodiscard]] bool equals(const String& other) const noexcept;
        [[nodiscard]] bool equals(const char* str) const noexcept;
        
        // Standard comparison operators (use with caution - not constant-time)
        [[nodiscard]] bool operator==(const String& other) const noexcept;
        [[nodiscard]] bool operator!=(const String& other) const noexcept;
        [[nodiscard]] bool operator<(const String& other) const noexcept;

        friend String operator+(const String& lhs, const String& rhs);
        friend String operator+(const String& lhs, const char* rhs);
        friend String operator+(const char* lhs, const String& rhs);
        friend String operator+(const String& lhs, char rhs);
        friend std::ostream& operator<<(std::ostream& os, const String& str);

        // Modifiers
        String& operator+=(const String& other) noexcept;
        String& append(const String& str) noexcept;
        String& append(const char* str) noexcept;
        String& append(size_type n, char c) noexcept;
        void push_back(char c) noexcept;
        void pop_back() noexcept;
        void clear() noexcept;

        // String operations (with security considerations)
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        iterator insert(iterator pos, const char c);
        iterator insert(iterator pos, const char* str);
        iterator insert(iterator pos, const String& other);
        [[nodiscard]] String substr(size_type pos = 0, size_type count = npos) const;
        void toUpperCase() noexcept;                    // Constant-time implementation
        void toLowerCase() noexcept;                    // Constant-time implementation
        void removeChars(const String& chars) noexcept;  // Constant-time implementation
        void removeDuplicates() noexcept;                     // Constant-time implementation
        [[nodiscard]] bool containsUniqueChars() const noexcept;  // Constant-time implementation

        // Conversion operations
        [[nodiscard]] std::string toStdString() const;
        [[nodiscard]] std::vector<uint8_t> toBytes() const noexcept;
        [[nodiscard]] std::vector<uint8_t> hexToBytes() const;  // Constant-time implementation

        // String manipulation
        [[nodiscard]] String trimStart() const noexcept;
        [[nodiscard]] String trimEnd() const noexcept;
        [[nodiscard]] std::vector<String> split(char separator) const noexcept;
        void sort() noexcept;                                      // Constant-time implementation
        void rotateLeft(size_type positions) noexcept;
        void rotateRight(size_type positions) noexcept;

        // Search operations (with constant-time option)
        [[nodiscard]] size_type find(const uint8_t c, const size_type pos) const noexcept;
        [[nodiscard]] size_type find(const uint8_t c) const noexcept;  // Always scans entire string

        [[nodiscard]] static std::vector<String> chunk(const std::string_view input, 
            const size_type chunk_size = MAX_SECURE_LENGTH);

    private:
        // Cache-line aligned secure buffer
        alignas(64) char m_data[MAX_SECURE_LENGTH + 1];  // +1 for null terminator
        size_type m_length;

        // Helper methods for secure operations
        void memory_barrier() const noexcept;
        [[nodiscard]] uint8_t hex_char_to_value(char c) const noexcept;
        [[nodiscard]] bool is_valid_length(size_type new_length) const noexcept;
        void ensure_null_terminated() noexcept;
        [[nodiscard]] bool constexpr isHexChar(const char c) const noexcept;
    };
}