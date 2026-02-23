export module serde.core.meta.functions.data_structures.sequence.decomposing;

import <vector>;
export import serde.core.meta.functions.tag_invokable;

export namespace serde::functions {
    struct decompose_sequentially_tag {};

    template <typename T>
    concept SequentiallyDecomposable = requires(std::vector<T>& out, const T& value) {
        tag_invoke(decompose_sequentially_tag{}, out, value);
    };

    inline constexpr struct decompose_sequentially_fn {
        template<typename T>
            requires SequentiallyDecomposable<T>
        constexpr void operator()(std::vector<T>& out, const T& value) const
            noexcept(noexcept(tag_invoke(decompose_sequentially_tag{}, out, value)))
        {
            tag_invoke(decompose_sequentially_tag{}, out, value);
        }
    } decompose_sequentially;
}

