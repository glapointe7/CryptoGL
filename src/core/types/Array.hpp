#pragma once

#include "Types.hpp"
#include "Integer.hpp"

#include <functional>
#include <algorithm>
#include <array>

namespace CryptoGL
{
    template <typename Type, size_t Size>
    class Array
    {
        private:
            std::array<Type, Size> data_{};

            template<char Separator, size_t Index>
            [[nodiscard]] String joinRecursive() const
            {
                if constexpr (Index == Size - 1) {
                    // Last element - no separator needed
                    return Integer<Type>::toString(data_[Index]);
                } else {
                    // Recursive case - add element, separator, and continue
                    String current = Integer<Type>::toString(data_[Index]);
                    current.push_back(Separator);
                    current.append(joinRecursive<Separator, Index + 1>());
                    return current;
                }
            }

            template<uint32_t ChunkSize, uint32_t NumChunks>
            [[nodiscard]] constexpr auto createChunks() const
            {
                Array<Array<Type, ChunkSize>, NumChunks> result;
                
                // Compile-time unrolled loop for optimal performance
                for (uint32_t i = 0; i < NumChunks; ++i) {
                    Array<Type, ChunkSize> chunk;
                    for (uint32_t j = 0; j < ChunkSize; ++j) {
                        chunk[j] = data_[i * ChunkSize + j];
                    }
                    result[i] = chunk;
                }
                
                return result;
            }

            template<std::size_t NewSize, std::size_t Begin, std::size_t End, std::size_t RangeSize>
            [[nodiscard]] constexpr auto createExtendedArray() const
            {
                Array<Type, NewSize> result;
                
                // Copy original elements
                for (std::size_t i = 0; i < Size; ++i) {
                    result[i] = data_[i];
                }
                
                // Copy range elements from source array
                for (std::size_t i = 0; i < RangeSize; ++i) {
                    result[Size + i] = data_[Begin + i];
                }
                
                return result;
            }

        public:
            constexpr Array() noexcept = default;
            // Copy constructor
            constexpr Array(const Array& other) noexcept = default;

            // Move constructor
            constexpr Array(Array&& other) noexcept = default;
            
            constexpr explicit Array(const std::array<Type, Size>& arr) noexcept
                : data_(arr) {}

            /**
             * @brief Constructor from initializer list.
             * 
             * Allows creating Array objects directly from brace-enclosed initializer lists.
             * 
             * @param init_list The initializer list to construct from
             */
            constexpr Array(std::initializer_list<Type> init_list) noexcept
            {
                size_t i = 0;
                for (const auto& value : init_list) {
                    if (i < Size) {
                        data_[i] = value;
                        ++i;
                    }
                }
                // Fill remaining elements with zeros if initializer list is shorter
                while (i < Size) {
                    data_[i] = Type{};
                    ++i;
                }
            }

            constexpr Type& operator[](std::size_t i) noexcept { return data_[i]; }
            constexpr const Type& operator[](std::size_t i) const noexcept { return data_[i]; }

            /**
             * @brief Copy assignment operator for Array objects.
             * 
             * Allows assigning one Array to another of the same type and size.
             * 
             * @param other The Array to copy from
             * @return Reference to this Array
             */
            constexpr Array& operator=(const Array& other) noexcept
            {
                if (this != &other) {
                    for (size_t i = 0; i < Size; ++i) {
                        data_[i] = other.data_[i];
                    }
                }
                return *this;
            }

            /**
             * @brief Initializer list assignment operator.
             * 
             * Allows assigning an initializer list to an existing Array object.
             * 
             * @param init_list The initializer list to assign
             * @return Reference to this Array
             */
            constexpr Array& operator=(std::initializer_list<Type> init_list) noexcept
            {
                size_t i = 0;
                for (const auto& value : init_list) {
                    if (i < Size) {
                        data_[i] = value;
                        ++i;
                    }
                }
                // Fill remaining elements with zeros if initializer list is shorter
                while (i < Size) {
                    data_[i] = Type{};
                    ++i;
                }
                return *this;
            }

            /**
             * @brief Move assignment operator for Array objects.
             * 
             * Allows moving one Array to another, which can be more efficient
             * than copying for some types.
             * 
             * @param other The Array to move from
             * @return Reference to this Array
             */
            constexpr Array& operator=(Array&& other) noexcept
            {
                if (this != &other) {
                    for (size_t i = 0; i < Size; ++i) {
                        data_[i] = std::move(other.data_[i]);
                    }
                }
                return *this;
            }

            static constexpr std::size_t size() noexcept { return Size; }

            constexpr auto begin() noexcept { return data_.begin(); }
            constexpr auto end() noexcept { return data_.end(); }
            constexpr auto begin() const noexcept { return data_.begin(); }
            constexpr auto end() const noexcept { return data_.end(); }

            [[nodiscard]] constexpr Array<Type, Size> Xor(const Array<Type, Size>& other) const noexcept {
                Array<Type, Size> result;
                for (size_t i = 0; i < Size; ++i) {
                    result.data_[i] = data_[i] ^ other.data_[i];
                }
                return result;
            }

            template <std::size_t OtherSize>
            [[nodiscard]] constexpr Array<Type, Size + OtherSize> Extend(const Array<Type, OtherSize>& other) const noexcept
            {
                Array<Type, Size + OtherSize> result;

                for (std::size_t i = 0; i < Size; ++i) {
                    result[i] = data_[i];
                }
                for (std::size_t j = 0; j < OtherSize; ++j) {
                    result[Size + j] = other[j];
                }

                return result;
            }

            /**
             * @brief Template-based range extend for compile-time known indices.
             * 
             * This implementation allows the range indices to be specified at compile-time,
             * enabling better compiler optimizations and potential compile-time evaluation
             * when the arrays are constant.
             * 
             * @tparam Type The type of elements in the array
             * @tparam Size The fixed size of the current array
             * @tparam OtherSize The size of the source array
             * @tparam Begin The starting index of the range (compile-time known)
             * @tparam End The ending index of the range (compile-time known)
             * @param other The source array to extract elements from
             * @return Array with extended size containing the original elements plus the range
             * 
             * @note This method can be evaluated at compile-time for constant arrays,
             *       providing optimal performance and zero runtime overhead.
             * 
             * @example
             * Array<uint8_t, 2> arr1 = {1, 2};
             * Array<uint8_t, 4> arr2 = {10, 20, 30, 40};
             * auto result = arr1.template extendRange<1, 3>(arr2); // Result: {1, 2, 20, 30}
             */
            template<std::size_t OtherSize, std::size_t Begin, std::size_t End>
            [[nodiscard]] constexpr auto extendRange() const
            {
                static_assert(Begin < End, "Begin index must be less than end index");
                static_assert(End <= OtherSize, "End index cannot exceed source array size");
                
                constexpr std::size_t range_size = End - Begin;
                constexpr std::size_t new_size = Size + range_size;
                
                return createExtendedArray<new_size, Begin, End, range_size>();
            }

            [[nodiscard]] constexpr std::ptrdiff_t Find(const Type& value) const noexcept
            {
                for (std::size_t i = 0; i < Size; ++i) {
                    if (data_[i] == value) {
                        return static_cast<std::ptrdiff_t>(i);
                    }
                }
                return -1; 
            }

            void Swap(size_t i, size_t j) noexcept {
                // Create a mask = 0xFF..FF if k == i or j, otherwise 0x00..00
                // Apply XOR on each element.
                if (i == j) return; // cas trivial (pas de fuite temporelle)

                for (size_t k = 0; k < Size; ++k) {
                    // mask_i = all ones if k == i, otherwise 0
                    auto mask_i = static_cast<Type>(-(static_cast<Type>(k == i)));
                    // mask_j = all ones if k == j, otherwise 0
                    auto mask_j = static_cast<Type>(-(static_cast<Type>(k == j)));

                    // Select values only for indices i and j
                    Type xi = data_[k] & mask_i;
                    Type xj = data_[k] & mask_j;

                    // Calculate the difference for the swap
                    Type diff = xi ^ xj;

                    // Apply diff on cases i and j uniquely
                    data_[k] ^= diff;
                }
            }

            [[nodiscard]] constexpr Type msb() const noexcept {
                Type result = 0;
                for (size_t i = 0; i < Size; ++i) {
                    // mask = 0xFF..FF si i == 0, sinon 0
                    auto mask = static_cast<Type>(-(static_cast<Type>(i == 0)));
                    result |= (data_[i] & static_cast<Type>(0x80)) & mask;
                }
                return result ? 1 : 0; // renvoie 1 si MSB=1, sinon 0
            }

            [[nodiscard]] constexpr Type lsb() const noexcept {
                Type result = 0;
                for (size_t i = 0; i < Size; ++i) {
                    // mask = 0xFF..FF si i == Size-1, sinon 0
                    auto mask = static_cast<Type>(-(static_cast<Type>(i == (Size - 1))));
                    result |= (data_[i] & static_cast<Type>(0x01)) & mask;
                }
                return result ? 1 : 0; // renvoie 1 si LSB=1, sinon 0
            }

            /// Constant-time logical left shift (whole array as a big integer).
            [[nodiscard]] constexpr Array<Type, Size> leftShift(uint32_t shift) const noexcept {
                Array<Type, Size> result{};
                constexpr uint32_t word_bits = sizeof(Type) * 8;
                constexpr uint32_t total_bits = word_bits * Size;

                // Si shift >= total_bits, tout devient 0 (branch-free via mask)
                const uint32_t valid = (shift < total_bits);  
                const uint32_t s_words = (shift / word_bits) * valid;
                const uint32_t s_bits  = (shift % word_bits) * valid;

                for (size_t i = 0; i < Size; ++i) {
                    uint64_t acc = 0;

                    // récupérer mot source
                    if (i + s_words < Size) {
                        acc = static_cast<uint64_t>(data_[i + s_words]) << s_bits;
                    }

                    // propagation des bits de la "droite"
                    if (s_bits && i + s_words + 1 < Size) {
                        acc |= static_cast<uint64_t>(data_[i + s_words + 1]) >> (word_bits - s_bits);
                    }

                    result.data_[i] = static_cast<Type>(acc);
                }

                // si shift >= total_bits, tout devient 0 (masquage branch-free)
                for (auto &x : result.data_) {
                    x &= static_cast<Type>(-static_cast<int32_t>(valid));
                }

                return result;
            }

            /// Constant-time logical right shift
            [[nodiscard]] constexpr Array<Type, Size> rightShift(uint32_t shift) const noexcept {
                Array<Type, Size> result{};
                constexpr uint32_t word_bits = sizeof(Type) * 8;
                constexpr uint32_t total_bits = word_bits * Size;

                const uint32_t valid = (shift < total_bits);  
                const uint32_t s_words = (shift / word_bits) * valid;
                const uint32_t s_bits  = (shift % word_bits) * valid;

                for (size_t i = 0; i < Size; ++i) {
                    uint64_t acc = 0;

                    // récupérer mot source
                    if (i + s_words < Size) {
                        acc = static_cast<uint64_t>(data_[Size - 1 - (i + s_words)]) >> s_bits;
                    }

                    // propagation des bits de la "gauche"
                    if (s_bits && i + s_words + 1 < Size) {
                        acc |= static_cast<uint64_t>(data_[Size - 1 - (i + s_words + 1)]) << (word_bits - s_bits);
                    }

                    result.data_[Size - 1 - i] = static_cast<Type>(acc);
                }

                // masquage si shift >= total_bits
                for (auto &x : result.data_) {
                    x &= static_cast<Type>(-static_cast<int32_t>(valid));
                }

                return result;
            }

            [[nodiscard]] constexpr Array<Type, Size> rotateLeft(const uint32_t to_rotate) const noexcept
            {
                if constexpr (Size == 1) {
                    // For single-word arrays, std::rotl is perfect
                    return Array<Type, Size>{std::rotl(data_[0], to_rotate)};
                } else {
                    // For multi-word arrays, use the cross-word rotation logic
                    constexpr uint32_t word_bits = sizeof(Type) * 8;
                    constexpr uint32_t total_bits = word_bits * Size;
                    
                    const uint32_t shift = to_rotate & (total_bits - 1);
                    const uint32_t s_words = shift / word_bits;
                    const uint32_t s_bits = shift % word_bits;
                    
                    Array<Type, Size> result{};
                    
                    for (size_t i = 0; i < Size; ++i) {
                        const size_t src_idx = (i + s_words) % Size;
                        const size_t carry_idx = (src_idx + 1) % Size;
                        
                        // Use std::rotl for the main rotation
                        uint64_t acc = std::rotl(static_cast<uint64_t>(data_[src_idx]), s_bits);
                        
                        if (s_bits > 0) {
                            const uint64_t carry_bits = static_cast<uint64_t>(data_[carry_idx]) >> (word_bits - s_bits);
                            acc = (acc & ((1ULL << word_bits) - 1)) | (carry_bits << word_bits);
                        }
                        
                        result.data_[i] = static_cast<Type>(acc);
                    }
                    
                    return result;
                }
            }

            [[nodiscard]] constexpr Array<Type, Size> rotateRight(const uint32_t to_rotate) const noexcept
            {
                if constexpr (Size == 1) {
                    // For single-word arrays, std::rotr is perfect and most efficient
                    return Array<Type, Size>{std::rotr(data_[0], to_rotate)};
                } else {
                    // For multi-word arrays, use cross-word rotation logic
                    constexpr uint32_t word_bits = sizeof(Type) * 8;
                    constexpr uint32_t total_bits = word_bits * Size;
                    
                    const uint32_t shift = to_rotate & (total_bits - 1);
                    const uint32_t s_words = shift / word_bits;
                    const uint32_t s_bits = shift % word_bits;
                    
                    Array<Type, Size> result{};
                    
                    for (size_t i = 0; i < Size; ++i) {
                        const size_t src_idx = (Size - 1 - i + s_words) % Size;
                        const size_t carry_idx = (src_idx - 1 + Size) % Size;
                        
                        // Use std::rotr for the main rotation when possible
                        uint64_t acc = std::rotr(static_cast<uint64_t>(data_[src_idx]), s_bits);
                        
                        // Handle cross-word carry for multi-word arrays
                        if (s_bits > 0) {
                            const uint64_t carry_bits = static_cast<uint64_t>(data_[carry_idx]) << (word_bits - s_bits);
                            // Combine with the rotated result, ensuring proper word boundaries
                            acc = (acc & ((1ULL << word_bits) - 1)) | (carry_bits & ((1ULL << word_bits) - 1));
                        }
                        
                        result.data_[Size - 1 - i] = static_cast<Type>(acc);
                    }
                    
                    return result;
                }
            }

            /**
             * @brief Template-based join method for compile-time evaluation with default separator.
             * 
             * This implementation provides compile-time evaluation when the separator is known
             * at compile-time. It offers zero runtime overhead and optimal performance.
             * 
             * @tparam Type The type of elements in the array
             * @tparam Size The fixed size of the array
             * @tparam Separator The separator character (defaults to '-' if not specified)
             * @return String representation of the array
             * 
             * @note This method can be evaluated entirely at compile-time for constant
             *       arrays, providing optimal performance and zero runtime overhead.
             * 
             * @example
             * Array<uint8_t, 4> arr = {12, 11, 3, 434};
             * auto result1 = arr.joinTemplate();           // Uses default separator '-'
             * auto result2 = arr.template joinTemplate<':'>(); // Custom separator ':'
             */
            template<char Separator = '-'>
            [[nodiscard]] String join() const
            {
                if constexpr (Size == 0) {
                    return String{};
                } else if constexpr (Size == 1) {
                    return Integer<Type>::toString(data_[0]);
                } else {
                    // Recursive template approach for compile-time evaluation
                    return joinRecursive<Separator, 0>();
                }
            }

            /**
             * @brief Template-based chunk method optimized for cryptographic block sizes.
             * 
             * This method is specifically designed for cryptographic applications where
             * the chunk size is known at compile-time (e.g., AES block size = 16 bytes).
             * It provides optimal performance through compile-time optimization and
             * zero runtime overhead for constant arrays.
             * 
             * @tparam Type The type of elements in the array
             * @tparam Size The fixed size of the array
             * @tparam ChunkSize The size of each chunk (must be > 0 and <= Size)
             * @return Array of Arrays with compile-time known chunk sizes
             * 
             * @note This method is optimized for the common cryptographic use case where
             *       messages are chunked into fixed-size blocks (16, 32, 64, 128 bytes).
             *       It can be evaluated at compile-time for constant arrays.
             * 
             * @example
             * // For AES operations (16-byte blocks)
             * Array<uint8_t, 64> message = {...};
             * auto blocks = message.template chunk<16>(); // 4 blocks of 16 bytes each
             * 
             * // For hash function input blocks
             * Array<uint8_t, 128> data = {...};
             * auto blocks = data.template chunk<64>(); // 2 blocks of 64 bytes each
             */
            template<uint32_t ChunkSize>
            [[nodiscard]] constexpr auto chunk() const
            {
                static_assert(ChunkSize > 0, "Chunk size must be greater than 0");
                static_assert(ChunkSize <= Size, "Chunk size cannot exceed array size");
                static_assert(Size % ChunkSize == 0, "Array size must be divisible by chunk size for optimal performance");
                
                if constexpr (Size == 0) {
                    return Array<Array<Type, 0>, 0>{};
                } else if constexpr (Size == ChunkSize) {
                    // Single chunk case
                    return Array<Array<Type, Size>, 1>{Array<Type, Size>(data_)};
                } else {
                    // Multiple chunks case - compile-time optimized
                    constexpr uint32_t NumChunks = Size / ChunkSize;
                    return createChunks<ChunkSize, NumChunks>();
                }
            }

            /**
             * @brief Converts the array contents to a hexadecimal string representation.
             * 
             * This method transforms each element of the array into its hexadecimal
             * representation and concatenates them into a single string. The method
             * maintains constant-time execution characteristics and can be evaluated
             * at compile-time when the array is constant.
             * 
             * @tparam Type The type of elements in the array
             * @tparam Size The fixed size of the array
             * @return String containing the hexadecimal representation of all array elements
             * 
             * @note This method maintains constant-time execution suitable for cryptographic
             *       applications. Each element is converted to hex using 4 bits per hex digit.
             * 
             * @example
             * Array<uint8_t, 4> arr = {0x01, 0x23, 0x45, 0x67};
             * String result = arr.toHexString(); // Returns "01234567"
             * 
             * Array<uint32_t, 2> arr2 = {0x12345678, 0x9ABCDEF0};
             * String result2 = arr2.toHexString(); // Returns "123456789ABCDEF0"
             */
            [[nodiscard]] String toHexString() const
            {
                if constexpr (Size == 0) {
                    return String{};
                }
                
                // Calculate exact string length at compile-time when possible
                constexpr uint8_t data_size = sizeof(Type) * 8;
                constexpr uint32_t hex_digits_per_element = data_size / 4;
                constexpr uint32_t total_hex_digits = Size * hex_digits_per_element;
                
                String hex_digest;
                //hex_digest.reserve(total_hex_digits);
                
                // Convert each element to hexadecimal
                for (uint32_t i = 0; i < Size; ++i) {
                    for (int8_t j = data_size - 4; j >= 0; j -= 4) {
                        hex_digest.push_back(String::hex_digits[(data_[i] >> j) & 0xF]);
                    }
                }
                
                return hex_digest;
            }
    };
}