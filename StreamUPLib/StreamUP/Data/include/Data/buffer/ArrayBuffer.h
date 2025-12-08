#pragma once


namespace SUP::BufferUtil
{


template<typename T>
void writeInt(uint8_t* data, T value)
{
    static_assert(std::is_integral_v<T>, "writeInt requires an integer type");

    constexpr size_t N = sizeof(T);
    for (int i = 0; i < N; ++i) {
        data[i] = static_cast<uint8_t>(value >> (8 * (N - 1 - i))); // MSB first
    }
}

template<typename T>
T readInt(const uint8_t* data)
{
    static_assert(std::is_integral_v<T>, "readInt requires an integer type");

    T value = 0;
    constexpr size_t N = sizeof(T);

    for (size_t i = 0; i < N; ++i) {
        value |= static_cast<T>(data[i]) << (8 * (N - 1 - i));
    }

    return value;
}


template<typename T>
void writeIntUpdate(uint8_t*& data, T value)
{
    writeInt(data, value);
    data += sizeof(T);
}

template<typename T>
T readIntUpdate(const uint8_t*& data)
{
    T value = readInt<T>(data);
    data += sizeof(T);
    return value;
}


}


