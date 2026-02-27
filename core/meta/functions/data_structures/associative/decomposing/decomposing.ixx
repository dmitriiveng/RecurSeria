module;
#include <map>

export module serde.core.meta.functions.data_structures.associative.decomposing;

export import serde.core.meta.functions.tag_invokable;

export namespace serde::functions {
    struct decompose_associatively_tag {};

    template <typename T>
    concept AssociativelyDecomposable = requires(std::map<T, T>& out, const T& value) {
        tag_invoke(decompose_associatively_tag{}, out, value);
    };

    inline constexpr struct decompose_associatively_fn {
        template<typename T>
            requires AssociativelyDecomposable<T>
        constexpr void operator()(std::map<T, T>& out, const T& value) const
            noexcept(noexcept(tag_invoke(decompose_associatively_tag{}, out, value)))
        {
            tag_invoke(decompose_associatively_tag{}, out, value);
        }
    } decompose_associatively;
}

