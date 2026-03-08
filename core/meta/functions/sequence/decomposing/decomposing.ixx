module;
#include <vector>

export module serde.core.meta.functions.data_structures.sequence.decomposing;

import serde.core.meta.functions.data_structures.sequence.default_decomposable;

export namespace serde::functions {

    struct decompose_sequentially_tag {};

    template <typename T>
    concept SequentiallyDecomposable = requires(std::vector<T>& out, const T& value) {
        tag_invoke(decompose_sequentially_tag{}, out, value);
    };

    inline constexpr struct decompose_sequentially_fn {
        template<typename T>
            /*requires SequentiallyDecomposable<T> || DefaultSequentiallyDecomposable<T>*/
        constexpr void operator()(std::vector<T>& out, const T& value) const {
            if constexpr (SequentiallyDecomposable<T>){
                tag_invoke(decompose_sequentially_tag{}, out, value);
            }
            else if constexpr (DefaultSequentiallyDecomposable<T>){
                tag_invoke(default_decompose_sequentially_tag{}, out, value);
            }
        }
    } decompose_sequentially;
}

