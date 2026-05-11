module;
#include <string>
#include <stdexcept>

export module recurseria.target.string.intermediate_representation:primitives_dsrlz;

import recurseria.core.meta.types_srlz_dsrlz;
import :format_tag;
import :tree_node;

export namespace recurseria::target::string {
    using deserialize_tag = recurseria::core::meta::deserialize_tag;

    template<typename T>
    using type_tag = recurseria::core::meta::type_tag<T>;

    // int
    int tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<int>,
        const StringIRTreeNode& input
    ) {
        return std::stoi(input.get_string());
    }

    // long
    long tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<long>,
        const StringIRTreeNode& input
    ) {
        return std::stol(input.get_string());
    }

    // long long
    long long tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<long long>,
        const StringIRTreeNode& input
    ) {
        return std::stoll(input.get_string());
    }

    // unsigned int
    unsigned int tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<unsigned int>,
        const StringIRTreeNode& input
    ) {
        return static_cast<unsigned int>(std::stoul(input.get_string()));
    }

    // unsigned long
    unsigned long tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<unsigned long>,
        const StringIRTreeNode& input
    ) {
        return std::stoul(input.get_string());
    }

    // unsigned long long
    unsigned long long tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<unsigned long long>,
        const StringIRTreeNode& input
    ) {
        return std::stoull(input.get_string());
    }

    // float
    float tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<float>,
        const StringIRTreeNode& input
    ) {
        return std::stof(input.get_string());
    }

    // double
    double tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<double>,
        const StringIRTreeNode& input
    ) {
        return std::stod(input.get_string());
    }

    // long double
    long double tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<long double>,
        const StringIRTreeNode& input
    ) {
        return std::stold(input.get_string());
    }

    // bool
    bool tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<bool>,
        const StringIRTreeNode& input
    ) {
        const auto& v = input.get_string();
        if (v == "true" || v == "1") {
            return true;
        } else if (v == "false" || v == "0") {
            return false;
        } else {
            throw std::invalid_argument("Invalid boolean value: " + v);
        }
    }

    // char
    char tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<char>,
        const StringIRTreeNode& input
    ) {
        std::string_view str = input.get_string();
        if (str.size() != 1) {
            throw std::invalid_argument(std::string("Invalid char value: ") + std::string(str));
        }
        return str[0];
    }

    // std::string
    std::string tag_invoke(
        string_intermediate_representation_format_tag,
        deserialize_tag,
        type_tag<std::string>,
        const StringIRTreeNode& input
    ) {
        return input.get_string();
    }
}
