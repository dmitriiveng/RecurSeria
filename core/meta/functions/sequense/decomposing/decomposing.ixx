module;
#include <vector>

export module serde.core.meta.functions.data_structures.sequence.decomposing;

import serde.core.meta.functions.tag_invokable;

namespace serde::functions {
    // Default

    void default_decompose_sequentially_fn();

    template <typename T>
    concept DefaultSequentiallyDecomposable = requires(std::vector<T>& out, const T& value) {
        default_decompose_sequentially_fn(out, value);
    };

    // User defined

    export struct decompose_sequentially_tag {};

    export template <typename T>
    concept SequentiallyDecomposable = requires(std::vector<T>& out, const T& value) {
        tag_invoke(decompose_sequentially_tag{}, out, value);
    };

    export inline constexpr struct decompose_sequentially_fn {
        template<typename T>
            requires SequentiallyDecomposable<T> || DefaultSequentiallyDecomposable<T>
        constexpr void operator()(std::vector<T>& out, const T& value) const
            noexcept(
                noexcept(tag_invoke(decompose_sequentially_tag{}, out, value))
                || (!SequentiallyDecomposable<T> && noexcept(default_decompose_sequentially_fn(out, value)))
            )
        {
            if constexpr (SequentiallyDecomposable<T>){
                tag_invoke(decompose_sequentially_tag{}, out, value);
            }
            else{
                default_decompose_sequentially_fn(out, value);
            }
        }
    } decompose_sequentially;
}

