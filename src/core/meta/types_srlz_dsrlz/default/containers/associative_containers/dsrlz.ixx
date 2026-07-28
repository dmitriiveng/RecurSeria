export module recurseria.core.meta.types_srlz_dsrlz:associative_containers_dsrlz;

import std;

// helpers
import recurseria.core.meta.helpers.associative_ops;
import recurseria.core.meta.helpers.output_iterator_getter;
// concepts
export import :associative_containers_concepts;
// default dsrlz
import :default_dsrlz;
import :dsrlz;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename OutputContainer, typename Input>
    requires SerializableDeserializableAssociativeContainer<FormatTag, OutputContainer, Input>
    OutputContainer tag_invoke(FormatTag, default_deserialize_tag, type_tag<OutputContainer>, const Input& input){
        using PairValue = std::ranges::range_value_t<OutputContainer>;
        using KeyType = OutputContainer::key_type;
        using MappedType = OutputContainer::mapped_type;

        auto deserialized_input = std::views::transform(decompose_associatively(FormatTag{}, input), [](const auto& element) {
            auto key = deserialize.as<FormatTag, KeyType>(std::get<0>(element));
            auto value = deserialize.as<FormatTag,  MappedType>(std::get<1>(element));
            return PairValue(std::move(key), std::move(value));
        });

        // creating output container and getting output iterator for it
        OutputContainer result;
        auto output_it = get_output_iterator(result);

        // copying deserialized elements into output container
        std::ranges::copy(deserialized_input, output_it);

        return result;
    }
}
