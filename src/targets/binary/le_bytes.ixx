module recurseria.target.binary:le_bytes;

import std;

namespace recurseria::target::binary {

    template <std::unsigned_integral U>
    constexpr std::vector<std::byte> to_le_bytes(U value) {
        std::vector<std::byte> bytes(sizeof(U));
        for (std::size_t i = 0; i < sizeof(U); ++i) {
            bytes[i] = static_cast<std::byte>(static_cast<std::uint8_t>(value >> (8 * i)));
        }
        return bytes;
    }

    template <std::unsigned_integral U>
    constexpr U from_le_bytes(const std::vector<std::byte>& input, std::size_t offset) {
        if (offset + sizeof(U) > input.size()) {
            throw std::out_of_range("binary: truncated input");
        }
        U value = 0;
        for (std::size_t i = 0; i < sizeof(U); ++i) {
            value |= static_cast<U>(input[offset + i]) << (8 * i);
        }
        return value;
    }

    template <std::integral T>
    constexpr std::vector<std::byte> to_le_bytes(T value) {
        return to_le_bytes(std::bit_cast<std::make_unsigned_t<T>>(value));
    }

    template <std::integral T>
    constexpr T from_le_bytes(const std::vector<std::byte>& input, std::size_t offset) {
        return std::bit_cast<T>(from_le_bytes<std::make_unsigned_t<T>>(input, offset));
    }
}
