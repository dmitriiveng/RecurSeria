module;

#include <span>
#include <stdexcept>

export module recurseria.targets.string.intermediate_representation:sequence_decomposing;

import recurseria.core.meta.helpers.sequence_ops;
import :format_tag;
import :tree_node;

export namespace recurseria::target::string {
    using decompose_sequentially_tag = recurseria::core::meta::decompose_sequentially_tag;

    auto tag_invoke(
        string_intermediate_representation_format_tag,
        decompose_sequentially_tag,
        const StringIRTreeNode& input
    ) -> std::span<const StringIRTreeNode> {
        if (input.is_leaf()) {
            throw std::runtime_error("tree node is a leaf");
        }
        const auto& seq = input.get_sequence();
        return std::span<const StringIRTreeNode>(seq.data(), seq.size());
    }
}
