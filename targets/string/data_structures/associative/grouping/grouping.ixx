module;
#include <map>
#include <ranges>

export module serde.targets.string.data_structures.associative.grouping;

import serde.targets.string.string_utils.to_structure.make_associative_structure_of_tokens;
import serde.core.meta.functions.data_structures.associative.grouping;
import serde.target.string.wrapped_string;

export namespace serde::targets::string {
    using group_associatively_tag = serde::functions::group_associatively_tag;

    void tag_invoke(group_associatively_tag, WrappedString& out, const std::map<WrappedString, WrappedString>& input) {
        std::map<std::string, std::string> str_map;
        for(const auto& pair : input | std::views::transform([](const auto& kv) {
            return std::pair{ 
                kv.first.value, 
                kv.second.value 
            };})
        ){
            str_map.insert(pair);
        }

        out = WrappedString(
            str_utils::make_associative_structure_of_tokens(
                str_map,
                '[', ']'
            )
        );
    }
}
