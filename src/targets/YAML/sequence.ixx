export module recurseria.target.yaml:sequence;

import std;
import yaml_cpp;

import recurseria.core.meta.helpers.sequence_ops;
import :format_tag;

export namespace recurseria::yaml {
    using group_sequentially_tag = recurseria::group_sequentially_tag;
    using decompose_sequentially_tag = recurseria::decompose_sequentially_tag;

    auto tag_invoke(
        yaml_format_tag,
        group_sequentially_tag,
        std::ranges::input_range auto&& input
    ) -> YAML::Node {
        YAML::Node node(YAML::NodeType::Sequence);
        for (auto&& child : input)
            node.push_back(std::forward<decltype(child)>(child));
        return node;
    }

    struct yaml_seq_view : std::ranges::view_interface<yaml_seq_view> {
        const YAML::Node* node;

        yaml_seq_view() = default;
        explicit yaml_seq_view(const YAML::Node& n) : node(&n) {}

        struct iterator {
            const YAML::Node* parent;
            mutable YAML::Node cache{};
            std::size_t idx{0};

            using iterator_category = std::random_access_iterator_tag;
            using value_type = YAML::Node;
            using difference_type = std::ptrdiff_t;
            using reference = const YAML::Node&;
            using pointer = const YAML::Node*;

            const YAML::Node& operator*() const {
                cache = (*parent)[static_cast<int>(idx)];
                return cache;
            }

            const YAML::Node& operator[](difference_type n) const {
                cache = (*parent)[static_cast<int>(idx + static_cast<std::size_t>(n))];
                return cache;
            }

            iterator& operator++() { ++idx; return *this; }
            iterator operator++(int) { auto tmp = *this; ++idx; return tmp; }
            iterator& operator--() { --idx; return *this; }
            iterator operator--(int) { auto tmp = *this; --idx; return tmp; }

            iterator& operator+=(difference_type n) { idx += static_cast<std::size_t>(n); return *this; }
            iterator& operator-=(difference_type n) { idx -= static_cast<std::size_t>(n); return *this; }

            iterator operator+(difference_type n) const { return {parent, {}, idx + static_cast<std::size_t>(n)}; }
            iterator operator-(difference_type n) const { return {parent, {}, idx - static_cast<std::size_t>(n)}; }

            difference_type operator-(const iterator& other) const {
                return static_cast<difference_type>(idx) - static_cast<difference_type>(other.idx);
            }

            bool operator==(const iterator& other) const { return idx == other.idx; }
            auto operator<=>(const iterator& other) const { return idx <=> other.idx; }

            friend iterator operator+(difference_type n, const iterator& it) {
                return {it.parent, {}, it.idx + static_cast<std::size_t>(n)};
            }
        };

        iterator begin() const { return {node, {}, 0}; }
        iterator end() const { return {node, {}, node->size()}; }

        YAML::Node operator[](std::ptrdiff_t n) const {
            return (*node)[static_cast<int>(n)];
        }
    };

    auto tag_invoke(
        yaml_format_tag,
        decompose_sequentially_tag,
        const YAML::Node& input
    ) -> yaml_seq_view {
        return yaml_seq_view{input};
    }
}

template<>
inline constexpr bool std::ranges::enable_view<recurseria::yaml::yaml_seq_view> = true;
