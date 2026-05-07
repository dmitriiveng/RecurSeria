module;
#include <string>
#include <optional>
#include <stdexcept>

export module recurseria.targets.string:primitives_dsrlz;

import recurseria.core.meta.types_srlz_dsrlz;
import recurseria.targets.string.utils;
import :format_tag;

export namespace recurseria::target::string {
    using deserialize_tag = recurseria::core::meta::deserialize_tag;

    template<typename T>
    using type_tag = recurseria::core::meta::type_tag<T>;

    // int
    int tag_invoke(default_string_format_tag, deserialize_tag, type_tag<int>, const std::string& input) {
        return std::stoi(input);
    }

    // long
    long tag_invoke(default_string_format_tag, deserialize_tag, type_tag<long>, const std::string& input) {
        return std::stol(input);
    }

    // long long
    long long tag_invoke(default_string_format_tag, deserialize_tag, type_tag<long long>, const std::string& input) {
        return std::stoll(input);
    }

    // unsigned int
    unsigned int tag_invoke(default_string_format_tag, deserialize_tag, type_tag<unsigned int>, const std::string& input) {
        return static_cast<unsigned int>(std::stoul(input));
    }

    // unsigned long
    unsigned long tag_invoke(default_string_format_tag, deserialize_tag, type_tag<unsigned long>, const std::string& input) {
        return std::stoul(input);
    }

    // unsigned long long
    unsigned long long tag_invoke(default_string_format_tag, deserialize_tag, type_tag<unsigned long long>, const std::string& input) {
        return std::stoull(input);
    }

    // float
    float tag_invoke(default_string_format_tag, deserialize_tag, type_tag<float>, const std::string& input) {
        return std::stof(input);
    }

    // double
    double tag_invoke(default_string_format_tag, deserialize_tag, type_tag<double>, const std::string& input) {
        return std::stod(input);
    }

    // long double
    long double tag_invoke(default_string_format_tag, deserialize_tag, type_tag<long double>, const std::string& input) {
        return std::stold(input);
    }

    // bool
    bool tag_invoke(default_string_format_tag, deserialize_tag, type_tag<bool>, const std::string& input) {
        const auto& v = input;
        if (v == "true" || v == "1") {
            return true;
        } else if (v == "false" || v == "0") {
            return false;
        } else {
            throw std::invalid_argument("Invalid boolean value: " + v);
        }
    }

    // char
    char tag_invoke(default_string_format_tag, deserialize_tag, type_tag<char>, const std::string& input) {
        if (input.size() != 1) {
            throw std::invalid_argument("Invalid char value: " + input);
        }
        return input[0];
    }

    // std::string
    std::string tag_invoke(default_string_format_tag, deserialize_tag, type_tag<std::string>, const std::string& input) {
        auto b = input.begin();
        const auto e = input.end();

        const std::optional<std::string> result = recurseria::targets::string::copy_quoted(b, e, '"');
        if (!result.has_value()) {
            throw std::runtime_error("Could not deserialize WrappedString: " + input + " to string");
        }
        std::string res_str = result.value();
        res_str.pop_back();
        return std::string(++res_str.begin(), res_str.end());
    }
}
