module;
#include <vector>
#include <ranges>

export module serde.targets.string.data_structures.sequence.grouping;

import serde.targets.string.string_utils.to_structure.make_sequence_of_tokens;
import serde.core.meta.functions.data_structures.sequence.grouping;
import serde.target.string.wrapped_string;

export namespace serde::targets::string {
    using group_sequentially_tag = serde::functions::group_sequentially_tag;

    void tag_invoke(group_sequentially_tag, WrappedString& out, const std::vector<WrappedString>& input) {
        out = WrappedString(
            str_utils::make_sequence_of_tokens(
                input
                | std::views::transform([](const auto& element) {
                    return element.value;
                })
                | std::ranges::to<std::vector<std::string>>(),
                '[', ']'
            )
        );
    }
}
