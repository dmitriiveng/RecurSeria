export module recurseria.core.meta.types_srlz_dsrlz:containers_dsrlz;

import std;

// helpers
import recurseria.core.meta.helpers.sequence_ops;
import recurseria.core.meta.helpers.output_iterator_getter;
// concepts
export import :containers_concepts;
// default dsrlz
import :default_dsrlz;
import :dsrlz;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename OutputContainer, typename Input>
    requires SerializableDeserializableContainer<FormatTag, OutputContainer, Input>
    OutputContainer tag_invoke(FormatTag, default_deserialize_tag, type_tag<OutputContainer>, const Input& input){

        auto deserialized_input = std::views::transform(decompose_sequentially(FormatTag{}, input), [](const Input& element) {
            return deserialize.as<FormatTag, std::ranges::range_value_t<OutputContainer>, Input>(element);
        });

        // creating output container and getting output iterator for it
        OutputContainer result;
        auto output_it = get_output_iterator(result);

        // copying deserialized elements into output container
        std::ranges::copy(deserialized_input, output_it);

        return result;
    }
}
