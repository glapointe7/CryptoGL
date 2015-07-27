#include "Hill.hpp"

#include "exceptions/EmptyMatrix.hpp"
#include "exceptions/MatrixNotSquare.hpp"
#include "exceptions/MatrixKeyNotReversible.hpp"
#include "exceptions/EmptyKey.hpp"

#include "MathematicalTools.hpp" 

using namespace CryptoGL;

void Hill::setKey(const Int64Matrix &key)
{
    try
    {
        this->key.setMatrix(key);
        this->key.setModulo(alpha.length());
    }
    catch (const EmptyMatrix &EM)
    {
        throw EM.what();
    }
    catch (const MatrixNotSquare &MNS)
    {
        throw MNS.what();
    }

    if (!Maths::areCoprimes(this->key.det(), this->key.getModulo()))
    {
        throw MatrixKeyNotReversible("Your matrix key must be reversible to be able to decode the message.");
    }
}

ClassicalType Hill::process(const ClassicalType &data) const
{
    const uint32_t key_dim = key.getDimension();
    const uint32_t data_len = data.length();
    ClassicalType message(data_len + key_dim);
    Int64Vector pos;
    pos.reserve(key_dim);

    for (uint32_t i = 0; i < data_len; i += key_dim)
    {
        for (uint32_t j = 0; j < key_dim; ++j)
        {
            pos.push_back(alpha.find(data[i + j]));
        }
        const Int64Vector cipher_pos = key.multiplyByVector(pos);

        for (const auto x : cipher_pos)
        {
            message.push_back(alpha[x]);
        }
        pos.clear();
    }

    return message;
}

ClassicalType Hill::encode(const ClassicalType &clear_text)
{
    const ClassicalType full_text = appendChars(clear_text, key.getDimension(), 'X');

    return process(full_text);
}

ClassicalType Hill::decode(const ClassicalType &cipher_text)
{
    key.inverse();
    return process(cipher_text);
}