#pragma once


namespace SUP::BufferUtil
{
template<typename T>
size_t writeInt(uint8_t *data, T value)
    requires std::is_integral_v<T>
{
    constexpr size_t N = sizeof(T);
    for (int i = 0; i < N; ++i)
    {
        data[i] = static_cast<uint8_t>(value >> (8 * (N - 1 - i))); // MSB first
    }
    return N;
}

template<typename T>
size_t readInt(const uint8_t *data, T &value)
    requires std::is_integral_v<T>
{
    value = 0;
    constexpr size_t N = sizeof(T);

    for (size_t i = 0; i < N; ++i)
    {
        value |= static_cast<T>(data[i]) << (8 * (N - 1 - i));
    }
    return N;
}

template<typename T>
size_t writeFloat(uint8_t *data, T value)
    requires std::is_floating_point_v<T>
{
    // Copy bytes, then ensure big-endian
    constexpr size_t N = sizeof(T);

    uint8_t tmp[N];
    std::memcpy(tmp, &value, N);

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    // Reverse bytes for network byte order
    for (size_t i = 0; i < N; ++i)
    {
        data[i] = tmp[N - 1 - i];
    }
#else
    std::memcpy(data, tmp, N);
#endif
    return N;
}


template<typename T>
size_t readFloat(const uint8_t *data, T &value)
    requires std::is_floating_point_v<T>
{
    constexpr size_t N = sizeof(T);
    uint8_t tmp[N];

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    // Reverse from network byte order to native
    for (size_t i = 0; i < N; ++i)
    {
        tmp[i] = data[N - 1 - i];
    }
#else
    std::memcpy(tmp, data, N);
#endif

    value = 0;
    std::memcpy(&value, tmp, N);
    return N;
}

template<typename T>
size_t write(uint8_t *data, T value)
    requires std::is_arithmetic_v<T>
{
    if constexpr (std::is_integral_v<T>)
    {
        return writeInt<T>(data, value);
    } else if constexpr (std::is_floating_point_v<T>)
    {
        return writeFloat<T>(data, value);
    }
}


template<typename T>
size_t read(const uint8_t *data, T& value)
    requires std::is_arithmetic_v<T>
{
    if constexpr (std::is_integral_v<T>)
    {
        return readInt<T>(data, value);
    } else if constexpr (std::is_floating_point_v<T>)
    {
        return readFloat<T>(data, value);
    }
}
}
