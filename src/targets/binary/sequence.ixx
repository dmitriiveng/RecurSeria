export module recurseria.binary:sequence;

import std;

import recurseria.helpers.sequence_ops;
import :format_tag;

namespace recurseria::binary {
    void append_frame_u64(std::vector<std::byte>& output, std::uint64_t value) {
        for (std::size_t i = 0; i < sizeof(std::uint64_t); ++i) {
            output.push_back(static_cast<std::byte>(static_cast<std::uint8_t>(value >> (8 * i))));
        }
    }

    std::uint64_t read_frame_u64(const std::vector<std::byte>& input, std::size_t offset) {
        if (offset + sizeof(std::uint64_t) > input.size()) {
            throw std::out_of_range("binary: truncated input");
        }
        std::uint64_t value = 0;
        for (std::size_t i = 0; i < sizeof(std::uint64_t); ++i) {
            value = value | static_cast<std::uint64_t>(input[offset + i]) << (8 * i);
        }
        return value;
    }
}

export namespace recurseria::binary {
    using group_sequentially_tag = recurseria::group_sequentially_tag;
    using decompose_sequentially_tag = recurseria::decompose_sequentially_tag;

    auto tag_invoke(
        binary_format_tag,
        group_sequentially_tag,
        std::ranges::input_range auto&& input
    ) -> std::vector<std::byte>
        requires std::convertible_to<
            std::ranges::range_value_t<decltype(input)>,
            std::vector<std::byte>
        >
    {
        std::vector<std::byte> result;
        for (auto&& element : input) {
            std::vector<std::byte> chunk(element.begin(), element.end());
            append_frame_u64(result, static_cast<std::uint64_t>(chunk.size()));
            result.insert(result.end(), chunk.begin(), chunk.end());
        }
        return result;
    }

    struct binary_seq_view : std::ranges::view_interface<binary_seq_view> {
        const std::vector<std::byte>* data{nullptr};
        std::vector<std::size_t> offsets;
        std::vector<std::size_t> lengths;

        binary_seq_view() = default;
        explicit binary_seq_view(const std::vector<std::byte>& input) : data(&input) {
            constexpr std::size_t header_size = sizeof(std::uint64_t);
            std::size_t pos = 0;
            while (pos < input.size()) {
                std::uint64_t len = read_frame_u64(input, pos);
                pos += header_size;
                if (len > input.size() - pos) {
                    throw std::out_of_range("binary: malformed sequence framing");
                }
                offsets.push_back(pos);
                lengths.push_back(static_cast<std::size_t>(len));
                pos += static_cast<std::size_t>(len);
            }
            if (pos != input.size()) {
                throw std::out_of_range("binary: malformed sequence framing");
            }
        }

        struct iterator {
            const std::vector<std::byte>* data{nullptr};
            const std::vector<std::size_t>* offsets{nullptr};
            const std::vector<std::size_t>* lengths{nullptr};
            std::size_t idx{0};

            using iterator_category = std::random_access_iterator_tag;
            using value_type = std::vector<std::byte>;
            using difference_type = std::ptrdiff_t;
            using reference = std::vector<std::byte>;
            using pointer = const std::vector<std::byte>*;

            std::vector<std::byte> slice() const {
                std::size_t begin = (*offsets)[idx];
                std::size_t size = (*lengths)[idx];
                return std::vector<std::byte>(
                    data->begin() + static_cast<difference_type>(begin),
                    data->begin() + static_cast<difference_type>(begin + size)
                );
            }

            reference operator*() const { return slice(); }
            reference operator[](difference_type n) const { return *(iterator{*this} += n); }

            iterator& operator++() { ++idx; return *this; }
            iterator operator++(int) { auto tmp = *this; ++idx; return tmp; }
            iterator& operator--() { --idx; return *this; }
            iterator operator--(int) { auto tmp = *this; --idx; return tmp; }

            iterator& operator+=(difference_type n) { idx += static_cast<std::size_t>(n); return *this; }
            iterator& operator-=(difference_type n) { idx -= static_cast<std::size_t>(n); return *this; }

            iterator operator+(difference_type n) const { return iterator{*this} += n; }
            iterator operator-(difference_type n) const { return iterator{*this} -= n; }

            friend iterator operator+(difference_type n, const iterator& it) { return it + n; }

            difference_type operator-(const iterator& other) const {
                return static_cast<difference_type>(idx) - static_cast<difference_type>(other.idx);
            }

            bool operator==(const iterator& other) const { return idx == other.idx; }
            auto operator<=>(const iterator& other) const { return idx <=> other.idx; }
        };

        iterator begin() const { return {data, &offsets, &lengths, 0}; }
        iterator end() const { return {data, &offsets, &lengths, size()}; }

        std::size_t size() const { return offsets.size(); }

        std::vector<std::byte> operator[](std::ptrdiff_t n) const {
            return *(begin() + n);
        }
    };

    auto tag_invoke(
        binary_format_tag,
        decompose_sequentially_tag,
        const std::vector<std::byte>& input
    ) -> binary_seq_view {
        return binary_seq_view{input};
    }
}

template<>
inline constexpr bool std::ranges::enable_view<recurseria::binary::binary_seq_view> = true;
