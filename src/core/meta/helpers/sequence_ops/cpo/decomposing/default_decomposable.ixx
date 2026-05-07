module;
#include <vector>

export module recurseria.core.meta.helpers.sequence_ops:default_decomposable;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    struct default_decompose_sequentially_tag{};

    template <typename FormatTag, typename T>
    concept DefaultSequentiallyDecomposable = requires(std::vector<T>& out, const T& value) {
        tag_invoke(FormatTag{}, default_decompose_sequentially_tag{}, out, value);
    };
}
