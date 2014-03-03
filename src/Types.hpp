
#ifndef TYPES_HPP
#define TYPES_HPP

#include "String.hpp"
#include "Vector.hpp"
#include <stdint.h>

/* For the string ciphers. */
using ClassicalType = String;

/* For strings in the test section for all cryptos except string ciphers. */
using StringTest = String;

using BytesVector = Vector<uint8_t>;
using UInt16Vector = Vector<uint16_t>;
using UInt32Vector = Vector<uint32_t>;
using UInt64Vector = Vector<uint64_t>;

using Int32Vector = Vector<int32_t>;

using BytesMatrix = Vector<BytesVector>;
using UInt16Matrix = Vector<UInt16Vector>;
using Int32Matrix = Vector<Int32Vector>;
using UInt32Matrix = Vector<UInt32Vector>;
using UInt64Matrix = Vector<UInt64Vector>;

using IV_Vector = BytesMatrix;

#endif