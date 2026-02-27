module;
#include <map>

export module serde.core.meta.functions.default_functions.template_containers.serialization:map;

import serde.core.meta.functions.data_structures.associative.grouping;
import serde.core.meta.functions.types.serialization;

export namespace serde::functions {
    template <typename Output, typename InputKey, typename InputValue>
    void tag_invoke(serialize_tag, Output& out, const std::map<InputKey, InputValue>& input){
        std::map<Output, Output> output_map;

        for (const auto& pair : input) {
            Output key{};
            serialize(key, pair.first);

            Output value{};
            serialize(value, pair.second);

            output_map.emplace(std::move(key), std::move(value));
        }

        Output result{};
        group_associatively(result, output_map);

        out = std::move(result);
    }
}