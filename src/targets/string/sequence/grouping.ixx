module;
#include <vector>
#include <string>

export module recurseria.targets.string:sequence_grouping;

import recurseria.core.meta.helpers.sequence_ops;
import recurseria.targets.string.utils;
import :format_tag;

export namespace recurseria::target::string {
    using group_sequentially_tag = recurseria::core::meta::group_sequentially_tag;

    void tag_invoke(
        default_string_format_tag,
        group_sequentially_tag,
        std::string& out,
        const std::vector<std::string>& input
    ) {
        out = recurseria::targets::string::make_sequence_of_tokens(input, '[', ']');
    }
}
