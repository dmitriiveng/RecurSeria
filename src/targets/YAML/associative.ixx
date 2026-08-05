export module recurseria.yaml:associative;

import std;
import yaml_cpp;

import recurseria.helpers.associative_ops;
import :format_tag;

export namespace recurseria::yaml {
    using group_associatively_tag = recurseria::group_associatively_tag;
    using decompose_associatively_tag = recurseria::decompose_associatively_tag;

    auto tag_invoke(
        yaml_format_tag,
        group_associatively_tag,
        std::ranges::input_range auto&& input
    ) -> YAML::Node {
        YAML::Node node(YAML::NodeType::Map);
        for (auto&& [key, value] : input)
            node[key] = value;
        return node;
    }

    struct yaml_assoc_view : std::ranges::view_interface<yaml_assoc_view> {
        const YAML::Node* node;

        yaml_assoc_view() = default;
        explicit yaml_assoc_view(const YAML::Node& n) : node(&n) {}

        struct iterator {
            YAML::const_iterator it;

            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<YAML::Node, YAML::Node>;
            using difference_type = std::ptrdiff_t;
            using reference = value_type;
            using pointer = const value_type*;

            value_type operator*() const {
                return {it->first, it->second};
            }

            iterator& operator++() { ++it; return *this; }
            iterator operator++(int) { auto tmp = *this; ++it; return tmp; }

            bool operator==(const iterator& other) const { return it == other.it; }
        };

        iterator begin() const { return {node->begin()}; }
        iterator end() const { return {node->end()}; }
    };

    auto tag_invoke(
        yaml_format_tag,
        decompose_associatively_tag,
        const YAML::Node& input
    ) -> yaml_assoc_view {
        return yaml_assoc_view{input};
    }
}

template<>
inline constexpr bool std::ranges::enable_view<recurseria::yaml::yaml_assoc_view> = true;
