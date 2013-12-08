#include "MAC.hpp"

template<>
const BytesVector Constant<8>::msb_value = {0,0,0,0,0,0,0,0x1B};

template <>
const BytesVector Constant<16>::msb_value = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x87};

template<>
const BytesVector Constant<8>::lsb_value = {0x80,0,0,0,0,0,0,0x0D};

template <>
const BytesVector Constant<16>::lsb_value = {0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x43};