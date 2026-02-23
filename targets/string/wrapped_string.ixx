export module serde.target.string.wrapped_string;

export import <string>;

export namespace serde::targets::string {
    struct WrappedString {
        std::string value;

        WrappedString() = default;
        WrappedString(std::string v) : value(std::move(v)) {}
    };
}