module;
#include <string>
#include <optional>
#include <stdexcept>

export module serde.targets.string.types.deserialization;

import serde.core.meta.functions.types.deserialization;
import serde.targets.string.string_utils.from_structure.copy_quoted;

export namespace serde::functions {
    using deserialize_tag = serde::functions::deserialize_tag;

    // int
    void tag_invoke(deserialize_tag, int& out, const std::string& input) {
        out = std::stoi(input);
    }

    // long
    void tag_invoke(deserialize_tag, long& out, const std::string& input) {
        out = std::stol(input);
    }

    // long long
    void tag_invoke(deserialize_tag, long long& out, const std::string& input) {
        out = std::stoll(input);
    }

    // unsigned int
    void tag_invoke(deserialize_tag, unsigned int& out, const std::string& input) {
        out = static_cast<unsigned int>(std::stoul(input));
    }

    // unsigned long
    void tag_invoke(deserialize_tag, unsigned long& out, const std::string& input) {
        out = std::stoul(input);
    }

    // unsigned long long
    void tag_invoke(deserialize_tag, unsigned long long& out, const std::string& input) {
        out = std::stoull(input);
    }

    // float
    void tag_invoke(deserialize_tag, float& out, const std::string& input) {
        out = std::stof(input);
    }

    // double
    void tag_invoke(deserialize_tag, double& out, const std::string& input) {
        out = std::stod(input);
    }

    // long double
    void tag_invoke(deserialize_tag, long double& out, const std::string& input) {
        out = std::stold(input);
    }

    // bool
    void tag_invoke(deserialize_tag, bool& out, const std::string& input) {
        const auto& v = input;
        if (v == "true" || v == "1") {
            out = true;
        } else if (v == "false" || v == "0") {
            out = false;
        } else {
            throw std::invalid_argument("Invalid boolean value: " + v);
        }
    }

    // char
    void tag_invoke(deserialize_tag, char& out, const std::string& input) {
        if (input.size() != 1) {
            throw std::invalid_argument("Invalid char value: " + input);
        }
        out = input[0];
    }

    // std::string
    void tag_invoke(deserialize_tag, std::string& out, const std::string& input) {
        auto b = input.begin();
        const auto e = input.end();

        const std::optional<std::string> result = str_utils::copy_quoted(b, e, '"');
        if (!result.has_value()) {
            throw std::runtime_error("Could not deserialize WrappedString: " + input + " to string");
        }
        std::string res_str = result.value();
        res_str.pop_back();
        out = std::string(++res_str.begin(), res_str.end());
    }
}