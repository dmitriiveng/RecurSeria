module;

#include <string>

export module recurseria.target.string.intermediate_representation:primitives_srlz;

import recurseria.core.meta.types_srlz_dsrlz;
import :format_tag;
import :tree_node;

export namespace recurseria::target::string {
    using serialize_tag = recurseria::core::meta::serialize_tag;

    template<typename T>
    using type_tag = recurseria::core::meta::type_tag<T>;

    // int
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const int& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // long
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const long& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // long long
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const long long& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // unsigned int
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const unsigned int& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // unsigned long
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const unsigned long& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // unsigned long long
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const unsigned long long& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // float
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const float& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // double
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const double& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // long double
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const long double& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // bool
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const bool& input
    ) {
        return StringIRTreeNode{input ? "true" : "false"};
    }

    // char
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const char& input
    ) {
        return StringIRTreeNode{std::string(1, input)};
    }

    // std::string
    StringIRTreeNode tag_invoke(
        string_intermediate_representation_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const std::string& input
    ) {
        return StringIRTreeNode{input};
    }
}
