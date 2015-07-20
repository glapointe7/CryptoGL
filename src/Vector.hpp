/*
 * Add responsabilities to the original std vector.
 */
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Types.hpp"
#include "Integer.hpp"

#include <functional>
#include <algorithm>

namespace CryptoGL
{
    /* Extension of std::vector. */
    template <typename Type>
    class Vector : public std::vector<Type>
    {
    private:
        using VectorType = std::vector<Type>;
        using MatrixType = std::vector<VectorType>;

    public:
        /* Create an empty vector. */
        Vector() : VectorType(0) { }
        
        /* Create an empty vector with memory reserved. */
        explicit Vector(const uint64_t &new_size)
        {
            this->reserve(new_size);
        }
        
        /* Create a copy of a std::vector in Vector. */
        explicit Vector(const VectorType &V) : VectorType(V) { }
        
        template <class InputIterator>
        Vector(InputIterator first, InputIterator last) : VectorType(first, last) { }
        
        /* Create a Vector filled with 'n' times the value 'value'. */
        Vector(const uint64_t &n, const Type value) : VectorType(n, value) { }
        
        /* Create a vector with a list of initialized values. */
        Vector(std::initializer_list<Type> init_list) : VectorType(init_list) { }
        
        /* Move Constructor. */
        Vector(VectorType&& V) : VectorType(V) { }
        
        /* XOR each element of vector V with the elements of vector W and return the result. */
        Vector Xor(const Vector &W) const
        {
            const uint64_t size = this->size();
            Vector out;
            out.reserve(size);

            for (uint64_t i = 0; i < size; ++i)
            {
                out.push_back(this->at(i) ^ W[i]);
            }

            return out;
        }
        
        /* Return a vector containing a range of values in vector V from begin to end. */
        Vector range(const uint64_t &begin, const uint64_t &end) const
        {
            return Vector(this->begin() + begin, this->begin() + end);
        }
        
        /* Return a vector containing a range of values in vector V from begin to end of V. */
        Vector range(const uint64_t &begin) const
        {
            return Vector(this->begin() + begin, this->end());
        }
        
        /* Append the vector W at the end of V. */
        void extend(Vector W)
        {
            //this->reserve(this->size() + W.size());
            this->insert(this->end(), W.begin(), W.end());
        }
        
        /* Append the range of W given from begin to end at the end. */
        void extend(Vector W, const uint64_t &begin, const uint64_t &end)
        {
            //this->reserve(this->size() + std::distance(W.begin() + begin, W.begin() + end));
            this->insert(this->end(), W.begin() + begin, W.begin() + end);
        }
        
        /* Append the range of W given from begin to end at the end of V. */
        void extend(Vector W, const uint64_t &begin)
        {
            //this->reserve(this->size() + std::distance(W.begin() + begin, W.end()));
            this->insert(this->end(), W.begin() + begin, W.end());
        }
        
        /* Find a value in the vector and return the position if found, -1 if not. */
        int64_t find(const Type value) const
        {
            const auto it = std::find(this->begin(), this->end(), value);
            if (it == this->end())
            {
                return -1;
            }

            return std::distance(this->begin(), it);
        }
        
        /* Split the vector V in 'vector_size' chunks and return the vector of chunks. */
        Vector<Vector<Type>> chunk(const uint64_t &vector_size) const
        {
            if (this->empty())
            {
                return {};
            }

            const uint64_t V_size = this->size();
            const uint64_t matrix_size = V_size / vector_size;

            Vector<Vector<Type>> result(matrix_size);
            for (uint64_t i = 0; i < V_size; i += vector_size)
            {
                result.emplace_back(range(i, i + vector_size));
            }

            return result;
        }
        
        /* Take the contain of this vector and transform it to String.
         * Example: {12, 11, 3, 434} with separator '-' will give the String "12-11-3-434". */
        String join(const char separator)
        {
            const uint64_t length = this->size();
            String result = Integer<Type>::toString(this->at(0));
            for (uint64_t i = 1; i < length; ++i)
            {
                result.push_back(separator);
                result.append(Integer<Type>::toString(this->at(i)));
            }

            return result;
        }
        
        /* Swap the positions i and j in the current vector. */
        void swap(const uint64_t &i, const uint64_t &j)
        {
            const Type tmp = this->at(i);
            this->at(i) = this->at(j);
            this->at(j) = tmp;
        }
        
        Type msb() const
        {
            return *(this->begin()) & 0x80;
        }
        
        Type lsb() const
        {
            return *(this->rbegin()) & 0x01;
        }
        
        /* Shift a vector of integers 'v' to the left of 'shift' bits. */
        Vector leftShift(uint32_t shift) const
        {
            const uint8_t size_type = sizeof(Type) * 8;
            const uint32_t v_size = this->size();
            const uint32_t start = std::min(shift / size_type, v_size);
            Vector result(range(start));
            result.reserve(v_size);

            result.insert(result.end(), start, 0);

            if (shift % size_type == 0 || shift >= size_type * v_size)
            {
                return result;
            }

            shift %= size_type;
            std::transform(result.rbegin(), result.rend(), result.rbegin(), LeftShift(shift));

            return result;
        }
        
        Vector rightShift(uint32_t shift) const
        {
            const uint8_t size_type = sizeof(Type) * 8;
            const uint32_t v_size = this->size();
            const uint32_t start = std::min(shift / size_type, v_size);
            Vector result(start, 0);
            result.extend(range(0), 0, v_size - start);

            if (shift % size_type == 0 || shift >= size_type * v_size)
            {
                return result;
            }

            shift %= size_type;
            std::transform(result.begin(), result.end(), result.begin(), RightShift(shift));

            return result;
        }
        
        /* Rotate a vector of integers 'V' to the left of 'to_rotate' bits. */
        Vector rotateLeft(const uint32_t to_rotate) const
        {
            const uint8_t size_type = sizeof(Type) * 8;
            const uint8_t bytes_to_rotate = to_rotate / size_type;
            const uint8_t bits_to_shift = to_rotate % size_type;
            const uint8_t rest = size_type - bits_to_shift;
            const uint64_t V_size = this->size();

            Vector rotated_bytes(V_size);
            for (uint8_t i = 0; i < V_size; ++i)
            {
                rotated_bytes.push_back((this->at((i + bytes_to_rotate) % V_size) << bits_to_shift)
                        | (this->at((i + bytes_to_rotate + 1) % V_size) >> rest));
            }

            return rotated_bytes;
        }
        
        /* Rotate a vector of integers 'V' to the right of 'to_rotate' bits. */
        Vector rotateRight(const uint32_t to_rotate) const
        {
            const uint8_t size_type = sizeof(Type) * 8;
            const uint8_t bytes_to_rotate = to_rotate / size_type;
            const uint8_t bits_to_shift = to_rotate % size_type;
            const uint8_t rest = size_type - bits_to_shift;
            const uint64_t V_size = this->size();

            Vector rotated_bytes(V_size);
            for (uint8_t i = 0; i < V_size; ++i)
            {
                rotated_bytes.push_back((this->at((i + bytes_to_rotate) % V_size) >> bits_to_shift)
                        | (this->at((i + bytes_to_rotate + 1) % V_size) << rest));
            }

            return rotated_bytes;
        }
        
        /* Transform a vector of integers to a hexadecimal string. */
        String toHexString() const
        {
            const uint8_t data_size = sizeof(Type) * 8;
            const uint64_t vect_size = this->size();
            String hex_digest;
            hex_digest.reserve(vect_size * data_size / 4);
            for (uint64_t i = 0; i < vect_size; ++i)
            {
                for (int8_t j = data_size - 4; j >= 0; j -= 4)
                {
                    hex_digest.push_back(String::hex_digits[(this->at(i) >> j) & 0xF]);
                }
            }

            return hex_digest;
        }
        
        /* Convert a Vector of bytes to a string. */
        String toString() const
        {
            return String(this->begin(), this->end());
        }
        
        /* Convert a Vector to std::vector. */
        VectorType toStdVector()
        {
            return VectorType(this->begin(), this->end());
        }

    private:
        class LeftShift
        {
        public:
            LeftShift(const uint32_t to_shift)
                : carry(0), mask(0xFF << (8 - to_shift)),
                  shift(to_shift), inner_shift(8 - to_shift) { }
            
            uint8_t operator()(uint8_t value)
            {
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
            RightShift(const uint32_t to_shift)
                : carry(0), mask(0xFF >> (8 - to_shift)),
                  shift(to_shift), inner_shift(8 - to_shift) { }
            
            uint8_t operator()(uint8_t value)
            {
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

#endif