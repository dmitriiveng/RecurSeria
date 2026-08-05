export module recurseria.core.meta.helpers.sequence_ops:decomposable;

import std;

export import recurseria.core.meta.tag_invokable;
export import :default_decomposable;

export namespace recurseria {
    struct decompose_sequentially_tag {};

    template <typename FormatTag, typename T>
    concept UserDefinedSequentiallyDecomposable =
        requires(T&& value) {
            {
                tag_invoke(FormatTag{}, decompose_sequentially_tag{}, std::forward<T>(value))
            } -> std::ranges::view;
        };

    template <typename FormatTag, typename T>
    concept SequentiallyDecomposable =
        UserDefinedSequentiallyDecomposable<FormatTag, T> ||
        DefaultSequentiallyDecomposable<FormatTag, T>;
}
