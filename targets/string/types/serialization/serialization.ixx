module;
#include <string>

export module serde.targets.string.types.serialization;

import serde.core.meta.functions.types.serialization;

export namespace serde::functions {
    using deserialize_tag = serde::functions::serialize_tag;

    // int
    void tag_invoke(deserialize_tag, std::string& out, const int& input) {
        out = std::to_string(input);
    }

    // long
    void tag_invoke(deserialize_tag, std::string& out, const long& input) {
        out = std::to_string(input);
    }

    // long long
    void tag_invoke(deserialize_tag, std::string& out, const long long& input) {
        out = std::to_string(input);
    }

    // unsigned int
    void tag_invoke(deserialize_tag, std::string& out, const unsigned int& input) {
        out = std::to_string(input);
    }

    // unsigned long
    void tag_invoke(deserialize_tag, std::string& out, const unsigned long& input) {
        out = std::to_string(input);
    }

    // unsigned long long
    void tag_invoke(deserialize_tag, std::string& out, const unsigned long long& input) {
        out = std::to_string(input);
    }

    // float
    void tag_invoke(deserialize_tag, std::string& out, const float& input) {
        out = std::to_string(input);
    }

    // double
    void tag_invoke(deserialize_tag, std::string& out, const double& input) {
        out = std::to_string(input);
    }

    // long double
    void tag_invoke(deserialize_tag, std::string& out, const long double& input) {
        out = std::to_string(input);
    }

    // bool
    void tag_invoke(deserialize_tag, std::string& out, const bool& input) {
        out = input ? "true" : "false";
    }

    // char
    void tag_invoke(deserialize_tag, std::string& out, const char& input) {
        out = std::string(1, input);
    }

    // std::string
    void tag_invoke(deserialize_tag, std::string& out, const std::string& input) {
        out = '"' + input + '"';
    }
}
