export module serde.targets.string.data_structures.associative.decomposing;

import <map>;
import <ranges>;
import serde.targets.string.string_utils.from_structure.split_by_tokens_associative;
import serde.core.meta.functions.data_structures.associative.decomposing;
import serde.target.string.wrapped_string;

export namespace serde::targets::string {
    using decompose_associatively_tag = serde::functions::decompose_associatively_tag;

    void tag_invoke(decompose_associatively_tag, std::map<WrappedString, WrappedString>& out, const WrappedString& input) {
        auto it_pair_begin = input.value.begin();
        auto it_pair_end = input.value.end();

        auto str_map = str_utils::split_by_tokens_associative<
            decltype(it_pair_begin),
            decltype(it_pair_end)
        >(
            it_pair_begin,
            it_pair_end,
            '[',
            ']',
            '"'
        );

        out = str_map
            | std::views::transform([](const auto& kv) {
                return std::pair{
                    WrappedString{kv.first},
                    WrappedString{kv.second}
                };
            })
            | std::ranges::to<std::map<WrappedString, WrappedString>>();
    }
}
