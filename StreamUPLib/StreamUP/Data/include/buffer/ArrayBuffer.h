#pragma once
#include <vector>

namespace SUP
{

namespace BufferUtil
{


template<typename T>
    void writeInt(uint8_t* data, T value)
{
    static_assert(std::is_integral_v<T>, "writeInt requires an integer type");

    constexpr size_t N = sizeof(T);
    for (int i = N - 1; i >= 0; --i) {
        *data = static_cast<uint8_t>(value >> (8 * i));
        data++;
    }
}

template<typename T>
T readInt(uint8_t* data)
{
    static_assert(std::is_integral_v<T>, "readInt requires an integer type");
    T value;
    constexpr size_t N = sizeof(T);
    for (int i = N - 1; i >= 0; --i) {
        value |= data[i] << (8 * i);
        data++;
    }
    return value;
}

}

}
