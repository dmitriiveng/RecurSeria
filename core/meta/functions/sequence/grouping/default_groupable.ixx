module;
#include <vector>

export module serde.core.meta.functions.data_structures.sequence.default_groupable;

export import serde.core.meta.functions.tag_invokable;

export namespace serde::functions {

    struct default_group_sequentially_tag{};

    template <typename T>
    concept DefaultSequentiallyGroupable = requires(T& out, const std::vector<T>& value) {
        tag_invoke(default_group_sequentially_tag{}, out, value);
    };
}