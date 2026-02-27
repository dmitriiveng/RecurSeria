module;
#include <map>

export module serde.core.meta.functions.data_structures.associative.grouping;

export import serde.core.meta.functions.tag_invokable;

export namespace serde::functions {
    struct group_associatively_tag {};

    template <typename T>
    concept AssociativelyGroupable = requires(T& out, const std::map<T, T>& map) {
        tag_invoke(group_associatively_tag{}, out, map);
    };

    inline constexpr struct group_associatively_fn {
        template<typename T>
            requires AssociativelyGroupable<T>
        constexpr void operator()(T& out, const std::map<T, T>& value) const
            noexcept(noexcept(tag_invoke(group_associatively_tag{}, out, value)))
        {
            tag_invoke(group_associatively_tag{}, out, value);
        }
    } group_associatively;
}

