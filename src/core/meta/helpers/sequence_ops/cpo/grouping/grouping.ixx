module;

#include <ranges>
#include <type_traits>

export module recurseria.core.meta.helpers.sequence_ops:grouping;

import :default_groupable;

export namespace recurseria::core::meta {
    struct group_sequentially_tag {};

    template <typename FormatTag, typename Range>
    concept SequentiallyGroupable =
        std::ranges::input_range<Range> &&
        requires(Range&& range) {
            {
                tag_invoke(FormatTag{}, group_sequentially_tag{}, std::forward<Range>(range))
            } -> std::same_as<std::remove_cvref_t<std::ranges::range_value_t<Range>>>;
        };

    inline constexpr struct group_sequentially_fn {
        template <typename FormatTag, std::ranges::input_range Range>
            requires SequentiallyGroupable<FormatTag, Range> || DefaultSequentiallyGroupable<FormatTag, Range>
        constexpr auto operator()(FormatTag, Range&& range) const {
            if constexpr (SequentiallyGroupable<FormatTag, Range>){
                return tag_invoke(FormatTag{}, group_sequentially_tag{}, std::forward<Range>(range));
            }
            else if constexpr (DefaultSequentiallyGroupable<FormatTag, Range>){
                return tag_invoke(FormatTag{}, default_group_sequentially_tag{}, std::forward<Range>(range));
            }
        }
    } group_sequentially;
}
