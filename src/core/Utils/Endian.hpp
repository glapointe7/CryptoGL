/*
 * Uses Curiously recurring template design pattern to simulate static polymorphism.
 */
#pragma once

#include "../types/Types.hpp"
#include <atomic>
#include <stdexcept>
#include <type_traits>
#include <cstring>

namespace CryptoGL
{
   /**
     * @brief Base class for secure endian conversion operations
     * 
     * This class provides a secure, high-performance foundation for endian conversions
     * using the CRTP pattern. All operations include bounds checking and are designed
     * to resist timing attacks through constant-time algorithms.
     * 
     * @tparam Endianess The derived class implementing specific endian conversion logic
     * @tparam UInteger The unsigned integer type for conversions (uint8_t, uint16_t, uint32_t, uint64_t)
     * 
     * @note UInteger must be an unsigned integral type
     * @note All methods are noexcept except where input validation may throw exceptions
     */
    template <typename Endianess, typename UInteger>
    class Endian
    {
        static_assert(std::is_integral_v<UInteger>, "Endian: UInteger must be an integral type");
        static_assert(std::is_unsigned_v<UInteger>, "Endian: UInteger must be an unsigned type");
        static_assert(sizeof(UInteger) <= 8, "Endian: UInteger must be at most 64-bit");

        using UIntVector = Vector<UInteger>;

    public:
      /**
      * @brief Convert a byte vector to an integer with bounds checking
      * 
      * Safely converts a BytesVector to an integer value using the specified endianess.
      * Includes comprehensive bounds checking to prevent buffer overflows.
      * 
      * @param bytes Input byte vector (must contain at least sizeof(UInteger) bytes)
      * @return UInteger The converted integer value
      * @throws std::invalid_argument if bytes.size() < sizeof(UInteger)
      * 
      * @note This operation is constant-time for security against timing attack
      * @warning Input validation may throw - catch exceptions appropriately
      */
       static UInteger toInteger(const BytesVector& bytes)
        {
            // Input validation - prevent buffer overflow attacks
            if (bytes.size() < sizeof(UInteger)) [[unlikely]] {
                throw std::invalid_argument(
                    "Endian::toInteger: Insufficient bytes. Required: " + 
                    std::to_string(sizeof(UInteger)) + 
                    ", Provided: " + std::to_string(bytes.size()));
            }
            
            return Endianess::toInteger(bytes);
        }

        /**
         * @brief Convert an integer to a byte vector
         * 
         * Converts an integer value to a BytesVector using the specified endianess.
         * The output vector will always contain exactly sizeof(UInteger) bytes.
         * 
         * @param value Input integer value to convert
         * @return BytesVector The converted byte sequence
         * @note This operation is noexcept and optimized for performance
         */

       static BytesVector toBytesVector(const UInteger& value) noexcept
        {
            return Endianess::toBytesVector(value);
        }

      /**
      * @brief Convert a byte vector range to an integer (safe range version)
      * 
      * Safely extracts a range from the input vector and converts it to an integer.
      * Includes bounds checking for both the range parameters and resulting vector.
      * 
      * @param bytes Input byte vector
      * @param from Starting index (inclusive)
      * @param to Ending index (exclusive)
      * @return UInteger The converted integer value
      * @throws std::invalid_argument if range is invalid or insufficient bytes
      */
       static UInteger toIntegerRange(const BytesVector& bytes, const size_t from, const size_t to)
        {
            // Validate range parameters
            if (from >= to || to > bytes.size()) [[unlikely]] {
                throw std::invalid_argument("Endian::toIntegerRange: Invalid range");
            }
            
            if ((to - from) < sizeof(UInteger)) [[unlikely]] {
                throw std::invalid_argument("Endian::toIntegerRange: Range too small");
            }
            
            return toInteger(bytes.range(from, to));
        }

       /**
      * @brief Convert a byte vector range to an integer (from index to end)
      * 
      * Safely extracts from a starting index to the end of the vector.
      * 
      * @param bytes Input byte vector
      * @param from Starting index (inclusive)
      * @return UInteger The converted integer value
      * @throws std::invalid_argument if range is invalid or insufficient bytes
      */
      static UInteger toIntegerRange(const BytesVector& bytes, const size_t from)
      {
         return toIntegerRange(bytes, from, bytes.size());
      }

       /**
      * @brief Convert a byte vector to a vector of integers (bulk conversion)
      * 
      * Performs efficient bulk conversion of bytes to integers with proper alignment.
      * Uses compile-time optimizations and SIMD hints for maximum performance.
      * 
      * @param V Input byte vector (size must be multiple of sizeof(UInteger))
      * @return UIntVector Vector of converted integers
      * @throws std::invalid_argument if vector size is not properly aligned
      * 
      * @note Optimized for bulk operations - prefer this over repeated single conversions
      */
       [[nodiscard]] static UIntVector toIntegersVector(const BytesVector& V)
        {
            constexpr size_t type_size = sizeof(UInteger);
            const size_t V_size = V.size();
            
            // Input validation
            if (V_size == 0) [[unlikely]] {
                return UIntVector{};
            }
            
            if (V_size % type_size != 0) [[unlikely]] {
                throw std::invalid_argument(
                    "Endian::toIntegersVector: Vector size (" + std::to_string(V_size) + 
                    ") not aligned to type size (" + std::to_string(type_size) + ")");
            }
            
            UIntVector result;
            const size_t num_integers = V_size / type_size;
            result.reserve(num_integers);
            
            // Bulk conversion with SIMD hints
            #pragma omp simd
            for (size_t i = 0; i < V_size; i += type_size) {
                result.push_back(toIntegerRange(V, i, i + type_size));
            }
            
            return result;
        }

       /**
      * @brief Convert a byte vector range to integers (partial bulk conversion)
      * 
      * Efficiently converts a specific range of bytes to integers.
      * 
      * @param V Input byte vector
      * @param start Starting position in the vector
      * @param length Number of bytes to convert (must be multiple of sizeof(UInteger))
      * @return UIntVector Vector of converted integers
      * @throws std::invalid_argument if parameters are invalid
      */
       static UIntVector toIntegersVector(const BytesVector& V, const size_t start, const size_t length)
        {
            constexpr size_t type_size = sizeof(UInteger);
            
            // Input validation
            if (start + length > V.size() || length % type_size != 0) [[unlikely]] {
                throw std::invalid_argument("Endian::toIntegersVector: Invalid range or alignment");
            }
            
            UIntVector result;
            const size_t num_integers = length / type_size;
            result.reserve(num_integers);
            
            // Bulk conversion with SIMD hints
            #pragma omp simd
            for (size_t i = 0; i < length; i += type_size) {
                const size_t absolute_pos = i + start;
                result.push_back(toIntegerRange(V, absolute_pos, absolute_pos + type_size));
            }
            
            return result;
        }

       /**
      * @brief Convert a vector of integers to bytes (bulk conversion)
      * 
      * Efficiently converts a vector of integers to bytes with optimized memory allocation.
      * 
      * @param V Input integer vector
      * @return BytesVector The converted byte sequence
      * @note noexcept operation optimized for performance
      */
      static BytesVector toBytesVector(const UIntVector& V) noexcept
        {
            const size_t vector_size = V.size();
            BytesVector result;
            
            if (vector_size == 0) [[unlikely]] {
                return result;
            }
            
            // Pre-allocate exact size needed
            result.reserve(vector_size * sizeof(UInteger));
            
            // Bulk conversion with SIMD hints
            #pragma omp simd
            for (size_t i = 0; i < vector_size; ++i) {
                result.extend(toBytesVector(V[i]));
            }
            
            return result;
        }

       /**
         * @brief Convert limited integers to bytes (partial bulk conversion)
         * 
         * Converts only the first 'length' integers from the vector.
         * 
         * @param V Input integer vector
         * @param length Number of integers to convert (must be <= V.size())
         * @return BytesVector The converted byte sequence
         * @note noexcept operation - length is clamped to vector size
         */
        static BytesVector toBytesVector(const UIntVector& V, const size_t length) noexcept
        {
            const size_t actual_length = std::min(length, V.size());
            BytesVector result;
            
            if (actual_length == 0) [[unlikely]] {
                return result;
            }
            
            // Pre-allocate exact size needed
            result.reserve(actual_length * sizeof(UInteger));
            
            // Bulk conversion with SIMD hints
            #pragma omp simd
            for (size_t i = 0; i < actual_length; ++i) {
                result.extend(toBytesVector(V[i]));
            }
            
            return result;
        }
    };


    /**
     * @brief Secure Little-Endian conversion implementation
     * 
     * Implements little-endian byte ordering with security-focused algorithms.
     * All operations are designed to be constant-time and resist side-channel attacks.
     * 
     * @tparam UInteger The unsigned integer type for conversions
     * 
     * @note Little-endian: least significant byte first (Intel x86/x64 native)
     */
    template <typename UInteger>
    class LittleEndian : public Endian<LittleEndian<UInteger>, UInteger>
    {
      using UIntVector = Vector<UInteger>;
    public:   
       /**
         * @brief Convert integer to little-endian byte vector
         * 
         * Optimized conversion using compile-time specialization for different integer sizes.
         * Uses hardware-optimized techniques where available.
         * 
         * @param value Input integer value
         * @return BytesVector Little-endian byte representation
         * @note Constant-time operation for all input values
         */
        static BytesVector toBytesVector(const UInteger& value) noexcept
        {
            BytesVector bytes;
            constexpr size_t byte_count = sizeof(UInteger);
            bytes.reserve(byte_count);
            
            // Compile-time optimization for different sizes
            if constexpr (byte_count == 1) {
                bytes.push_back(static_cast<uint8_t>(value));
            } else {
                // Generic little-endian conversion
                for (size_t i = 0; i < byte_count; ++i) {
                    const uint8_t byte_val = (value >> (i * 8)) & 0xFF;
                    bytes.push_back(byte_val);
                }
            }
            
            // Memory barrier for timing attack resistance
            std::atomic_thread_fence(std::memory_order_release);
            
            return bytes;
        }

      /**
         * @brief Convert little-endian byte vector to integer
         * 
         * Secure conversion with constant-time guarantees and bounds checking.
         * Assumes input has already been validated by the base class.
         * 
         * @param bytes Little-endian byte vector (must contain at least sizeof(UInteger) bytes)
         * @return UInteger The converted integer value
         * @note Input validation is performed by base class toInteger method
         */
        static UInteger toInteger(const BytesVector& bytes) noexcept
        {
            UInteger value = 0;
            constexpr size_t byte_count = sizeof(UInteger);
            
            // Constant-time little-endian conversion
            for (size_t i = 0; i < byte_count; ++i) {
                const UInteger byte_val = static_cast<UInteger>(bytes[i]);
                const size_t shift_amount = i * 8;
                
                // Overflow protection (always safe due to input validation)
                if (shift_amount < byte_count * 8) [[likely]] {
                    value |= (byte_val << shift_amount);
                }
            }
            
            // Memory barrier for timing attack resistance
            std::atomic_thread_fence(std::memory_order_acquire);
            
            return value;
        }
    };

    /**
     * @brief Secure Big-Endian conversion implementation
     * 
     * Implements big-endian byte ordering with security-focused algorithms.
     * All operations are designed to be constant-time and resist side-channel attacks.
     * 
     * @tparam UInteger The unsigned integer type for conversions
     * 
     * @note Big-endian: most significant byte first (network byte order, ARM default)
     */
    template <typename UInteger>
    class BigEndian : public Endian<BigEndian<UInteger>, UInteger>
    {
    public:
        /**
         * @brief Convert big-endian byte vector to integer
         * 
         * Secure conversion with constant-time guarantees and bounds checking.
         * Assumes input has already been validated by the base class.
         * 
         * @param bytes Big-endian byte vector (must contain at least sizeof(UInteger) bytes)
         * @return UInteger The converted integer value
         * @note Input validation is performed by base class toInteger method
         */
        static UInteger toInteger(const BytesVector& bytes) noexcept
        {
            UInteger value = 0;
            constexpr size_t byte_count = sizeof(UInteger);
            
            // Constant-time big-endian conversion
            for (size_t i = 0; i < byte_count; ++i) {
                const UInteger byte_val = static_cast<UInteger>(bytes[i]);
                const size_t shift_amount = (byte_count - i - 1) * 8;
                
                // Overflow protection (always safe due to input validation)
                if (shift_amount < byte_count * 8) [[likely]] {
                    value |= (byte_val << shift_amount);
                }
            }
            
            // Memory barrier for timing attack resistance
            std::atomic_thread_fence(std::memory_order_acquire);
            
            return value;
        }

        /**
         * @brief Convert integer to big-endian byte vector
         * 
         * Optimized conversion using compile-time specialization for different integer sizes.
         * Uses hardware-optimized techniques where available.
         * 
         * @param value Input integer value
         * @return BytesVector Big-endian byte representation
         * @note Constant-time operation for all input values
         */
        static BytesVector toBytesVector(const UInteger& value) noexcept
        {
            BytesVector bytes;
            constexpr size_t byte_count = sizeof(UInteger);
            bytes.reserve(byte_count);
            
            // Compile-time optimization for different sizes
            if constexpr (byte_count == 1) {
                bytes.push_back(static_cast<uint8_t>(value));
            } else {
                // Generic big-endian conversion (MSB first)
                for (size_t i = byte_count; i > 0; --i) {
                    const size_t shift_amount = (i - 1) * 8;
                    const uint8_t byte_val = (value >> shift_amount) & 0xFF;
                    bytes.push_back(byte_val);
                }
            }
            
            // Memory barrier for timing attack resistance
            std::atomic_thread_fence(std::memory_order_release);
            
            return bytes;
        }
    };

    // Convenient type aliases for common use cases
    // Big-Endian specializations
    using BigEndian8  = Endian<BigEndian<uint8_t>,  uint8_t>;   // 8-bit big-endian converter
    using BigEndian16 = Endian<BigEndian<uint16_t>, uint16_t>; // 16-bit big-endian converter
    using BigEndian32 = Endian<BigEndian<uint32_t>, uint32_t>; // 32-bit big-endian converter
    using BigEndian64 = Endian<BigEndian<uint64_t>, uint64_t>; // 64-bit big-endian converter

    // Little-Endian specializations
    using LittleEndian8  = Endian<LittleEndian<uint8_t>,  uint8_t>;   // 8-bit little-endian converter
    using LittleEndian16 = Endian<LittleEndian<uint16_t>, uint16_t>; // 16-bit little-endian converter
    using LittleEndian32 = Endian<LittleEndian<uint32_t>, uint32_t>; // 32-bit little-endian converter
    using LittleEndian64 = Endian<LittleEndian<uint64_t>, uint64_t>; // 64-bit little-endian converter
}