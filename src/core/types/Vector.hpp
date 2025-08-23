/*
 * Add responsabilities to the original std vector.
 */
#pragma once

#include "Types.hpp"
#include "Integer.hpp"

#include <functional>
#include <algorithm>

namespace CryptoGL
{
    template <typename Type>
    class Vector
    {
    private:
        using VectorType = std::vector<Type>;
        using MatrixType = std::vector<VectorType>;

        std::vector<Type> data;

    public:
         // Type aliases
        using value_type = Type;
        using size_type = typename std::vector<Type>::size_type;
        using iterator = typename std::vector<Type>::iterator;
        using const_iterator = typename std::vector<Type>::const_iterator;
        
        Vector() = default;
        explicit Vector(size_type count) : data(count) {}
        Vector(size_type count, const Type& value) : data(count, value) {}
        Vector(std::initializer_list<Type> init) : data(init) {}
        
        // Copy constructor
        Vector(const Vector& other) : data(other.data) {}

        Vector(const VectorType& other) : data(other) {}
        
        // Move constructor (noexcept for optimal performance)
        Vector(Vector&& other) noexcept : data(std::move(other.data)) {}

        template <typename InputIterator>
        Vector(InputIterator first, InputIterator last) : data(first, last) { }
        
        // Destructor with secure memory clearing
        ~Vector() {
            // Overwrite sensitive data before deallocation
            std::fill(data.begin(), data.end(), Type{});
            data.clear();
        }
                
         // Assignment operators
        Vector& operator=(const Vector& other) {
            if (this != &other) {
                data = other.data;
            }
            return *this;
        }
        
        Vector& operator=(Vector&& other) noexcept {
            if (this != &other) {
                data = std::move(other.data);
            }
            return *this;
        }
        
        // Assignment from initializer list
        Vector& operator=(std::initializer_list<Type> init) {
            data = init;
            return *this;
        }
        
        // Subscript operators - CRITICAL for constant-time access
        [[nodiscard]] constexpr Type& operator[](size_type pos) noexcept { 
            return data[pos]; // Direct access - no bounds checking for security
        }
        
        [[nodiscard]] constexpr const Type& operator[](size_type pos) const noexcept { 
            return data[pos]; // Direct access - no bounds checking for security
        }
        
        // Bounds-checked access (use sparingly in crypto code due to timing)
        [[nodiscard]] Type& at(size_type pos) { 
            return data.at(pos); // Delegates to std::vector::at for bounds checking
        }
        
        [[nodiscard]] const Type& at(size_type pos) const { 
            return data.at(pos); // Delegates to std::vector::at for bounds checking
        }
        
        // Constant-time equality operators (secure for cryptographic use)
        [[nodiscard]] constexpr bool operator==(const Vector& other) const noexcept {
            if (data.size() != other.data.size()) {
                return false; // Size mismatch - early return is safe
            }
            
            // Constant-time comparison to prevent timing attacks
            uint8_t diff = 0;
            const size_type size = data.size();
            for (size_type i = 0; i < size; ++i) {
                diff |= static_cast<uint8_t>(data[i] ^ other.data[i]);
            }
            return diff == 0;
        }
        
        [[nodiscard]] constexpr bool operator!=(const Vector& other) const noexcept {
            return !(*this == other);
        }
        
        // Standard (non-constant-time) equality for performance when timing doesn't matter
        [[nodiscard]] bool equals(const Vector& other) const noexcept {
            return data == other.data; // Uses std::vector's optimized comparison
        }
        
        // Controlled access to std::vector interface
        [[nodiscard]] constexpr size_type size() const noexcept { return data.size(); }
        [[nodiscard]] constexpr bool empty() const noexcept { return data.empty(); }
        [[nodiscard]] constexpr size_type capacity() const noexcept { return data.capacity(); }
        
        // Iterator access
        [[nodiscard]] constexpr iterator begin() noexcept { return data.begin(); }
        [[nodiscard]] constexpr const_iterator begin() const noexcept { return data.begin(); }
        [[nodiscard]] constexpr iterator end() noexcept { return data.end(); }
        [[nodiscard]] constexpr const_iterator end() const noexcept { return data.end(); }
        
        // Memory management (cannot be constexpr due to dynamic allocation)
        void reserve(size_type new_cap) { data.reserve(new_cap); }
        void resize(size_type count) { data.resize(count); }
        void resize(size_type count, const Type& value) { data.resize(count, value); }
        
        // Modifiers with security considerations (cannot be constexpr due to potential reallocation)
        void push_back(const Type& value) { data.push_back(value); }
        void push_back(Type&& value) { data.push_back(std::move(value)); }
        
        template<typename... Args>
        Type& emplace_back(Args&&... args) {
            return data.emplace_back(std::forward<Args>(args)...);
        }

        // Conversion methods
        [[nodiscard]] constexpr const std::vector<Type>& std_vector() const noexcept { return data; }
        [[nodiscard]] constexpr std::vector<Type>& std_vector() noexcept { return data; }
        
        // Create a new std::vector copy (for legacy compatibility)
        [[nodiscard]] std::vector<Type> toStdVector() const {
            return std::vector<Type>(data.begin(), data.end());
        }
        
        // Front and back element access
        [[nodiscard]] constexpr Type& front() noexcept { return data.front(); }
        [[nodiscard]] constexpr const Type& front() const noexcept { return data.front(); }
        [[nodiscard]] constexpr Type& back() noexcept { return data.back(); }
        [[nodiscard]] constexpr const Type& back() const noexcept { return data.back(); }
        
        // Direct data pointer access (use with caution)
        [[nodiscard]] constexpr Type* raw_data() noexcept { return data.data(); }
        [[nodiscard]] constexpr const Type* raw_data() const noexcept { return data.data(); }
        
        // Element insertion and deletion
        void pop_back() { data.pop_back(); }
        
        void clear() noexcept { data.clear(); }
        
        // Insert elements at specific positions
        iterator insert(const_iterator pos, const Type& value) {
            return data.insert(pos, value);
        }
        
        iterator insert(const_iterator pos, Type&& value) {
            return data.insert(pos, std::move(value));
        }
        
        template<typename InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            return data.insert(pos, first, last);
        }
        
        // Erase elements
        iterator erase(const_iterator pos) {
            return data.erase(pos);
        }
        
        iterator erase(const_iterator first, const_iterator last) {
            return data.erase(first, last);
        }
        
        // Assign methods
        void assign(size_type count, const Type& value) {
            data.assign(count, value);
        }
        
        template<typename InputIt>
        void assign(InputIt first, InputIt last) {
            data.assign(first, last);
        }
        
        void assign(std::initializer_list<Type> init) {
            data.assign(init);
        }
        
        // Capacity management with security considerations
        void shrink_to_fit() { data.shrink_to_fit(); }
        [[nodiscard]] constexpr size_type max_size() const noexcept { return data.max_size(); }


        
        /* Return a vector containing a range of values in vector V from begin to end. */
        [[nodiscard]] Vector range(const uint64_t& begin, const uint64_t& end) const noexcept {
            return Vector(data.begin() + begin, data.begin() + end);
        }
        
        /* Return a vector containing a range of values in vector V from begin to end of V. */
        [[nodiscard]] Vector range(const uint64_t& begin) const noexcept {
            return Vector(data.begin() + begin, data.end());
        }
        
        // Extend operations (optimized with const references and move semantics)
        void extend(const Vector& W) noexcept {
            const size_type new_size = data.size() + W.data.size();
            data.reserve(new_size);  // Prevent reallocations for security/performance
            data.insert(data.end(), W.data.begin(), W.data.end());
        }
        
        void extend(Vector&& W) noexcept {
            const size_type new_size = data.size() + W.data.size();
            data.reserve(new_size);
            data.insert(data.end(), 
                        std::make_move_iterator(W.data.begin()), 
                        std::make_move_iterator(W.data.end()));
        }
        
        void extend(const Vector& W, const uint64_t& begin, const uint64_t& end) noexcept {
            const size_type range_size = end - begin;
            const size_type new_size = data.size() + range_size;
            data.reserve(new_size);
            data.insert(data.end(), W.data.begin() + begin, W.data.begin() + end);
        }
        
        void extend(const Vector& W, const uint64_t& begin) noexcept {
            const size_type range_size = W.data.size() - begin;
            const size_type new_size = data.size() + range_size;
            data.reserve(new_size);
            data.insert(data.end(), W.data.begin() + begin, W.data.end());
        }

        /* XOR each element of vector V with the elements of vector W and return the result. */
        [[nodiscard]] Vector Xor(const Vector& other) const noexcept {
            const size_type size = this->size();
            Vector result;
            result.reserve(size);  // Prevent reallocations
            
            for (size_type i = 0; i < size; ++i) {
                result.push_back(this->data[i] ^ other.data[i]);
            }
            
            return result;
        }
        
        /* Find a value in the vector and return the position if found, -1 if not. */
        [[nodiscard]] constexpr int64_t find(const Type value) const noexcept {
            int64_t result = -1;
            const size_type size = this->size();
            
            // Always scan entire vector for constant time
            for (size_type i = 0; i < size; ++i) {
                // True constant-time using bitwise operations only
                const Type diff = data[i] ^ value;
                // Create mask: 0 if equal, non-zero if different
                const uint64_t expanded = static_cast<uint64_t>(diff);
                // Convert to all-1s or all-0s mask
                const uint64_t mask = ((expanded | (-expanded)) >> 63) - 1;
                // Update result using bitwise operations (mask=0 means equal)
                result = (result & mask) | (static_cast<int64_t>(i) & ~mask);
            }
            
            return result;
        }
        
        /* Split the vector V in 'vector_size' chunks and return the vector of chunks. */
        [[nodiscard]] Vector<Vector<Type>> chunk(const uint64_t& vector_size) const noexcept {
            if (data.empty()) {
                return {};
            }
            
            const uint64_t V_size = data.size();
            const uint64_t matrix_size = V_size / vector_size;
            Vector<Vector<Type>> result;
            result.reserve(matrix_size);  // Pre-allocate outer vector
            
            for (uint64_t i = 0; i < V_size; i += vector_size) {
                result.emplace_back(range(i, std::min(i + vector_size, V_size)));
            }
            
            return result;
        }
        
        /* Take the contain of this vector and transform it to String.
         * Example: {12, 11, 3, 434} with separator '-' will give the String "12-11-3-434". */
         [[nodiscard]] String join(const char separator) const {
            const uint64_t length = data.size();
            if (length == 0) {
                return String{};
            }
            
            // Pre-calculate buffer size to avoid timing leaks from reallocations
            String result;
            // Conservative estimate for total size
            //result.reserve(length * 10 + length);  // Assume avg 10 chars per element + separators
            
            result.append(Integer<Type>::toString(data[0]));
            for (uint64_t i = 1; i < length; ++i) {
                result.push_back(separator);
                result.append(Integer<Type>::toString(data[i]));
            }
            
            return result;
        }
        
        // Constant-time swap using direct indexing
        void swap(const uint64_t& i, const uint64_t& j) noexcept {
            std::swap(data[i], data[j]);  // Use std::swap for optimal performance
        }
        
        [[nodiscard]] constexpr Type msb() const noexcept {
            if constexpr (std::is_integral_v<Type>) {
                return data[0] & (Type(1) << (sizeof(Type) * 8 - 1));
            } else {
                return data[0];
            }
        }
        
        [[nodiscard]] constexpr Type lsb() const noexcept {
            const size_type last_idx = this->size() - 1;
            return data[last_idx] & Type(1);
        }
        
        /* Shift a vector of integers 'v' to the left of 'shift' bits. */
        [[nodiscard]] Vector leftShift(uint32_t shift) const {
            constexpr uint8_t size_type = sizeof(Type) * 8;
            const uint32_t v_size = data.size();
            const uint32_t start = std::min(shift / size_type, v_size);
            Vector result = range(start);
            result.reserve(v_size);
            
            result.data.insert(result.data.end(), start, Type{});
            
            if (shift % size_type == 0 || shift >= size_type * v_size) {
                return result;
            }
            
            shift %= size_type;
            std::transform(result.data.rbegin(), result.data.rend(), 
                          result.data.rbegin(), LeftShift(shift));
            
            return result;
        }
        
        [[nodiscard]] Vector rightShift(uint32_t shift) const {
            const uint8_t size_type = sizeof(Type) * 8;
            const uint32_t v_size = data.size();
            const uint32_t start = std::min(shift / size_type, v_size);
            Vector result(start, Type{});
            result.extend(range(0), 0, v_size - start);
            
            if (shift % size_type == 0 || shift >= size_type * v_size) {
                return result;
            }
            
            shift %= size_type;
            std::transform(result.data.begin(), result.data.end(), 
                          result.data.begin(), RightShift(shift));
            
            return result;
        }
        
        /* Rotate a vector of integers 'V' to the left of 'to_rotate' bits. */
        [[nodiscard]] Vector rotateLeft(const uint32_t to_rotate) const {
            constexpr uint8_t size_type = sizeof(Type) * 8;
            const uint8_t bytes_to_rotate = to_rotate / size_type;
            const uint8_t bits_to_shift = to_rotate % size_type;
            const uint8_t rest = size_type - bits_to_shift;
            const uint64_t V_size = data.size();
            
            Vector rotated_bytes;
            rotated_bytes.reserve(V_size);  // Pre-allocate for performance
            
            for (uint8_t i = 0; i < V_size; ++i) {
                rotated_bytes.push_back((data[(i + bytes_to_rotate) % V_size] << bits_to_shift) |
                                         (data[(i + bytes_to_rotate + 1) % V_size] >> rest));
            }
            
            return rotated_bytes;
        }
        
        /* Rotate a vector of integers 'V' to the right of 'to_rotate' bits. */
        [[nodiscard]] Vector rotateRight(const uint32_t to_rotate) const {
            const uint8_t size_type = sizeof(Type) * 8;
            const uint8_t bytes_to_rotate = to_rotate / size_type;
            const uint8_t bits_to_shift = to_rotate % size_type;
            const uint8_t rest = size_type - bits_to_shift;
            const uint64_t V_size = data.size();
            
            Vector rotated_bytes;
            rotated_bytes.reserve(V_size);  // Pre-allocate for performance
            
            for (uint8_t i = 0; i < V_size; ++i) {
                rotated_bytes.push_back((data[(i + bytes_to_rotate) % V_size] >> bits_to_shift) |
                                         (data[(i + bytes_to_rotate + 1) % V_size] << rest));
            }
            
            return rotated_bytes;
        }
        
        /* Transform a vector of integers to a hexadecimal string. */
        [[nodiscard]] String toHexString() const {
            const uint8_t data_size = sizeof(Type) * 8;
            const uint64_t vect_size = data.size();
            String hex_digest;
            //hex_digest.reserve(vect_size * data_size / 4); 
            
            for (uint64_t i = 0; i < vect_size; ++i) {
                for (int8_t j = data_size - 4; j >= 0; j -= 4) {
                    hex_digest.push_back(String::hex_digits[(data[i] >> j) & 0xF]);
                }
            }
            
            return hex_digest;
        }
        
        /* Convert a Vector of bytes to a string. */
        [[nodiscard]] String toString() const {
            return String(data.begin(), data.end());
        }
        
        /* Convert a Vector to std::vector. */
        Vector toStdVector()
        {
            return Vector(this->begin(), this->end());
        }

    private:
        class LeftShift
        {
        public:
        explicit LeftShift(const uint32_t to_shift)
            : carry(0), mask(0xFF << (8 - to_shift)),
            shift(to_shift), inner_shift(8 - to_shift) {}
        
        uint8_t operator()(uint8_t value) {
            const uint8_t direction = value & mask;
            value <<= shift;
            value |= carry;
            
            carry = direction >> inner_shift;
            return value;
        }

        private:
            uint8_t carry;
            const uint8_t mask;
            const uint32_t shift;
            const uint32_t inner_shift;
        };

        
        class RightShift
        {
        public:
        explicit RightShift(const uint32_t to_shift)
            : carry(0), mask(0xFF >> (8 - to_shift)),
            shift(to_shift), inner_shift(8 - to_shift) {}
        
        uint8_t operator()(uint8_t value) {
            const uint8_t direction = value & mask;
            value >>= shift;
            value |= carry;
            
            carry = direction << inner_shift;
            return value;
        }

        private:
            uint8_t carry;
            const uint8_t mask;
            const uint32_t shift;
            const uint32_t inner_shift;
        };
    };
}