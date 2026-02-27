module;
#include <vector>

export module serde.core.meta.functions.default_functions.template_containers.serialization:vector;

import serde.core.meta.functions.data_structures.sequence.grouping;
import serde.core.meta.functions.types.serialization;

namespace serde::functions {
    template <typename Output, typename Input>
    void default_serialize_fn(Output& out, const std::vector<Input>& input){
        std::vector<Output> output_vector;

        for (const auto& element : input) {
            Output value{};
            serialize(value, element);

            output_vector.push_back(std::move(element));
        }

        Output result{};
        group_sequentially(result, output_vector);

        out = std::move(result);
    }
}