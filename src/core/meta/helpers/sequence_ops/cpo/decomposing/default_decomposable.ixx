module;

#include <ranges>
#include <utility>

export module recurseria.core.meta.helpers.sequence_ops:default_decomposable;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    struct default_decompose_sequentially_tag{};

    template <typename FormatTag, typename T>
    concept DefaultSequentiallyDecomposable =
        requires(T&& value) {
            {
                tag_invoke(FormatTag{}, default_decompose_sequentially_tag{}, std::forward<T>(value))
            } -> std::ranges::view;
        };
}
