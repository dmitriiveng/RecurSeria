export module recurseria.target.string.yaml_bridge:bridge_srlz;

import std;
import yaml_cpp;

import recurseria.core.meta.types_srlz_dsrlz;
import recurseria.target.string.intermediate_representation;
import :format_tag;

namespace recurseria::target::string {

    export YAML::Node tag_invoke(
        yaml_bridge_format_tag,
        recurseria::core::meta::serialize_tag,
        recurseria::core::meta::type_tag<YAML::Node>,
        const StringIRTreeNode& input
    ) {
        if (input.is_leaf()) {
            return YAML::Node(input.get_string());
        }

        YAML::Node node(YAML::NodeType::Sequence);
        for (const auto& child : input.get_sequence()) {
            node.push_back(
                recurseria::core::meta::serialize.as<yaml_bridge_format_tag, YAML::Node>(child)
            );
        }
        return node;
    }

}
