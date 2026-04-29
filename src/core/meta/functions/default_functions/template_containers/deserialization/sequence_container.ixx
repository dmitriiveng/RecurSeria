module;

#include <vector>
#include <ranges>

export module serde.core.meta.functions.default_functions.template_containers.deserialization:sequence_container;

import serde.core.meta.functions.default_functions.template_containers.inserting.inserting_cpo;
import serde.core.meta.functions.default_functions.template_containers.concepts;
import serde.core.meta.functions.data_structures.sequence.decomposing;
import serde.core.meta.functions.types.default_deserializable;
import serde.core.meta.functions.types.deserialization;

export namespace serde::functions {
    template <typename OutputContainer, typename Input>
    requires
        DeserializableContainer<OutputContainer> &&
        (
            TagInvokeDeserializable<std::ranges::range_value_t<OutputContainer>, Input> ||
            DefaultDeserializable<std::ranges::range_value_t<OutputContainer>, Input>
        )
    void tag_invoke(default_deserialize_tag, OutputContainer& out, const Input& input){
        // getting input elements
        // TODO replace with views based later
        std::vector<Input> elements;
        decompose_sequentially(elements, input);

        // deserializing every element
        // TODO replace with pipe notation
        auto deserialized_input = std::views::transform(elements, [](const Input& element) {
            std::ranges::range_value_t<OutputContainer> value{};
            deserialize(value, element);
            return value;
        });

        // creating output container and output iterator for it
        OutputContainer result;
        auto output_it = get_output_iterator(result);

        // copying deserialized elements into output container
        std::ranges::copy(deserialized_input, output_it);

        // moving output container into output reference
        out = std::move(result);
    }
}
