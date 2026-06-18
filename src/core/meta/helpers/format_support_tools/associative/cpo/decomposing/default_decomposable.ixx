export module recurseria.core.meta.helpers.associative_ops:default_decomposable;

import std;
import :pair_concept;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    struct default_decompose_associatively_tag{};

    template <typename FormatTag, typename T>
    concept DefaultAssociativelyDecomposable =
        requires(T&& value) {
            { tag_invoke(FormatTag{}, default_decompose_associatively_tag{}, std::forward<T>(value)) } -> std::ranges::view;
        } &&
        PairLike<std::ranges::range_value_t<
            decltype(tag_invoke(FormatTag{}, default_decompose_associatively_tag{}, std::declval<T>()))
        >>; // returning sth assoсiative
}
