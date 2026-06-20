export module recurseria.core.meta.helpers.sequence_ops:grouping;

import std;
export import :default_groupable;
export import :groupable;
export import :groupable_noexcept;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.exceptions;

export namespace recurseria::core::meta {
    inline constexpr struct group_sequentially_fn {
        template <typename FormatTag, std::ranges::input_range Range>
            requires SequentiallyGroupableNoexcept<FormatTag, Range>
        constexpr auto operator()(FormatTag, Range&& range) const noexcept
        {
            if constexpr (SequentiallyGroupable<FormatTag, Range> &&
                          noexcept(tag_invoke(FormatTag{}, group_sequentially_tag{}, std::forward<Range>(range))))
            {
                return tag_invoke(FormatTag{}, group_sequentially_tag{}, std::forward<Range>(range));
            } else {
                return tag_invoke(FormatTag{}, default_group_sequentially_tag{}, std::forward<Range>(range));
            }
        }

        template <typename FormatTag, std::ranges::input_range Range>
            requires SequentiallyGroupable<FormatTag, Range> || DefaultSequentiallyGroupable<FormatTag, Range>
        constexpr auto operator()(FormatTag, Range&& range) const {
            if constexpr (SequentiallyGroupable<FormatTag, Range>){
                try {
                    return tag_invoke(FormatTag{}, group_sequentially_tag{}, std::forward<Range>(range));
                } catch (const std::exception& e) {
                    throw tag_invoke_error("group_sequentially", typeid(std::ranges::range_value_t<Range>).name(), e.what());
                } catch (...) {
                    throw tag_invoke_error("group_sequentially", typeid(std::ranges::range_value_t<Range>).name(), "unknown exception");
                }
            }
            else {
                try {
                    return tag_invoke(FormatTag{}, default_group_sequentially_tag{}, std::forward<Range>(range));
                } catch (const std::exception& e) {
                    throw tag_invoke_error("group_sequentially", typeid(std::ranges::range_value_t<Range>).name(), e.what());
                } catch (...) {
                    throw tag_invoke_error("group_sequentially", typeid(std::ranges::range_value_t<Range>).name(), "unknown exception");
                }
            }
        }
    } group_sequentially;
}
