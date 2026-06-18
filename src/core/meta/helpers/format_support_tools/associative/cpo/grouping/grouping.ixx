export module recurseria.core.meta.helpers.associative_ops:grouping;

import std;
export import :default_groupable;
export import :groupable;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.exceptions;

export namespace recurseria::core::meta {
    inline constexpr struct group_associatively_fn {
        template <typename FormatTag, std::ranges::input_range Range>
            requires AssociativelyGroupable<FormatTag, Range> || DefaultAssociativelyGroupable<FormatTag, Range>
        constexpr auto operator()(FormatTag, Range&& range) const {
            if constexpr (AssociativelyGroupable<FormatTag, Range>){
                try {
                    return tag_invoke(FormatTag{}, group_associatively_tag{}, std::forward<Range>(range));
                } catch (const std::exception& e) {
                    throw tag_invoke_error("group_associatively", typeid(std::ranges::range_value_t<Range>).name(), e.what());
                }
            }
            else if constexpr (DefaultAssociativelyGroupable<FormatTag, Range>){
                try {
                    return tag_invoke(FormatTag{}, default_group_associatively_tag{}, std::forward<Range>(range));
                } catch (const std::exception& e) {
                    throw tag_invoke_error("group_associatively", typeid(std::ranges::range_value_t<Range>).name(), e.what());
                }
            }
        }
    } group_associatively;
}
