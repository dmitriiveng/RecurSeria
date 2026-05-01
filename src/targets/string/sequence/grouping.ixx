module;
#include <vector>
#include <ranges>

export module recurseria.targets.string:sequence_grouping;

import recurseria.core.meta.helpers.sequence_ops;
import recurseria.targets.string.utils;

export namespace recurseria::core::meta {
    void tag_invoke(
        group_sequentially_tag,
        std::string& out,
        const std::vector<std::string>& input
    ) {
        out = recurseria::targets::string::make_sequence_of_tokens(input, '[', ']');
    }
}
