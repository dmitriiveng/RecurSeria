module;

#include <vector>
#include <ranges>

export module serde.core.meta.functions.default_functions.template_containers.serialization:sequence_container;

import serde.core.meta.functions.default_functions.template_containers.inserting.inserting_cpo;
import serde.core.meta.functions.default_functions.template_containers.concepts;
import serde.core.meta.functions.data_structures.sequence.grouping;
import serde.core.meta.functions.types.default_serializable;
import serde.core.meta.functions.types.serialization;

export namespace serde::functions {
    template <typename Output, typename InputContainer>
    requires
        SerializableContainer<InputContainer> &&
        (
            TagInvokeSerializable<Output, std::ranges::range_value_t<InputContainer>> ||
            DefaultSerializable<Output, std::ranges::range_value_t<InputContainer>>
        )
    void tag_invoke(default_serialize_tag, Output& out, const InputContainer& input){
        std::vector<Output> output_vector;

        auto transformed_input = input | std::ranges::views::transform([](const auto& element){
            Output value{};
            serialize(value, element);
            return value;
        });

        std::ranges::copy(transformed_input, get_output_iterator(output_vector));

        Output result{};
        group_sequentially(result, output_vector);

        out = std::move(result);
    }
}
