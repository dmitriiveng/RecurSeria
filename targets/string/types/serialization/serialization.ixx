export module serde.targets.string.types.serialization;

import serde.target.string.wrapped_string;
import serde.core.meta.functions.types.serialization;

export namespace serde::targets::string {
    using deserialize_tag = serde::functions::serialize_tag;

    // int
    void tag_invoke(deserialize_tag, WrappedString& out, const int& input) {
        out = WrappedString(std::to_string(input));
    }

    // long
    void tag_invoke(deserialize_tag, WrappedString& out, const long& input) {
        out = WrappedString(std::to_string(input));
    }

    // long long
    void tag_invoke(deserialize_tag, WrappedString& out, const long long& input) {
        out = WrappedString(std::to_string(input));
    }

    // unsigned int
    void tag_invoke(deserialize_tag, WrappedString& out, const unsigned int& input) {
        out = WrappedString(std::to_string(input));
    }

    // unsigned long
    void tag_invoke(deserialize_tag, WrappedString& out, const unsigned long& input) {
        out = WrappedString(std::to_string(input));
    }

    // unsigned long long
    void tag_invoke(deserialize_tag, WrappedString& out, const unsigned long long& input) {
        out = WrappedString(std::to_string(input));
    }

    // float
    void tag_invoke(deserialize_tag, WrappedString& out, const float& input) {
        out = WrappedString(std::to_string(input));
    }

    // double
    void tag_invoke(deserialize_tag, WrappedString& out, const double& input) {
        out = WrappedString(std::to_string(input));
    }

    // long double
    void tag_invoke(deserialize_tag, WrappedString& out, const long double& input) {
        out = WrappedString(std::to_string(input));
    }

    // bool
    void tag_invoke(deserialize_tag, WrappedString& out, const bool& input) {
        out = WrappedString(input ? "true" : "false");
    }

    // char
    void tag_invoke(deserialize_tag, WrappedString& out, const char& input) {
        out = WrappedString(std::string(1, input));
    }

    // std::string
    void tag_invoke(deserialize_tag, WrappedString& out, const std::string& input) {
        out = WrappedString('"' + input + '"');
    }
}
