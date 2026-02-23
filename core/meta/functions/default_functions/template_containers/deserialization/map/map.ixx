export module serde.core.meta.functions.default_functions.template_containers.deserialization:map;

import <map>;
import serde.core.meta.functions.data_structures.associative.decomposing;
import serde.core.meta.functions.types.deserialization;

export namespace serde::functions {
    template <typename OutputKey, typename OutputValue, typename Input>
    void tag_invoke(deserialize_tag, std::map<OutputKey, OutputValue>& out, const Input& input){
        std::map<Input, Input> elements;
        decompose_associatively(elements, input);

        std::map<OutputKey, OutputValue> result;

        for (const auto& element : elements) {
            OutputKey key{};
            deserialize(key, element.first);

            OutputValue value{};
            deserialize(value, element.second);

            result.emplace(std::move(key), std::move(value));
        }

        out = std::move(result);
    }
}