export module serde.targets.string.data_structures.sequence.decomposing;

import <vector>;
import <ranges>;
import serde.targets.string.string_utils.from_structure.split_by_tokens_sequence;
import serde.core.meta.functions.data_structures.sequence.decomposing;
import serde.target.string.wrapped_string;

export namespace serde::targets::string {
    using decompose_sequentially_tag = serde::functions::decompose_sequentially_tag;

    void tag_invoke(decompose_sequentially_tag, std::vector<WrappedString>& out, const WrappedString& input) {
        auto it_pair_begin = input.value.begin();
        auto it_pair_end = input.value.end();

        auto seq = str_utils::split_by_tokens_sequence<
            decltype(it_pair_begin),
            decltype(it_pair_end)
        >(
            it_pair_begin,
            it_pair_end,
            '[', ']', '"'
        );

        out = std::move(seq)
            | std::views::transform([](std::string& element) {
                return WrappedString(std::move(element));
            })
            | std::ranges::to<std::vector<WrappedString>>();
    }
}
