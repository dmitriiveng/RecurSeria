export module recurseria.core.meta.helpers.associative_ops:default_groupable;

import std;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    struct default_group_associatively_tag{};

    template <typename FormatTag, typename Range>
    concept DefaultAssociativelyGroupable =
        std::ranges::input_range<Range> &&
        requires(Range&& range) {
            tag_invoke(FormatTag{}, default_group_associatively_tag{}, std::forward<Range>(range));
        };
}
