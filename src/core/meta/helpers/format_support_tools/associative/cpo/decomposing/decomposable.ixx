export module recurseria.core.meta.helpers.associative_ops:decomposable;

import std;
export import :default_decomposable;
import :pair_concept;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    struct decompose_associatively_tag {};

    template <typename FormatTag, typename T>
    concept UserDefinedAssociativelyDecomposable =
        requires(T&& value) {
            { tag_invoke(FormatTag{}, decompose_associatively_tag{}, std::forward<T>(value)) } -> std::ranges::view;
        } &&
        PairLike<std::ranges::range_value_t<
            decltype(tag_invoke(FormatTag{}, decompose_associatively_tag{}, std::declval<T>()))
        >>;

    template <typename FormatTag, typename T>
    concept AssociativelyDecomposable =
        UserDefinedAssociativelyDecomposable<FormatTag, T> ||
        DefaultAssociativelyDecomposable<FormatTag, T>;
}
