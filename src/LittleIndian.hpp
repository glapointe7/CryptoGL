
#ifndef LITTLE_INDIAN_HPP
#define LITTLE_INDIAN_HPP

template <class T>
class LittleIndian {
    T value = 0u;
    const uint32_t mod = sizeof(T);

public:
    void add(const uint8_t byte, const uint32_t offset) 
    {
        value |= byte << (8 * (offset % mod));
    }

    T getValue() const { return value; }
    void reset() { value = 0; }
};

using LittleIndian4Bytes = LittleIndian<uint32_t>;
using LittleIndian8Bytes = LittleIndian<uint64_t>;

#endif

