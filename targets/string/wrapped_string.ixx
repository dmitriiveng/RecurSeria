module;
#include <string>

export module serde.target.string.wrapped_string;

export namespace serde::targets::string {
    struct WrappedString {
        std::string value;

        WrappedString() = default;
        WrappedString(std::string v) : value(std::move(v)) {}

        bool operator<(const WrappedString& other) const {
            return value < other.value;
        }

        operator std::string() const { return value; }
    };
}