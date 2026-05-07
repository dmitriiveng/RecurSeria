module;
#include <string>

export module recurseria.targets.string:primitives_srlz;

import recurseria.core.meta.types_srlz_dsrlz;
import :format_tag;

export namespace recurseria::target::string {
    using serialize_tag = recurseria::core::meta::serialize_tag;

    template<typename T>
    using type_tag = recurseria::core::meta::type_tag<T>;

    // int
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const int& input) {
        return std::to_string(input);
    }

    // long
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const long& input) {
        return std::to_string(input);
    }

    // long long
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const long long& input) {
        return std::to_string(input);
    }

    // unsigned int
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const unsigned int& input) {
        return std::to_string(input);
    }

    // unsigned long
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const unsigned long& input) {
        return std::to_string(input);
    }

    // unsigned long long
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const unsigned long long& input) {
        return std::to_string(input);
    }

    // float
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const float& input) {
        return std::to_string(input);
    }

    // double
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const double& input) {
        return std::to_string(input);
    }

    // long double
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const long double& input) {
        return std::to_string(input);
    }

    // bool
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const bool& input) {
        return input ? "true" : "false";
    }

    // char
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const char& input) {
        return std::string(1, input);
    }

    // std::string
    std::string tag_invoke(default_string_format_tag, serialize_tag, type_tag<std::string>, const std::string& input) {
        return '"' + input + '"';
    }
}
