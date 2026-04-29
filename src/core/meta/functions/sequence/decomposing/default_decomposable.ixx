module;
#include <vector>

export module serde.core.meta.functions.data_structures.sequence.default_decomposable;

export import serde.core.meta.functions.tag_invokable;

export namespace serde::functions {

    struct default_decompose_sequentially_tag{};

    template <typename T>
    concept DefaultSequentiallyDecomposable = requires(std::vector<T>& out, const T& value) {
        tag_invoke(default_decompose_sequentially_tag{}, out, value);
    };
}
