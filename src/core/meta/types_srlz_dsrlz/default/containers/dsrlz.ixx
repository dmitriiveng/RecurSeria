module;

#include <vector>
#include <ranges>

export module recurseria.core.meta.types_srlz_dsrlz:containers_dsrlz;

// helpers
import recurseria.core.meta.helpers.sequence_ops;
import recurseria.core.meta.helpers.output_iterator_getter;
// concepts
import :containers_concepts;
// default dsrlz
import :default_dsrlz;
import :dsrlz;

export namespace recurseria::core::meta {
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
