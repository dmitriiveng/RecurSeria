module;
#include <ranges>

export module recurseria.core.meta.helpers.sequence_ops:decomposing;

import :default_decomposable;

export namespace recurseria::core::meta {
    struct decompose_sequentially_tag {};

    template <typename FormatTag, typename T>
    concept SequentiallyDecomposable =
        requires(T&& value) {
            {
                tag_invoke(FormatTag{}, decompose_sequentially_tag{}, std::forward<T>(value))
            } -> std::ranges::view;
        };

    inline constexpr struct decompose_sequentially_fn {
        template<typename FormatTag, typename T>
            requires SequentiallyDecomposable<FormatTag, T> || DefaultSequentiallyDecomposable<FormatTag, T>
        constexpr auto operator()(FormatTag, const T& value) const {
            if constexpr (SequentiallyDecomposable<FormatTag, T>){
                return tag_invoke(FormatTag{}, decompose_sequentially_tag{}, value);
            }
            else if constexpr (DefaultSequentiallyDecomposable<FormatTag, T>){
                return tag_invoke(FormatTag{}, default_decompose_sequentially_tag{}, value);
            }
        }
    } decompose_sequentially;
}
