export module recurseria.target.yaml:primitives_srlz;

import std;

import recurseria.core.meta.types_srlz_dsrlz;
import :format_tag;

export namespace recurseria::target::yaml {
    // Serialization
    
    using serialize_tag = recurseria::core::meta::serialize_tag;

    template<typename T>
    using type_tag = recurseria::core::meta::type_tag<T>;

    // int
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const int& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // long
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const long& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // long long
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const long long& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // unsigned int
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const unsigned int& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // unsigned long
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const unsigned long& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // unsigned long long
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const unsigned long long& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // float
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const float& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // double
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const double& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // long double
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const long double& input
    ) {
        return StringIRTreeNode{std::to_string(input)};
    }

    // bool
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const bool& input
    ) {
        return StringIRTreeNode{input ? "true" : "false"};
    }

    // char
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const char& input
    ) {
        return StringIRTreeNode{std::string(1, input)};
    }

    // std::string
    StringIRTreeNode tag_invoke(
        yaml_format_tag,
        serialize_tag,
        type_tag<StringIRTreeNode>,
        const std::string& input
    ) {
        return StringIRTreeNode{input};
    }

    // Deserialization
    
    using deserialize_tag = recurseria::core::meta::deserialize_tag;

    template<typename T>
    using type_tag = recurseria::core::meta::type_tag<T>;

    // int
    int tag_invoke(
        yaml_format_tag,
        deserialize_tag,
        type_tag<int>,
        const StringIRTreeNode& input
    ) {
        return std::stoi(input.get_string());
    }

    // long
    long tag_invoke(
        yaml_format_tag,
        deserialize_tag,
        type_tag<long>,
        const StringIRTreeNode& input
    ) {
        return std::stol(input.get_string());
    }

    // long long
    long long tag_invoke(
        yaml_format_tag,
        deserialize_tag,
        type_tag<long long>,
        const StringIRTreeNode& input
    ) {
        return std::stoll(input.get_string());
    }

    // unsigned int
    unsigned int tag_invoke(
        yaml_format_tag,
        deserialize_tag,
        type_tag<unsigned int>,
        const StringIRTreeNode& input
    ) {
        return static_cast<unsigned int>(std::stoul(input.get_string()));
    }

    // unsigned long
    unsigned long tag_invoke(
        yaml_format_tag,
        deserialize_tag,
        type_tag<unsigned long>,
        const StringIRTreeNode& input
    ) {
        return std::stoul(input.get_string());
    }

    // unsigned long long
    unsigned long long tag_invoke(
        yaml_format_tag,
        deserialize_tag,
        type_tag<unsigned long long>,
        const StringIRTreeNode& input
    ) {
        return std::stoull(input.get_string());
    }

    // float
    float tag_invoke(
        yaml_format_tag,
        deserialize_tag,
        type_tag<float>,
        const StringIRTreeNode& input
    ) {
        return std::stof(input.get_string());
    }

    // double
    double tag_invoke(
        yaml_format_tag,
        deserialize_tag,
        type_tag<double>,
        const StringIRTreeNode& input
    ) {
        return std::stod(input.get_string());
    }

    // long double
    long double tag_invoke(
        yaml_format_tag,
        deserialize_tag,
        type_tag<long double>,
        const StringIRTreeNode& input
    ) {
        return std::stold(input.get_string());
    }

    // bool
    bool tag_invoke(
        yaml_format_tag,
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
        yaml_format_tag,
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
        yaml_format_tag,
        deserialize_tag,
        type_tag<std::string>,
        const StringIRTreeNode& input
    ) {
        return input.get_string();
    }
}
