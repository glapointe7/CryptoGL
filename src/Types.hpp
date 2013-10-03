
#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>
#include <stdint.h>

/* For the string ciphers. */
using ClassicalType = std::string;

using BytesVector = std::vector<uint8_t>;
using UInt16Vector = std::vector<uint16_t>;
using UInt32Vector = std::vector<uint32_t>;
using UInt64Vector = std::vector<uint64_t>;

using Int32Vector = std::vector<int32_t>;

using BytesMatrix = std::vector<BytesVector>;
using UInt16Matrix = std::vector<UInt16Vector>;
using Int32Matrix = std::vector<Int32Vector>;
using UInt32Matrix = std::vector<UInt32Vector>;
using UInt64Matrix = std::vector<UInt64Vector>;

using IV_Vector = BytesMatrix;

#endif