export module recurseria.core.meta.helpers.sequence_ops:decomposable_noexcept;

import std;

export import :decomposable;
export import :default_decomposable;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename T>
    concept SequentiallyDecomposableNoexcept =
        (SequentiallyDecomposable<FormatTag, T> &&
         noexcept(tag_invoke(FormatTag{}, decompose_sequentially_tag{}, std::declval<const T&>()))) ||
        (DefaultSequentiallyDecomposable<FormatTag, T> &&
         noexcept(tag_invoke(FormatTag{}, default_decompose_sequentially_tag{}, std::declval<const T&>())));
}
