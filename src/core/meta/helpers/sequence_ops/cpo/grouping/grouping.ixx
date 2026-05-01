module;
#include <vector>

export module recurseria.core.meta.helpers.sequence_ops:grouping;

import :default_groupable;

export namespace recurseria::core::meta {
    struct group_sequentially_tag {};

    template <typename T>
    concept SequentiallyGroupable = requires(T& out, const std::vector<T>& vector) {
        tag_invoke(group_sequentially_tag{}, out, vector);
    };

    inline constexpr struct group_sequentially_fn {
        template<typename T>
            requires SequentiallyGroupable<T> || DefaultSequentiallyGroupable<T>
        constexpr void operator()(T& out, const std::vector<T>& value) const {
            if constexpr (SequentiallyGroupable<T>){
                tag_invoke(group_sequentially_tag{}, out, value);
            }
            else if constexpr (DefaultSequentiallyGroupable<T>){
                tag_invoke(default_group_sequentially_tag{}, out, value);
            }
        }
    } group_sequentially;
}
