#pragma once

#include <string>
#include <vector>
#include <array>

namespace CryptoGL
{
    /**
     * @brief Constant-time string view wrapper for cryptographic applications.
     * 
     * This class provides a constant-time alternative to the String class,
     * using std::string_view for optimal performance and security. It includes
     * all the methods from your original String class.
     * 
     * @note This class maintains constant-time execution characteristics
     *       and can be evaluated at compile-time when possible.
     */
    class StringView
    {
    private:
        std::string_view data_;
        
        // Compile-time constants (equivalent to your String constants)
        static constexpr std::string_view letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        static constexpr std::string_view uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static constexpr std::string_view lowercase = "abcdefghijklmnopqrstuvwxyz";
        static constexpr std::string_view grid_uppercase_fr = "ABCDEFGHIJKLMNOPQRSTUVXYZ";
        static constexpr std::string_view grid_lowercase_fr = "abcdefghijklmnopqrstuvxyz";
        static constexpr std::string_view grid_uppercase_en = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
        static constexpr std::string_view grid_lowercase_en = "abcdefghiklmnopqrstuvwxyz";
        static constexpr std::string_view digits = "0123456789";
        static constexpr std::string_view uppercase_digits = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        static constexpr std::string_view lowercase_digits = "abcdefghijklmnopqrstuvwxyz0123456789";
        static constexpr std::string_view hex_digits = "0123456789ABCDEF";
        static constexpr std::string_view alpha_numeric = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        static constexpr std::string_view ascii_ordered_alpha_numeric = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        static constexpr std::string_view lower_ordered_alpha_numeric = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        static constexpr std::string_view base64_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        static constexpr std::string_view printable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!\"'().:;? \t\n\r#[]$%&*/@|_=+<>{}~";
        static constexpr std::string_view punctuation = "!\"'().:;?-";
        static constexpr std::string_view white_space = " \t\n\r";

    public:
        // Constructors
        constexpr StringView() noexcept = default;
        constexpr explicit StringView(const char* str) noexcept : data_(str) {}
        constexpr explicit StringView(const std::string& str) noexcept : data_(str) {}
        constexpr explicit StringView(std::string_view str) noexcept : data_(str) {}
        
        // Basic operations
        [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }
        [[nodiscard]] constexpr size_t length() const noexcept { return data_.size(); }
        
        // Access operations
        [[nodiscard]] constexpr char operator[](size_t pos) const noexcept { return data_[pos]; }
        [[nodiscard]] constexpr char at(size_t pos) const noexcept { return data_[pos]; }
        
        // Iterator operations
        [[nodiscard]] constexpr auto begin() const noexcept { return data_.begin(); }
        [[nodiscard]] constexpr auto end() const noexcept { return data_.end(); }
        [[nodiscard]] constexpr auto cbegin() const noexcept { return data_.cbegin(); }
        [[nodiscard]] constexpr auto cend() const noexcept { return data_.cend(); }
        

        /**
         * @brief Convert string to array of bytes.
         * 
         * @return std::array<char, string_length> of characters
         */
        [[nodiscard]] constexpr auto toBytes() const
        {
            constexpr size_t string_length = this->length();
            std::array<char, string_length> bytes;
            for (size_t i = 0; i < string_length; ++i)
            {
                bytes[i] = this->at(i);
            }

            return bytes;
        }
        
        
    };
}