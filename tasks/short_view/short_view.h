#pragma once

#include <stdexcept>
#include <string>
#include <iostream>

class ShortView {
public:
    ShortView(const char *data, uint16_t size) {
        data_ = reinterpret_cast<char*>(reinterpret_cast<uint64_t>(data) & 0x0000FFFFFFFFFFFF | (static_cast<uint64_t>(size) << 48));
    }

    [[nodiscard]] size_t Size() const {
        return reinterpret_cast<uint64_t>(data_) >> 48;
    }

    [[nodiscard]] const char *Data() const {
        uint64_t mask = 1ULL << 47;
        if (reinterpret_cast<uint64_t>(data_) & mask)
            return reinterpret_cast<char*>(reinterpret_cast<uint64_t>(data_ )| 0xFFFF000000000000);
        return reinterpret_cast<char*>(reinterpret_cast<uint64_t>(data_ ) & 0x0000FFFFFFFFFFFF);;
    }

    [[nodiscard]] std::string_view ToStringView() const {
        return std::string_view(Data(), Size());
    }

private:
    const char *data_;
};
