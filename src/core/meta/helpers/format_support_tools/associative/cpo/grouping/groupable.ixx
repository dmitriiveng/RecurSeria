export module recurseria.core.meta.helpers.associative_ops:groupable;

import std;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    struct group_associatively_tag {};

    template <typename FormatTag, typename Range>
    concept AssociativelyGroupable =
        std::ranges::input_range<Range> &&
        requires(Range&& range) {
            {
                tag_invoke(FormatTag{}, group_associatively_tag{}, std::forward<Range>(range))
            } -> std::same_as<std::remove_cvref_t<std::ranges::range_value_t<Range>>>;
        };
}
