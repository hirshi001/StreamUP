#pragma once


namespace SUP::BufferUtil
{

template<typename T>
void writeInt(uint8_t* data, T value)
requires std::is_integral_v<T>
{

    constexpr size_t N = sizeof(T);
    for (int i = 0; i < N; ++i) {
        data[i] = static_cast<uint8_t>(value >> (8 * (N - 1 - i))); // MSB first
    }
}

template<typename T>
T readInt(const uint8_t* data)
requires std::is_integral_v<T>
{
    T value = 0;
    constexpr size_t N = sizeof(T);

    for (size_t i = 0; i < N; ++i) {
        value |= static_cast<T>(data[i]) << (8 * (N - 1 - i));
    }

    return value;
}


template<typename T>
void writeIntUpdate(uint8_t*& data, T value)
requires std::is_integral_v<T>
{
    writeInt(data, value);
    data += sizeof(T);
}

template<typename T>
T readIntUpdate(const uint8_t*& data)
requires std::is_integral_v<T>
{
    T value = readInt<T>(data);
    data += sizeof(T);
    return value;
}

template<typename T>
void writeFloat(uint8_t* data, T value)
requires std::is_floating_point_v<T>
{
    // Copy bytes, then ensure big-endian
    uint8_t tmp[sizeof(T)];
    std::memcpy(tmp, &value, sizeof(T));

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    // Reverse bytes for network byte order
    for (size_t i = 0; i < sizeof(T); ++i) {
        data[i] = tmp[sizeof(T) - 1 - i];
    }
#else
    std::memcpy(data, tmp, sizeof(T));
#endif
    data += sizeof(T);
}


template<typename T>
T readFloat(const uint8_t* data)
requires std::is_floating_point_v<T>
{
    uint8_t tmp[sizeof(T)];

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    // Reverse from network byte order to native
    for (size_t i = 0; i < sizeof(T); ++i) {
        tmp[i] = data[sizeof(T) - 1 - i];
    }
#else
    std::memcpy(tmp, data, sizeof(T));
#endif

    T value;
    std::memcpy(&value, tmp, sizeof(T));
    return value;
}


template<typename T>
void writeFloatUpdate(uint8_t*& data, T value)
requires std::is_floating_point_v<T>
{
    writeFloat<T>(data, value);
    data += sizeof(T);
}

template<typename T>
T readFloatUpdate(const uint8_t*& data)
requires std::is_floating_point_v<T>
{
    T value = readFloat<T>(data);
    data += sizeof(T);
    return value;
}

template<typename T>
void write(uint8_t* data, T value)
requires std::is_arithmetic_v<T>
{
    if constexpr (std::is_integral_v<T>) {
        writeInt<T>(data, value);
    }
    else if constexpr (std::is_floating_point_v<T>) {
        writeFLoat<T>(data, value);
    }
}


template<typename T>
T read(const uint8_t* data, T value)
requires std::is_arithmetic_v<T>
{
    if constexpr (std::is_integral_v<T>) {
        return readInt<T>(data, value);
    }
    else if constexpr (std::is_floating_point_v<T>) {
        return readFloat<T>(data, value);
    }
}

template<typename T>
void writeUpdate(uint8_t*& data, T value)
requires std::is_arithmetic_v<T>
{
    if constexpr (std::is_integral_v<T>) {
        writeIntUpdate<T>(data, value);
    }
    else if constexpr (std::is_floating_point_v<T>) {
        writeFloatUpdate<T>(data, value);
    }
}


template<typename T>
T readUpdate(const uint8_t*& data)
requires std::is_arithmetic_v<T>
{
    if constexpr (std::is_integral_v<T>) {
        return readIntUpdate<T>(data);
    }
    else if constexpr (std::is_floating_point_v<T>) {
        return readFloatUpdate<T>(data);
    }
}


}


