export module recurseria.core.meta.validators:group_decompose;

import std;

import recurseria.core.meta.helpers.sequence_ops;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename T>
        requires std::equality_comparable<T>
    bool decompose_group_round_trip_validation(const T& value) {
        auto view = decompose_sequentially(FormatTag{}, value);
        auto result = group_sequentially(FormatTag{}, view);
        return value == result;
    }
}
