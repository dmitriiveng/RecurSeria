module;
#include <vector>

export module serde.core.meta.functions.data_structures.sequence.grouping;

export import serde.core.meta.functions.tag_invokable;

export namespace serde::functions {
    struct group_sequentially_tag {};

    template <typename T>
    concept SequentiallyGroupable = requires(T& out, const std::vector<T>& vector) {
        tag_invoke(group_sequentially_tag{}, out, vector);
    };

    inline constexpr struct group_sequentially_fn {
        template<typename T>
            requires SequentiallyGroupable<T>
        constexpr void operator()(T& out, const std::vector<T>& value) const
            noexcept(noexcept(tag_invoke(group_sequentially_tag{}, out, value)))
        {
            tag_invoke(group_sequentially_tag{}, out, value);
        }
    } group_sequentially;
}

