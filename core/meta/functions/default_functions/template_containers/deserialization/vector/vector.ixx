module;
#include <vector>

export module serde.core.meta.functions.default_functions.template_containers.deserialization:vector;

import serde.core.meta.functions.data_structures.sequence.decomposing;
import serde.core.meta.functions.types.deserialization;

namespace serde::functions {
    template <typename Output, typename Input>
    void default_deserialize_fn(std::vector<Output>& out, const Input& input){
        std::vector<Input> elements;
        decompose_sequentially(elements, input);

        std::vector<Output> result;
        result.reserve(elements.size());

        for (const auto& element : elements) {
            Output value{};
            deserialize(value, element);
            result.push_back(std::move(value));
        }

        out = std::move(result);
    }
}

