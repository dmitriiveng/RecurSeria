module;
#include <vector>
#include <string>

export module recurseria.targets.string:sequence_decomposing;

import recurseria.core.meta.helpers.sequence_ops;
import recurseria.targets.string.utils;
import :format_tag;

export namespace recurseria::target::string {
    using decompose_sequentially_tag = recurseria::core::meta::decompose_sequentially_tag;

    void tag_invoke(
        default_string_format_tag,
        decompose_sequentially_tag,
        std::vector<std::string>& out,
        const std::string& input
    ) {
        auto it_pair_begin = input.begin();
        auto it_pair_end = input.end();

        auto seq = recurseria::targets::string::split_by_tokens_sequence<
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
