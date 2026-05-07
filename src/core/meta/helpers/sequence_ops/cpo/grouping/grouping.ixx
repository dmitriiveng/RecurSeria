module;
#include <vector>

export module recurseria.core.meta.helpers.sequence_ops:grouping;

import :default_groupable;

export namespace recurseria::core::meta {
    struct group_sequentially_tag {};

    template <typename FormatTag, typename T>
    concept SequentiallyGroupable = requires(T& out, const std::vector<T>& vector) {
        tag_invoke(FormatTag{}, group_sequentially_tag{}, out, vector);
    };

    inline constexpr struct group_sequentially_fn {
        template<typename FormatTag, typename T>
            requires SequentiallyGroupable<FormatTag, T> || DefaultSequentiallyGroupable<FormatTag, T>
        constexpr void operator()(FormatTag, T& out, const std::vector<T>& value) const {
            if constexpr (SequentiallyGroupable<FormatTag, T>){
                tag_invoke(FormatTag{}, group_sequentially_tag{}, out, value);
            }
            else if constexpr (DefaultSequentiallyGroupable<FormatTag, T>){
                tag_invoke(FormatTag{}, default_group_sequentially_tag{}, out, value);
            }
        }
    } group_sequentially;
}
