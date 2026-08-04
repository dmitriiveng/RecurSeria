export module recurseria.target.binary:primitives;

import std;

import recurseria.core.meta.types_srlz_dsrlz;
import :format_tag;
import :le_bytes;

// helpers

namespace recurseria::target::binary {

    static_assert(sizeof(int) == 4, "binary: int must be 32-bit for the canonical wire format");
    static_assert(sizeof(long) == 4 || sizeof(long) == 8, "binary: unsupported long width");
    static_assert(sizeof(long long) == 8, "binary: long long must be 64-bit for the canonical wire format");

    template <std::floating_point T, std::unsigned_integral U>
    constexpr std::vector<std::byte> float_to_le_bytes(T value) {
        static_assert(sizeof(T) == sizeof(U));
        return to_le_bytes(std::bit_cast<U>(value));
    }

    template <std::floating_point T, std::unsigned_integral U>
    constexpr T float_from_le_bytes(const std::vector<std::byte>& input, std::size_t offset) {
        static_assert(sizeof(T) == sizeof(U));
        return std::bit_cast<T>(from_le_bytes<U>(input, offset));
    }

    void append_u64(std::vector<std::byte>& output, std::uint64_t value) {
        auto bytes = to_le_bytes(value);
        output.insert(output.end(), bytes.begin(), bytes.end());
    }

    std::uint64_t read_u64(const std::vector<std::byte>& input, std::size_t offset) {
        return from_le_bytes<std::uint64_t>(input, offset);
    }

    std::vector<std::byte> string_to_bytes(const std::string& input) {
        std::vector<std::byte> result;
        result.reserve(sizeof(std::uint64_t) + input.size());
        append_u64(result, static_cast<std::uint64_t>(input.size()));
        for (char c : input) {
            result.push_back(static_cast<std::byte>(static_cast<unsigned char>(c)));
        }
        return result;
    }

    std::string string_from_bytes(const std::vector<std::byte>& input) {
        constexpr std::size_t len_size = sizeof(std::uint64_t);
        std::uint64_t len = read_u64(input, 0);
        if (static_cast<std::uint64_t>(input.size()) < len_size + len) {
            throw std::out_of_range("binary: truncated string");
        }
        std::string result;
        result.reserve(static_cast<std::size_t>(len));
        for (std::uint64_t i = 0; i < len; ++i) {
            result.push_back(static_cast<char>(input[len_size + static_cast<std::size_t>(i)]));
        }
        return result;
    }
}

// functions

export namespace recurseria::target::binary {
    using serialize_tag = recurseria::core::meta::serialize_tag;
    using deserialize_tag = recurseria::core::meta::deserialize_tag;

    template<typename T>
    using type_tag = recurseria::core::meta::type_tag<T>;

    // serialize

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const int& input) {
        return to_le_bytes(static_cast<std::int32_t>(input));
    }

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const long& input) {
        return to_le_bytes(static_cast<std::int64_t>(input));
    }

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const long long& input) {
        return to_le_bytes(static_cast<std::int64_t>(input));
    }

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const unsigned int& input) {
        return to_le_bytes(static_cast<std::uint32_t>(input));
    }

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const unsigned long& input) {
        return to_le_bytes(static_cast<std::uint64_t>(input));
    }

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const unsigned long long& input) {
        return to_le_bytes(static_cast<std::uint64_t>(input));
    }

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const float& input) {
        return float_to_le_bytes<float, std::uint32_t>(input);
    }

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const double& input) {
        return float_to_le_bytes<double, std::uint64_t>(input);
    }

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const long double& input) {
        return float_to_le_bytes<double, std::uint64_t>(static_cast<double>(input));
    }

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const bool& input) {
        return to_le_bytes(static_cast<std::uint8_t>(input ? 1 : 0));
    }

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const char& input) {
        return to_le_bytes(static_cast<std::uint8_t>(input));
    }

    std::vector<std::byte> tag_invoke(binary_format_tag, serialize_tag, type_tag<std::vector<std::byte>>, const std::string& input) {
        return string_to_bytes(input);
    }

    // deserialize

    int tag_invoke(binary_format_tag, deserialize_tag, type_tag<int>, const std::vector<std::byte>& input) {
        return static_cast<int>(from_le_bytes<std::int32_t>(input, 0));
    }

    long tag_invoke(binary_format_tag, deserialize_tag, type_tag<long>, const std::vector<std::byte>& input) {
        return static_cast<long>(from_le_bytes<std::int64_t>(input, 0));
    }

    long long tag_invoke(binary_format_tag, deserialize_tag, type_tag<long long>, const std::vector<std::byte>& input) {
        return static_cast<long long>(from_le_bytes<std::int64_t>(input, 0));
    }

    unsigned int tag_invoke(binary_format_tag, deserialize_tag, type_tag<unsigned int>, const std::vector<std::byte>& input) {
        return static_cast<unsigned int>(from_le_bytes<std::uint32_t>(input, 0));
    }

    unsigned long tag_invoke(binary_format_tag, deserialize_tag, type_tag<unsigned long>, const std::vector<std::byte>& input) {
        return static_cast<unsigned long>(from_le_bytes<std::uint64_t>(input, 0));
    }

    unsigned long long tag_invoke(binary_format_tag, deserialize_tag, type_tag<unsigned long long>, const std::vector<std::byte>& input) {
        return static_cast<unsigned long long>(from_le_bytes<std::uint64_t>(input, 0));
    }

    float tag_invoke(binary_format_tag, deserialize_tag, type_tag<float>, const std::vector<std::byte>& input) {
        return float_from_le_bytes<float, std::uint32_t>(input, 0);
    }

    double tag_invoke(binary_format_tag, deserialize_tag, type_tag<double>, const std::vector<std::byte>& input) {
        return float_from_le_bytes<double, std::uint64_t>(input, 0);
    }

    long double tag_invoke(binary_format_tag, deserialize_tag, type_tag<long double>, const std::vector<std::byte>& input) {
        return static_cast<long double>(float_from_le_bytes<double, std::uint64_t>(input, 0));
    }

    bool tag_invoke(binary_format_tag, deserialize_tag, type_tag<bool>, const std::vector<std::byte>& input) {
        return from_le_bytes<std::uint8_t>(input, 0) != 0;
    }

    char tag_invoke(binary_format_tag, deserialize_tag, type_tag<char>, const std::vector<std::byte>& input) {
        return static_cast<char>(from_le_bytes<std::uint8_t>(input, 0));
    }

    std::string tag_invoke(binary_format_tag, deserialize_tag, type_tag<std::string>, const std::vector<std::byte>& input) {
        return string_from_bytes(input);
    }
}
