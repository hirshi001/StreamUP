#pragma once

#include "Data/buffer/BufferUtil.h"

#include <cstdint>
#include <type_traits>


namespace SUP::BufferUtil
{
class WriteBufferWrapper
{
public:
    WriteBufferWrapper(uint8_t *data, int length) : data(data), length(length), writeIndex(0) {}

    template<typename T>
    size_t write(T &value)
        requires std::is_arithmetic_v<T>
    {
        size_t written = BufferUtil::write<T>(data + writeIndex, value);
        writeIndex += static_cast<int>(written);
        return written;
    }

    void writeFromArray(uint8_t *src, int numBytes)
    {
        std::copy_n(src, numBytes, data + writeIndex);
        writeIndex += numBytes;
    }

    void setWriteIndex(int index) { writeIndex = index; }

    [[nodiscard]] int getWriteIndex() const { return writeIndex; }

    [[nodiscard]] int getWritableBytes() const { return length - writeIndex; }

    [[nodiscard]] bool ensureWritableBytes(int numBytes) const { return getWritableBytes() >= numBytes; }

    uint8_t *data;

private:
    int length;
    int writeIndex;
};
}
