export module recurseria.helpers.sequence_ops:groupable;

import std;

export import recurseria.tag_invokable;
export import :default_groupable;

export namespace recurseria {
    struct group_sequentially_tag {};

    template <typename FormatTag, typename Range>
    concept UserDefinedSequentiallyGroupable =
        std::ranges::input_range<Range> &&
        requires(Range&& range) {
            tag_invoke(FormatTag{}, group_sequentially_tag{}, std::forward<Range>(range));
        };

    template <typename FormatTag, typename Range>
    concept SequentiallyGroupable =
        UserDefinedSequentiallyGroupable<FormatTag, Range> ||
        DefaultSequentiallyGroupable<FormatTag, Range>;
}
