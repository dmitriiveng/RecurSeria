module;
#include <yaml-cpp/yaml.h>

export module recurseria.target.string.yaml_bridge:bridge_dsrlz;

import std;

import recurseria.core.meta.types_srlz_dsrlz;
import recurseria.target.string.intermediate_representation;
import :format_tag;

namespace recurseria::target::string {

    export StringIRTreeNode tag_invoke(
        yaml_bridge_format_tag,
        recurseria::core::meta::deserialize_tag,
        recurseria::core::meta::type_tag<StringIRTreeNode>,
        const YAML::Node& input
    ) {
        switch (input.Type()) {
            case YAML::NodeType::Scalar:
                return StringIRTreeNode(input.as<std::string>());

            case YAML::NodeType::Null:
                return StringIRTreeNode(std::string{});

            case YAML::NodeType::Sequence: {
                std::vector<StringIRTreeNode> children;
                children.reserve(input.size());
                for (const auto& child : input) {
                    children.push_back(
                        recurseria::core::meta::deserialize.as<yaml_bridge_format_tag, StringIRTreeNode>(child)
                    );
                }
                return StringIRTreeNode(std::move(children));
            }

            case YAML::NodeType::Map: {
                std::vector<StringIRTreeNode> children;
                children.reserve(input.size());
                for (const auto& entry : input) {
                    std::vector<StringIRTreeNode> kv;
                    kv.reserve(2);
                    kv.push_back(
                        recurseria::core::meta::deserialize.as<yaml_bridge_format_tag, StringIRTreeNode>(entry.first)
                    );
                    kv.push_back(
                        recurseria::core::meta::deserialize.as<yaml_bridge_format_tag, StringIRTreeNode>(entry.second)
                    );
                    children.push_back(StringIRTreeNode(std::move(kv)));
                }
                return StringIRTreeNode(std::move(children));
            }

            case YAML::NodeType::Undefined:
                throw std::runtime_error("cannot deserialize undefined YAML node");
        }

        throw std::runtime_error("unknown YAML node type");
    }

}
