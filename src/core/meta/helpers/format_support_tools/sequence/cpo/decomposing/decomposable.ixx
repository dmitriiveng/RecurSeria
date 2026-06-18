export module recurseria.core.meta.helpers.sequence_ops:decomposable;

import std;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    struct decompose_sequentially_tag {};

    template <typename FormatTag, typename T>
    concept SequentiallyDecomposable =
        requires(T&& value) {
            {
                tag_invoke(FormatTag{}, decompose_sequentially_tag{}, std::forward<T>(value))
            } -> std::ranges::view;
        };
}
