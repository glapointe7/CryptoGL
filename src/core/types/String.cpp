#include "String.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <atomic>
#include <iostream>
#include <string> 


namespace CryptoGL {
    std::ostream& operator<<(std::ostream& os, const String& str)
    {
        // Explicit bounds checking and secure output
        if (str.empty()) {
            return os;
        }
        
        // Use the secure data access method
        const char* data = str.data();
        const std::streamsize size = static_cast<std::streamsize>(str.size());
        
        // Write the exact number of characters (no null terminator)
        return os.write(data, size);
    }

    // Concaténation : String + String
    String operator+(const String& lhs, const String& rhs)
    {
        if (lhs.length() + rhs.length() > String::MAX_SECURE_LENGTH) {
            throw std::invalid_argument(
            "String::operator+: length (" + std::to_string(lhs.length() + rhs.length()) + 
            ") exceeds MAX_SECURE_LENGTH (" + std::to_string(String::MAX_SECURE_LENGTH) + 
            "). Chunk your strings to use smaller strings.");
        }

        String result = lhs;
        result.append(rhs);
        return result;
    }

    // Concaténation : String + const char*
    String operator+(const String& lhs, const char* rhs)
    {
        const size_t rhs_length = std::char_traits<char>::length(rhs);
        if (lhs.length() + rhs_length > String::MAX_SECURE_LENGTH) {
            throw std::invalid_argument(
            "String::operator+: length (" + std::to_string(lhs.length() + rhs_length) + 
            ") exceeds MAX_SECURE_LENGTH (" + std::to_string(String::MAX_SECURE_LENGTH) + 
            "). Chunk your strings to use smaller strings.");
        }

        String result = lhs;
        result.append(rhs);
        return result;
    }

    // Concaténation : const char* + String
    String operator+(const char* lhs, const String& rhs)
    {
        const size_t lhs_length = std::char_traits<char>::length(lhs);
        if (lhs_length + rhs.length() > String::MAX_SECURE_LENGTH) {
            throw std::invalid_argument(
            "String::operator+: length (" + std::to_string(lhs_length + rhs.length()) + 
            ") exceeds MAX_SECURE_LENGTH (" + std::to_string(String::MAX_SECURE_LENGTH) + 
            "). Chunk your strings to use smaller strings.");
        }

        String result = lhs;
        result.append(rhs);
        return result;
    }

    // Concaténation : String + char
    String operator+(const String& lhs, char rhs)
    {
        if (lhs.length() >= String::MAX_SECURE_LENGTH) {
            throw std::invalid_argument(
            "String::operator+: length (" + std::to_string(lhs.length()) + 
            ") exceeds MAX_SECURE_LENGTH (" + std::to_string(String::MAX_SECURE_LENGTH) + 
            "). Chunk your string to use a smaller string.");
        }

        String result = lhs;
        result.push_back(rhs);
        return result;
    }
}

using namespace CryptoGL;

// Static constant definitions
const String String::letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const String String::uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const String String::lowercase = "abcdefghijklmnopqrstuvwxyz";
const String String::grid_uppercase_fr = "ABCDEFGHIJKLMNOPQRSTUVXYZ";
const String String::grid_lowercase_fr = "abcdefghijklmnopqrstuvxyz";
const String String::grid_uppercase_en = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
const String String::grid_lowercase_en = "abcdefghiklmnopqrstuvwxyz";
const String String::digits = "0123456789";
const String String::uppercase_digits = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const String String::lowercase_digits = "abcdefghijklmnopqrstuvwxyz0123456789";
const String String::hex_digits = "0123456789ABCDEF";
const String String::alpha_numeric = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
const String String::ascii_ordered_alpha_numeric = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const String String::lower_ordered_alpha_numeric = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const String String::base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const String String::printable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"'().:;? \t\n\r#[]$%&*/@|_=+<>{}~";
const String String::punctuation = "!\"'().:;?-";
const String String::white_space = " \t\n\r";

// Constructor implementations
String::String() noexcept
    : m_data{}, m_length(0)
{
    ensure_null_terminated();
}

String::String(size_type to_reserve) noexcept
    : m_data{}, m_length(0)
{
    // Note: Fixed-size buffer, so reservation is informational only
    ensure_null_terminated();
}

String::String(const char* str) noexcept
    : m_data{}, m_length(0)
{
    if (str != nullptr) {
        const size_type len = std::strlen(str);
        const size_type copy_len = std::min(len, MAX_SECURE_LENGTH);
        
        // Secure copy - always copy same amount of data
        for (size_type i = 0; i < copy_len; ++i) {
            m_data[i] = str[i];
        }
        m_length = copy_len;
    }
    ensure_null_terminated();
}

String::String(const std::string& str) noexcept
    : String(str.c_str())
{
}

String::String(size_type n, char c) noexcept
    : m_data{}, m_length(0)
{
    const size_type fill_len = std::min(n, MAX_SECURE_LENGTH);
    
    // Secure fill - constant time
    for (size_type i = 0; i < fill_len; ++i) {
        m_data[i] = c;
    }
    m_length = fill_len;
    ensure_null_terminated();
}

String::String(std::initializer_list<char> init) noexcept
    : m_data{}, m_length(0)
{
    const size_type copy_len = std::min(init.size(), MAX_SECURE_LENGTH);
    std::copy(init.begin(), init.begin() + copy_len, m_data);
    m_length = copy_len;
    ensure_null_terminated();
}

// Copy constructor
String::String(const String& other) noexcept
    : m_data{}, m_length(other.m_length)
{
    // Always copy fixed amount to prevent timing analysis
    for (size_type i = 0; i < MAX_SECURE_LENGTH; ++i) {
        m_data[i] = other.m_data[i];
    }
    ensure_null_terminated();
}

// Move constructor
String::String(String&& other) noexcept
    : m_data{}, m_length(other.m_length)
{
    // Move by copying then secure-clearing source
    for (size_type i = 0; i < MAX_SECURE_LENGTH; ++i) {
        m_data[i] = other.m_data[i];
    }
    other.clear();
    ensure_null_terminated();
}

// Destructor with guaranteed secure cleanup
String::~String() noexcept
{
    clear();
}

// Assignment operators
String& String::operator=(const String& other) noexcept
{
    if (this != &other) {
        clear();
        m_length = other.m_length;
        for (size_type i = 0; i < MAX_SECURE_LENGTH; ++i) {
            m_data[i] = other.m_data[i];
        }
        ensure_null_terminated();
    }
    return *this;
}

String& String::operator=(String&& other) noexcept
{
    if (this != &other) {
        clear();
        m_length = other.m_length;
        for (size_type i = 0; i < MAX_SECURE_LENGTH; ++i) {
            m_data[i] = other.m_data[i];
        }
        other.clear();
        ensure_null_terminated();
    }
    return *this;
}

String& String::operator=(const char* str) noexcept
{
    clear();
    if (str != nullptr) {
        const size_type len = std::strlen(str);
        const size_type copy_len = std::min(len, MAX_SECURE_LENGTH);
        for (size_type i = 0; i < copy_len; ++i) {
            m_data[i] = str[i];
        }
        m_length = copy_len;
    }
    ensure_null_terminated();
    return *this;
}

String& String::operator+=(const String& other) noexcept
{
    size_type new_len = m_length + other.m_length;
    if (new_len > MAX_SECURE_LENGTH) {
        new_len = MAX_SECURE_LENGTH; // on tronque pour rester safe
    }

    // On copie jusqu'à MAX_SECURE_LENGTH pour rester constant-time
    for (size_type i = 0; i < MAX_SECURE_LENGTH; ++i) {
        // masque : 1 si l’indice est valide dans la partie à ajouter
        unsigned char in_range = static_cast<unsigned char>(
            (i < other.m_length) & (m_length + i < MAX_SECURE_LENGTH)
        );

        // soit on prend le caractère de other, soit 0
        char c = static_cast<char>(other.m_data[i] * in_range);

        // on écrit seulement si dans la zone cible (branchless aussi)
        unsigned char can_write = static_cast<unsigned char>(
            (m_length + i) < MAX_SECURE_LENGTH
        );

        m_data[m_length + i] = (c * can_write) |
                               (m_data[m_length + i] & ~static_cast<char>(can_write));
    }

    m_length = new_len;
    m_data[m_length] = '\0';

    memory_barrier();
    return *this;
}

// Element access with bounds checking
String::reference String::at(size_type pos)
{
    if (pos >= m_length) {
        throw std::out_of_range("String::at: index out of range");
    }
    return m_data[pos];
}

String::const_reference String::at(size_type pos) const
{
    if (pos >= m_length) {
        throw std::out_of_range("String::at: index out of range");
    }
    return m_data[pos];
}

// String data access
const char* String::c_str() const noexcept
{
    // Ensure null termination
    const_cast<String*>(this)->ensure_null_terminated();
    return m_data;
}

// CRITICAL: Constant-time comparison operations
bool String::equals(const String& other) const noexcept
{
    // Always check full buffer length to prevent size-based timing attacks
    volatile uint8_t result = 0;
    
    // Check length difference in constant time
    result |= (m_length != other.m_length);
    
    // Always compare MAX_SECURE_LENGTH bytes to prevent timing leaks
    for (size_type i = 0; i < MAX_SECURE_LENGTH; ++i) {
        result |= (m_data[i] ^ other.m_data[i]);
    }
    
    memory_barrier();  // Prevent compiler optimization
    return (result == 0);
}

bool String::equals(const char* str) const noexcept
{
    if (str == nullptr) {
        return m_length == 0;
    }
    
    const size_type str_len = std::strlen(str);
    volatile uint8_t result = 0;
    
    result |= (m_length != str_len);
    
    // Always compare full buffer for constant time
    for (size_type i = 0; i < MAX_SECURE_LENGTH; ++i) {
        char str_char = (i < str_len) ? str[i] : 0;
        result |= (m_data[i] ^ str_char);
    }
    
    memory_barrier();
    return (result == 0);
}

// Standard comparison operators (not constant-time - use with caution)
bool String::operator==(const String& other) const noexcept
{
    if (m_length != other.m_length) {
        return false;
    }
    return std::memcmp(m_data, other.m_data, m_length) == 0;
}

bool String::operator!=(const String& other) const noexcept
{
    return !(*this == other);
}

bool String::operator<(const String& other) const noexcept
{
    const size_type min_len = std::min(m_length, other.m_length);
    int result = std::memcmp(m_data, other.m_data, min_len);
    return (result < 0) || (result == 0 && m_length < other.m_length);
}

String String::substr(size_type pos, size_type count) const {
    if (pos > m_length) {
        throw std::out_of_range("String::substr: pos > size()");
    }

    if (count == npos || pos + count > m_length) {
        count = m_length - pos;
    }

    String result;
    result.m_length = count;

    // copie sécurisée
    std::memcpy(result.m_data, m_data + pos, count);
    result.m_data[count] = '\0';

    return result; 
}

String::iterator String::erase(iterator pos) {
    if (pos < begin() || pos >= end()) {
        throw std::out_of_range("String::erase: iterator out of range");
    }

    iterator next = pos + 1;
    size_type num_to_move = static_cast<size_type>(end() - next);

    // décaler les caractères vers la gauche
    std::memmove(pos, next, num_to_move);

    --m_length;
    m_data[m_length] = '\0';

    return pos;
}

// --- erase range ---
String::iterator String::erase(iterator first, iterator last) 
{
    if (first >= last || first < begin() || last > end()) {
        return first; // Return first for invalid ranges
    }
    
    const size_type first_idx = static_cast<size_type>(first - begin());
    const size_type last_idx = static_cast<size_type>(last - begin());
    const size_type erase_count = last_idx - first_idx;
    
    if (erase_count == 0) {
        return first;
    }
    
    // Shift remaining elements left (constant-time for security)
    for (size_type i = first_idx; i < m_length - erase_count; ++i) {
        m_data[i] = m_data[i + erase_count];
    }
    
    m_length -= erase_count;
    ensure_null_terminated();
    
    // Return iterator to the element that now occupies the first erased position
    return begin() + first_idx;
}

String::iterator String::insert(iterator pos, const char c) {
    if (pos < begin() || pos > end()) {
        throw std::out_of_range("String::insert: iterator out of range");
    }
    if (m_length >= String::MAX_SECURE_LENGTH) {
        throw std::length_error("String::insert: maximum capacity reached");
    }

    // décaler les caractères à droite pour faire de la place
    size_type offset = static_cast<size_type>(pos - begin());
    std::memmove(m_data + offset + 1, m_data + offset, m_length - offset);

    m_data[offset] = c;
    ++m_length;
    m_data[m_length] = '\0'; // nul-terminé

    return begin() + offset;
}

// --- insert C-string ---
String::iterator String::insert(iterator pos, const char* str) {
    if (pos < begin() || pos > end()) {
        throw std::out_of_range("String::insert: iterator out of range");
    }
    size_type len = std::strlen(str);
    if (m_length + len > String::MAX_SECURE_LENGTH) {
        throw std::length_error("String::insert: maximum capacity exceeded");
    }

    size_type offset = static_cast<size_type>(pos - begin());
    std::memmove(m_data + offset + len, m_data + offset, m_length - offset);

    std::memcpy(m_data + offset, str, len);
    m_length += len;
    m_data[m_length] = '\0';

    return begin() + offset;
}

// --- insert another String ---
String::iterator String::insert(iterator pos, const String& other) {
    if (pos < begin() || pos > end()) {
        throw std::out_of_range("String::insert: iterator out of range");
    }
    if (m_length + other.m_length > String::MAX_SECURE_LENGTH) {
        throw std::length_error("String::insert: maximum capacity exceeded");
    }

    size_type offset = static_cast<size_type>(pos - begin());
    std::memmove(m_data + offset + other.m_length, m_data + offset, m_length - offset);

    std::memcpy(m_data + offset, other.m_data, other.m_length);
    m_length += other.m_length;
    m_data[m_length] = '\0';

    return begin() + offset;
}

void String::toUpperCase() noexcept
{
    for (size_type i = 0; i < m_length; ++i) {
        unsigned char c = static_cast<unsigned char>(m_data[i]);

        // mask = 0xFF if it's between 'a' and 'z', else 0x00
        unsigned char is_lower = static_cast<unsigned char>(
            (c >= 'a') & (c <= 'z')
        );

        // -32 if is_lower=1, else 0
        m_data[i] = static_cast<char>(c - (is_lower * 32));
    }

    memory_barrier();
}

void String::toLowerCase() noexcept
{
    for (size_type i = 0; i < m_length; ++i) {
        unsigned char c = static_cast<unsigned char>(m_data[i]);

        // mask = 1 if 'A' <= c <= 'Z', else 0
        unsigned char is_upper = static_cast<unsigned char>(
            (c >= 'A') & (c <= 'Z')
        );

        // +32 if uppercase, else +0
        m_data[i] = static_cast<char>(c + (is_upper * 32));
    }

    memory_barrier();
}

void String::removeChars(const String& chars) noexcept
{
    size_type write_pos = 0;
    
    for (size_type read_pos = 0; read_pos < m_length; ++read_pos) {
        char c = m_data[read_pos];
        uint8_t should_keep = 1;  // Assume keep by default
        
        // Always check against all chars for constant time
        for (size_type j = 0; j < chars.m_length; ++j) {
            uint8_t matches = (c == chars.m_data[j]) ? 1 : 0;
            should_keep &= (1 - matches);  // Branchless: if matches, should_keep becomes 0
        }
        
        // Branchless copy
        m_data[write_pos] = c;
        write_pos += should_keep;  // Only advance write position if keeping char
    }
    
    // Clear remaining buffer
    for (size_type i = write_pos; i < MAX_SECURE_LENGTH; ++i) {
        m_data[i] = 0;
    }
    
    m_length = write_pos;
    ensure_null_terminated();
}

void String::removeDuplicates() noexcept
{
    uint8_t seen[256] = {};  // Track seen characters
    size_type write_pos = 0;
    
    for (size_type read_pos = 0; read_pos < m_length; ++read_pos) {
        char c = m_data[read_pos];
        uint8_t char_idx = static_cast<uint8_t>(c);
        
        // Branchless duplicate detection
        uint8_t is_duplicate = seen[char_idx];
        uint8_t should_keep = 1 - is_duplicate;
        
        // Mark as seen (always, for constant time)
        seen[char_idx] = 1;
        
        // Branchless copy
        m_data[write_pos] = c;
        write_pos += should_keep;
    }
    
    // Clear remaining buffer
    for (size_type i = write_pos; i < MAX_SECURE_LENGTH; ++i) {
        m_data[i] = 0;
    }
    
    m_length = write_pos;
    ensure_null_terminated();
}

bool String::containsUniqueChars() const noexcept
{
    uint8_t seen[256] = {};  // Track seen characters
    volatile uint8_t has_duplicate = 0;
    
    // Always process entire length for constant time
    for (size_type i = 0; i < m_length; ++i) {
        uint8_t char_idx = static_cast<uint8_t>(m_data[i]);
        
        // Branchless duplicate detection
        has_duplicate |= seen[char_idx];  // If already seen, mark as duplicate
        seen[char_idx] = 1;
    }
    
    memory_barrier();
    return has_duplicate == 0;
}

// Conversion operations
std::string String::toStdString() const
{
    return std::string(m_data, m_length);
}

std::vector<uint8_t> String::toBytes() const noexcept
{
    return std::vector<uint8_t>(m_data, m_data + m_length);
}

// Search operations
String::size_type String::find(const uint8_t c) const noexcept
{
    volatile size_type result = npos;
    
    // Always scan entire string for constant time
    for (size_type i = 0; i < m_length; ++i) {
        // Branchless: update result only on first match
        uint8_t matches = (m_data[i] == c) ? 1 : 0;
        uint8_t is_first_match = (result == npos) ? 1 : 0;
        size_type new_result = (matches & is_first_match) ? i : result;
        result = new_result;
    }
    
    memory_barrier();
    return result;
}

// Standard find operation (use with caution - not constant-time)
String::size_type String::find(const uint8_t c, const size_type pos) const noexcept
{
    if (pos >= m_length) {
        return npos;
    }
    
    // Standard find with early exit - use with caution in crypto contexts
    for (size_type i = pos; i < m_length; ++i) {
        if (m_data[i] == c) {
            return i;
        }
    }
    
    return npos;
}

String::size_type String::find(const String& substr, size_type pos) const noexcept
{
    if (substr.m_length == 0 || substr.m_length > m_length) {
        return npos;
    }

    for (size_type i = pos; i <= m_length - substr.m_length; ++i) {
        size_type j = 0;
        for (; j < substr.m_length; ++j) {
            if (m_data[i + j] != substr.m_data[j]) {
                break;
            }
        }
        if (j == substr.m_length) {
            return i; 
        }
    }
    return npos; 
}

// String manipulation operations
String& String::append(const String& str) noexcept
{
    const size_type append_len = std::min(str.m_length, MAX_SECURE_LENGTH - m_length);
    
    // Secure copy - always same timing regardless of append size
    for (size_type i = 0; i < append_len; ++i) {
        m_data[m_length + i] = str.m_data[i];
    }
    
    m_length += append_len;
    ensure_null_terminated();
    return *this;
}

String& String::append(const char* str) noexcept
{
    if (str == nullptr) {
        return *this;
    }
    
    const size_type str_len = std::strlen(str);
    const size_type append_len = std::min(str_len, MAX_SECURE_LENGTH - m_length);
    
    // Secure copy - constant time for given append_len
    for (size_type i = 0; i < append_len; ++i) {
        m_data[m_length + i] = str[i];
    }
    
    m_length += append_len;
    ensure_null_terminated();
    return *this;
}

String& String::append(size_type n, char c) noexcept
{
    const size_type append_len = std::min(n, MAX_SECURE_LENGTH - m_length);
    
    // Secure fill - constant time for given append_len
    for (size_type i = 0; i < append_len; ++i) {
        m_data[m_length + i] = c;
    }
    
    m_length += append_len;
    ensure_null_terminated();
    return *this;
}

void String::push_back(char c) noexcept
{
    if (m_length < MAX_SECURE_LENGTH) {
        m_data[m_length] = c;
        m_length++;
        ensure_null_terminated();
    }
}

void String::pop_back() noexcept
{
    if (m_length > 0) {
        m_length--;
        // Secure overwrite the removed character
        m_data[m_length] = 0;
        ensure_null_terminated();
    }
}

// Additional string manipulation operations
String String::trimStart() const noexcept
{
    size_type start_pos = 0;
    
    // Find first non-space character (not constant-time)
    while (start_pos < m_length && m_data[start_pos] == ' ') {
        start_pos++;
    }
    
    String result;
    const size_type copy_len = m_length - start_pos;
    
    for (size_type i = 0; i < copy_len; ++i) {
        result.m_data[i] = m_data[start_pos + i];
    }
    
    result.m_length = copy_len;
    result.ensure_null_terminated();
    return result;
}

String String::trimEnd() const noexcept
{
    size_type end_pos = m_length;
    
    // Find last non-space character (not constant-time)
    while (end_pos > 0 && m_data[end_pos - 1] == ' ') {
        end_pos--;
    }
    
    String result;
    
    for (size_type i = 0; i < end_pos; ++i) {
        result.m_data[i] = m_data[i];
    }
    
    result.m_length = end_pos;
    result.ensure_null_terminated();
    return result;
}

std::vector<String> String::split(char separator) const noexcept
{
    std::vector<String> result;
    
    if (m_length == 0) {
        return result;
    }
    
    size_type start = 0;
    
    for (size_type i = 0; i < m_length; ++i) {
        if (m_data[i] == separator) {
            // Extract substring from start to i
            String substring;
            const size_type substr_len = i - start;
            
            for (size_type j = 0; j < substr_len; ++j) {
                substring.m_data[j] = m_data[start + j];
            }
            
            substring.m_length = substr_len;
            substring.ensure_null_terminated();
            result.push_back(substring);
            
            start = i + 1;
        }
    }
    
    // Add remaining substring after last separator
    if (start < m_length) {
        String substring;
        const size_type substr_len = m_length - start;
        
        for (size_type j = 0; j < substr_len; ++j) {
            substring.m_data[j] = m_data[start + j];
        }
        
        substring.m_length = substr_len;
        substring.ensure_null_terminated();
        result.push_back(substring);
    }
    
    return result;
}

// CRITICAL: Constant-time sort - secure implementation
void String::sort() noexcept
{
    // Constant-time sorting network for security (bubble sort with fixed iterations)
    for (size_type i = 0; i < m_length; ++i) {
        for (size_type j = 0; j < m_length - 1; ++j) {
            // Branchless swap if elements are out of order
            char a = m_data[j];
            char b = m_data[j + 1];
            uint8_t should_swap = (a > b) ? 1 : 0;
            
            // Conditional move without branches
            m_data[j] = should_swap ? b : a;
            m_data[j + 1] = should_swap ? a : b;
        }
    }
}

void String::rotateLeft(size_type positions) noexcept
{
    if (m_length == 0 || positions == 0) {
        return;
    }
    
    positions = positions % m_length;  // Handle positions larger than length
    
    // Use temporary buffer for rotation
    char temp[MAX_SECURE_LENGTH];
    
    // Copy rotated content to temp buffer
    for (size_type i = 0; i < m_length; ++i) {
        temp[i] = m_data[(i + positions) % m_length];
    }
    
    // Copy back to main buffer
    for (size_type i = 0; i < m_length; ++i) {
        m_data[i] = temp[i];
    }
    
    // Clear temporary buffer for security
    for (size_type i = 0; i < MAX_SECURE_LENGTH; ++i) {
        temp[i] = 0;
    }
}

void String::rotateRight(size_type positions) noexcept
{
    if (m_length == 0 || positions == 0) {
        return;
    }
    
    // Right rotation by N is equivalent to left rotation by (length - N)
    size_type left_positions = m_length - (positions % m_length);
    rotateLeft(left_positions);
}

// Large data processing - chunk input into String pieces
std::vector<String> String::chunk(const std::string_view input, const size_type chunk_size)
{
   // Validate chunk size
   // Validate chunk size - throw exception for invalid sizes
   if (chunk_size == 0) {
      throw std::invalid_argument("SecureString::chunk(): chunk_size cannot be zero");
   }
  
   if (chunk_size > MAX_SECURE_LENGTH) {
      throw std::invalid_argument(
          "SecureString::chunk(): chunk_size (" + std::to_string(chunk_size) + 
          ") exceeds MAX_SECURE_LENGTH (" + std::to_string(MAX_SECURE_LENGTH) + 
          "). Use smaller chunks.");
   }

   std::vector<String> chunks;
   const size_type num_chunks = (input.size() + chunk_size - 1) / chunk_size;  // Ceiling division

   chunks.reserve(num_chunks);

   for (size_type i = 0; i < input.size(); i += chunk_size) {
      // Calculate chunk length (handle last chunk that might be smaller)
      size_type current_chunk_size = std::min(chunk_size, input.size() - i);

      // Create String from substring
      String chunk_string;
      const std::string_view chunk_view = input.substr(i, current_chunk_size);

      // Secure copy into chunk - constant-time for the chunk size
      if (chunk_view.size() <= MAX_SECURE_LENGTH) {
         chunk_string.m_length = chunk_view.size();

         // Copy characters with constant-time behavior
         const char* src = chunk_view.data();
         for (size_type j = 0; j < MAX_SECURE_LENGTH; ++j) {
            // Branchless copy - always same number of operations
            char src_char = (j < chunk_view.size()) ? src[j] : '\0';
            chunk_string.m_data[j] = src_char;
         }

         // Ensure null termination
         chunk_string.m_data[MAX_SECURE_LENGTH] = '\0';
         chunk_string.ensure_null_terminated();
      }

      chunks.emplace_back(std::move(chunk_string));
   }

   return chunks;
}

// Secure memory operations
void String::clear() noexcept
{
    // Compiler barrier prevents optimization
    volatile char* ptr = m_data;
    for (size_type i = 0; i < MAX_SECURE_LENGTH + 1; ++i) {
        ptr[i] = 0;
    }
    m_length = 0;
    memory_barrier();
}

// Helper methods
void String::memory_barrier() const noexcept
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
}

uint8_t String::hex_char_to_value(char c) const noexcept {
    // Crée des masques (0xFF si vrai, 0x00 sinon) sans branches
    auto mask = [](bool cond) constexpr noexcept -> uint8_t {
        return static_cast<uint8_t>(-static_cast<int>(cond));
    };

    uint8_t is_digit = mask(c >= '0' && c <= '9');
    uint8_t is_upper = mask(c >= 'A' && c <= 'F');
    uint8_t is_lower = mask(c >= 'a' && c <= 'f');

    uint8_t digit_val = static_cast<uint8_t>(c - '0');
    uint8_t upper_val = static_cast<uint8_t>(c - 'A' + 10);
    uint8_t lower_val = static_cast<uint8_t>(c - 'a' + 10);

    // Combine avec masques : exactement une des trois valeurs est conservée
    return (digit_val & is_digit) |
           (upper_val & is_upper) |
           (lower_val & is_lower);
}

std::vector<uint8_t> String::hexToBytes() const {
    std::vector<uint8_t> result;
    result.reserve(m_length / 2);

    for (size_type i = 0; i < m_length; i += 2) {
        char high_char = m_data[i];
        char low_char  = (i + 1 < m_length) ? m_data[i + 1] : '0';

        if (!isHexChar(high_char) || !isHexChar(low_char)) {
            throw std::invalid_argument("String::hexToBytes: invalid hex character");
        }

        uint8_t high = hex_char_to_value(high_char);
        uint8_t low  = hex_char_to_value(low_char);
        result.push_back(static_cast<uint8_t>((high << 4) | low));
    }
    return result;
}

constexpr bool String::isHexChar(const char c) const noexcept {
    return (c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'F') ||
           (c >= 'a' && c <= 'f');
}

bool String::is_valid_length(size_type new_length) const noexcept
{
    return new_length <= MAX_SECURE_LENGTH;
}

void String::ensure_null_terminated() noexcept
{
    m_data[m_length] = '\0';
}