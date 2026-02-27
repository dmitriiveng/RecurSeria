module;
#include <vector>
#include <ranges>

export module serde.targets.string.data_structures.sequence.grouping;

import serde.targets.string.string_utils.to_structure.make_sequence_of_tokens;
import serde.core.meta.functions.data_structures.sequence.grouping;

namespace serde::functions {
    void tag_invoke(
        group_sequentially_tag, 
        std::string& out, 
        const std::vector<std::string>& input
    ) {
        out = str_utils::make_sequence_of_tokens(input, '[', ']');
    }
}
