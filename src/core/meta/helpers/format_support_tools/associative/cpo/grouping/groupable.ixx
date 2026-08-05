export module recurseria.core.meta.helpers.associative_ops:groupable;

import std;

export import recurseria.core.meta.tag_invokable;
export import :default_groupable;

export namespace recurseria {
    struct group_associatively_tag {};

    template <typename FormatTag, typename Range>
    concept UserDefinedAssociativelyGroupable =
        std::ranges::input_range<Range> &&
        requires(Range&& range) {
            tag_invoke(FormatTag{}, group_associatively_tag{}, std::forward<Range>(range));
        };

    template <typename FormatTag, typename Range>
    concept AssociativelyGroupable =
        UserDefinedAssociativelyGroupable<FormatTag, Range> ||
        DefaultAssociativelyGroupable<FormatTag, Range>;
}
