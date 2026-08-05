export module recurseria.helpers.associative_ops:decomposable_noexcept;

import std;

export import :decomposable;
export import :default_decomposable;

export namespace recurseria {
    template <typename FormatTag, typename T>
    concept AssociativelyDecomposableNoexcept =
        (UserDefinedAssociativelyDecomposable<FormatTag, T> &&
         noexcept(tag_invoke(FormatTag{}, decompose_associatively_tag{}, std::declval<const T&>()))) ||
        (DefaultAssociativelyDecomposable<FormatTag, T> &&
         noexcept(tag_invoke(FormatTag{}, default_decompose_associatively_tag{}, std::declval<const T&>())));
}
