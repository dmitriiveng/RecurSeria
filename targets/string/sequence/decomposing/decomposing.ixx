module;
#include <vector>
#include <ranges>

export module serde.targets.string.data_structures.sequence.decomposing;

import serde.targets.string.string_utils.from_structure.split_by_tokens_sequence;
import serde.core.meta.functions.data_structures.sequence.decomposing;

export namespace serde::functions {

    void tag_invoke(
        decompose_sequentially_tag, 
        std::vector<std::string>& out, 
        const std::string& input
    ) {
        auto it_pair_begin = input.begin();
        auto it_pair_end = input.end();

        auto seq = str_utils::split_by_tokens_sequence<
            decltype(it_pair_begin),
            decltype(it_pair_end)
        >(
            it_pair_begin,
            it_pair_end,
            '[', ']', '"'
        );

        out = std::move(seq);
    }
}
