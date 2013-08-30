
#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>
#include <stdint.h>

/* For the string ciphers. */
typedef std::string ClassicalType;

typedef std::vector<uint8_t> BytesVector;
typedef std::vector<uint16_t> UInt16Vector;
typedef std::vector<uint32_t> UInt32Vector;
typedef std::vector<uint64_t> UInt64Vector;

typedef std::vector<int32_t> Int32Vector;

typedef std::vector<BytesVector> BytesMatrix;
typedef std::vector<std::vector<int32_t> > Int32Matrix;
typedef std::vector<UInt64Vector> UInt64Matrix;

typedef BytesMatrix IV_Vector;

#endif