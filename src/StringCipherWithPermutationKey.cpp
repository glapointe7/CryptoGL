#include "StringCipherWithPermutationKey.hpp"

#include <algorithm>
#include <set>

#include "exceptions/EmptyKey.hpp"

using namespace CryptoGL;

bool StringCipherWithPermutationKey::isUniqueWithoutMissingIntegers(const KeyType &key)
{
    const uint32_t key_len = key.size();
    const std::set<int32_t> sorted_key(key.begin(), key.end());
    if (key_len != sorted_key.size())
    {
        return false;
    }

    // Check if all integers are between 0 and the size of the key.
    const uint32_t max_int = *std::max_element(sorted_key.begin(), sorted_key.end());
    if (max_int >= key_len)
    {
        return false;
    }

    return true;
}

void StringCipherWithPermutationKey::setKey(const KeyType &key)
{
    if (key.empty())
    {
        throw EmptyKey("Your key is empty.");
    }

    if (!isUniqueWithoutMissingIntegers(key))
    {
        throw BadPermutationKey("Your permutation key has to contain unique integers and / or integers "
                "between 0 and the size of your key have to be all there.");
    }

    this->key = key;
}