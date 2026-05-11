module;

#include <ranges>
#include <vector>

export module recurseria.targets.string.intermediate_representation:sequence_grouping;

import recurseria.core.meta.helpers.sequence_ops;
import :format_tag;
import :tree_node;

export namespace recurseria::target::string {
    using group_sequentially_tag = recurseria::core::meta::group_sequentially_tag;

    auto tag_invoke(
        string_intermediate_representation_format_tag,
        group_sequentially_tag,
        std::ranges::input_range auto&& input
    ) -> StringIRTreeNode {
        std::vector<StringIRTreeNode> vec(
            std::ranges::begin(input),
            std::ranges::end(input)
        );
        return StringIRTreeNode(std::move(vec));
    }
}
