module;
#include <vector>

export module serde.core.meta.functions.data_structures.sequence.grouping;

import serde.core.meta.functions.tag_invokable;

namespace serde::functions {
    // Default

    void default_group_sequentially_fn();

    template <typename T>
    concept DefaultSequentiallyGroupable = requires(T& out, const std::vector<T>& value) {
        default_group_sequentially_fn(out, value);
    };

    // User defined

    export struct group_sequentially_tag {};

    export template <typename T>
    concept SequentiallyGroupable = requires(T& out, const std::vector<T>& vector) {
        tag_invoke(group_sequentially_tag{}, out, vector);
    };

    export inline constexpr struct group_sequentially_fn {
        template<typename T>
            requires SequentiallyGroupable<T> || DefaultSequentiallyGroupable<T>
        constexpr void operator()(T& out, const std::vector<T>& value) const
            noexcept(
                noexcept(tag_invoke(group_sequentially_tag{}, out, value))
                || (!SequentiallyGroupable<T> && noexcept(default_group_sequentially_fn(out, value)))
            )
        {
            if constexpr (SequentiallyGroupable<T>){
                tag_invoke(group_sequentially_tag{}, out, value);
            }
            else{
                default_group_sequentially_fn(out, value);
            }
        }
    } group_sequentially;
}

