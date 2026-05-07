module;
#include <vector>

export module recurseria.core.meta.helpers.sequence_ops:decomposing;

import :default_decomposable;

export namespace recurseria::core::meta {
    struct decompose_sequentially_tag {};

    template <typename FormatTag, typename T>
    concept SequentiallyDecomposable = requires(std::vector<T>& out, const T& value) {
        tag_invoke(FormatTag{}, decompose_sequentially_tag{}, out, value);
    };

    inline constexpr struct decompose_sequentially_fn {
        template<typename FormatTag, typename T>
            requires SequentiallyDecomposable<FormatTag, T> || DefaultSequentiallyDecomposable<FormatTag, T>
        constexpr void operator()(FormatTag, std::vector<T>& out, const T& value) const {
            if constexpr (SequentiallyDecomposable<FormatTag, T>){
                tag_invoke(FormatTag{}, decompose_sequentially_tag{}, out, value);
            }
            else if constexpr (DefaultSequentiallyDecomposable<FormatTag, T>){
                tag_invoke(FormatTag{}, default_decompose_sequentially_tag{}, out, value);
            }
        }
    } decompose_sequentially;
}
