export module recurseria.core.meta.helpers.associative_ops:groupable_noexcept;

import std;

export import :groupable;
export import :default_groupable;

export namespace recurseria {
    template <typename FormatTag, typename Range>
    concept AssociativelyGroupableNoexcept =
        (UserDefinedAssociativelyGroupable<FormatTag, Range> &&
         noexcept(tag_invoke(FormatTag{}, group_associatively_tag{}, std::declval<Range>()))) ||
        (DefaultAssociativelyGroupable<FormatTag, Range> &&
         noexcept(tag_invoke(FormatTag{}, default_group_associatively_tag{}, std::declval<Range>())));
}
