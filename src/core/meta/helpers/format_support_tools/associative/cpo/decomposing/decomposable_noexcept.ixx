export module recurseria.core.meta.helpers.associative_ops:decomposable_noexcept;

import std;

export import :decomposable;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename T>
    concept AssociativelyDecomposableNoexcept =
        (AssociativelyDecomposable<FormatTag, T> &&
         noexcept(tag_invoke(FormatTag{}, decompose_associatively_tag{}, std::declval<const T&>()))) ||
        (DefaultAssociativelyDecomposable<FormatTag, T> &&
         noexcept(tag_invoke(FormatTag{}, default_decompose_associatively_tag{}, std::declval<const T&>())));
}
