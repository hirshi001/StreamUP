#pragma once
#include "Data/buffer/BufferUtil.h"

#include <cstdint>
#include <type_traits>


namespace SUP::BufferUtil
{
class ReadBufferWrapper
{
public:
    ReadBufferWrapper(const uint8_t *data, size_t length) : data(data), length(length), readIndex(0) {}

    template<typename T>
    void read(T &value)
        requires std::is_arithmetic_v<T> { readIndex += BufferUtil::read<T>(data + readIndex, value); }

    template<typename T>
    T read()
        requires std::is_arithmetic_v<T>
    {
        T tmp;
        readIndex += BufferUtil::read<T>(data + readIndex, tmp);
        return tmp;
    }

    void readToArray(uint8_t *dest, size_t numBytes)
    {
        std::copy_n(data + readIndex, numBytes, dest);
        readIndex += numBytes;
    }

    void setReadIndex(size_t index) { readIndex = index; }

    [[nodiscard]] size_t getReadIndex() const { return readIndex; }

    [[nodiscard]] size_t getReadableBytes() const { return length - readIndex; }

    [[nodiscard]] bool ensureReadableBytes(size_t numBytes) const { return getReadableBytes() >= numBytes; }

private:
    const uint8_t *data;
    size_t length;
    size_t readIndex;
};
}
