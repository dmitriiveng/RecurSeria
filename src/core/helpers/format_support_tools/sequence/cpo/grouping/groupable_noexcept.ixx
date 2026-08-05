export module recurseria.helpers.sequence_ops:groupable_noexcept;

import std;

export import :groupable;
export import :default_groupable;

export namespace recurseria {
    template <typename FormatTag, typename Range>
    concept SequentiallyGroupableNoexcept =
        (UserDefinedSequentiallyGroupable<FormatTag, Range> &&
         noexcept(tag_invoke(FormatTag{}, group_sequentially_tag{}, std::declval<Range>()))) ||
        (DefaultSequentiallyGroupable<FormatTag, Range> &&
         noexcept(tag_invoke(FormatTag{}, default_group_sequentially_tag{}, std::declval<Range>())));
}
