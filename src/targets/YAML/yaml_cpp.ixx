module;
#include <yaml-cpp/yaml.h>
export module yaml_cpp;

export {
    namespace YAML {
        using YAML::Node;
        using YAML::Emitter;
        using YAML::iterator;
        using YAML::const_iterator;
        using YAML::Dump;

        using YAML::operator<<;

        namespace NodeType {
            using YAML::NodeType::value;
            using YAML::NodeType::Undefined;
            using YAML::NodeType::Null;
            using YAML::NodeType::Scalar;
            using YAML::NodeType::Sequence;
            using YAML::NodeType::Map;
        }
    }
}
