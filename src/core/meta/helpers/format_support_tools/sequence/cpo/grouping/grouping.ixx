export module recurseria.core.meta.helpers.sequence_ops:grouping;

import std;
export import :default_groupable;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.exceptions;

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

    /// Groups elements from a range back into a single value.
    ///
    /// @tparam FormatTag serialization format tag
    /// @tparam Range     input range type
    /// @param  range     range of elements to group
    /// @return the reconstructed value
    ///
    /// @throws tag_invoke_error if the underlying `tag_invoke` raises an exception.
    inline constexpr struct group_sequentially_fn {
        template <typename FormatTag, std::ranges::input_range Range>
            requires SequentiallyGroupable<FormatTag, Range> || DefaultSequentiallyGroupable<FormatTag, Range>
        constexpr auto operator()(FormatTag, Range&& range) const {
            if constexpr (SequentiallyGroupable<FormatTag, Range>){
                try {
                    return tag_invoke(FormatTag{}, group_sequentially_tag{}, std::forward<Range>(range));
                } catch (const std::exception& e) {
                    throw tag_invoke_error("group_sequentially", typeid(std::ranges::range_value_t<Range>).name(), e.what());
                }
            }
            else if constexpr (DefaultSequentiallyGroupable<FormatTag, Range>){
                try {
                    return tag_invoke(FormatTag{}, default_group_sequentially_tag{}, std::forward<Range>(range));
                } catch (const std::exception& e) {
                    throw tag_invoke_error("group_sequentially", typeid(std::ranges::range_value_t<Range>).name(), e.what());
                }
            }
        }
    } group_sequentially;
}
