#pragma once


namespace SUP::BufferUtil
{
template<typename T>
constexpr size_t writeInt(uint8_t *data, T value)
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
constexpr size_t readInt(const uint8_t *data, T &value)
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
constexpr size_t writeFloat(uint8_t *data, const T& value)
    requires std::is_floating_point_v<T>
{
    // Copy bytes, then ensure big-endian
    constexpr size_t N = sizeof(T);

    auto bytes = std::bit_cast<std::array<uint8_t, N>>(value);

    if constexpr (std::endian::native == std::endian::little)
        std::reverse(bytes.begin(), bytes.end());

    for (size_t i = 0; i < N; ++i)
        data[i] = bytes[i];
    return N;
}


template<typename T>
constexpr size_t readFloat(const uint8_t *data, T &value)
    requires std::is_floating_point_v<T>
{
    constexpr size_t N = sizeof(T);
    std::array<uint8_t, N> bytes;

    for (size_t i = 0; i < N; ++i)
        bytes[i] = data[i];

    if constexpr (std::endian::native == std::endian::little)
        std::reverse(bytes.begin(), bytes.end());

    value = std::bit_cast<T>(bytes);
    return N;
}

template<typename T>
constexpr size_t write(uint8_t *data, T value)
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
constexpr size_t read(const uint8_t *data, T& value)
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
