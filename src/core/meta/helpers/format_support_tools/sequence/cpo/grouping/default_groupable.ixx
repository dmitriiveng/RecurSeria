export module recurseria.core.meta.helpers.sequence_ops:default_groupable;

import std;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    struct default_group_sequentially_tag{};

    template <typename FormatTag, typename Range>
    concept DefaultSequentiallyGroupable =
        std::ranges::input_range<Range> &&
        requires(Range&& range) {
            tag_invoke(FormatTag{}, default_group_sequentially_tag{}, std::forward<Range>(range));
        };
}
