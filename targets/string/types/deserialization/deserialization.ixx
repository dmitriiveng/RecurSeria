export module serde.targets.string.types.deserialization;

import serde.target.string.wrapped_string;
import serde.core.meta.functions.types.deserialization;
import serde.targets.string.string_utils.from_structure.copy_quoted;

export namespace serde::targets::string{
    using deserialize_tag = serde::functions::deserialize_tag;

    // int
    void tag_invoke(deserialize_tag, int& out, const WrappedString& input) {
        out = std::stoi(input.value);
    }

    // long
    void tag_invoke(deserialize_tag, long& out, const WrappedString& input) {
        out = std::stol(input.value);
    }

    // long long
    void tag_invoke(deserialize_tag, long long& out, const WrappedString& input) {
        out = std::stoll(input.value);
    }

    // unsigned int
    void tag_invoke(deserialize_tag, unsigned int& out, const WrappedString& input) {
        out = static_cast<unsigned int>(std::stoul(input.value));
    }

    // unsigned long
    void tag_invoke(deserialize_tag, unsigned long& out, const WrappedString& input) {
        out = std::stoul(input.value);
    }

    // unsigned long long
    void tag_invoke(deserialize_tag, unsigned long long& out, const WrappedString& input) {
        out = std::stoull(input.value);
    }

    // float
    void tag_invoke(deserialize_tag, float& out, const WrappedString& input) {
        out = std::stof(input.value);
    }

    // double
    void tag_invoke(deserialize_tag, double& out, const WrappedString& input) {
        out = std::stod(input.value);
    }

    // long double
    void tag_invoke(deserialize_tag, long double& out, const WrappedString& input) {
        out = std::stold(input.value);
    }

    // bool
    void tag_invoke(deserialize_tag, bool& out, const WrappedString& input) {
        const auto& v = input.value;
        if (v == "true" || v == "1") {
            out = true;
        } else if (v == "false" || v == "0") {
            out = false;
        } else {
            throw std::invalid_argument("Invalid boolean value: " + v);
        }
    }

    // char
    void tag_invoke(deserialize_tag, char& out, const WrappedString& input) {
        if (input.value.size() != 1) {
            throw std::invalid_argument("Invalid char value: " + input.value);
        }
        out = input.value[0];
    }

    // std::string
    void tag_invoke(deserialize_tag, std::string& out, const WrappedString& input) {
        auto b = input.value.begin();
        const auto e = input.value.end();

        const std::optional<std::string> result = str_utils::copy_quoted(b, e, '"');
        if (!result.has_value()) {
            throw std::runtime_error("Could not deserialize WrappedString: " + input.value + " to string");
        }
        std::string res_str = result.value();
        res_str.pop_back();
        out = std::string(++res_str.begin(), res_str.end());
    }
}