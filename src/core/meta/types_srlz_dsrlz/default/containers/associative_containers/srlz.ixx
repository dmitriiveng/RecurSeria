export module recurseria.core.meta.types_srlz_dsrlz:associative_containers_srlz;

import std;

// helpers
import recurseria.core.meta.helpers.associative_ops;
import recurseria.core.meta.helpers.output_iterator_getter;
// concepts
import :associative_containers_concepts;
// default dsrlz
import :default_srlz;
import :srlz;

export namespace recurseria {
    template <typename FormatTag, typename Output, typename InputContainer>
    requires SerializableDeserializableAssociativeContainer<FormatTag, InputContainer, Output>
    Output tag_invoke(FormatTag, default_serialize_tag, type_tag<Output>, const InputContainer& input){
        using KeyType = InputContainer::key_type;

        auto transformed_input = input | std::ranges::views::transform([](const auto& element){
            return std::pair<Output, Output>{
                serialize.as<FormatTag, Output, KeyType>(std::get<0>(element)),
                serialize.as<FormatTag, Output, decltype(std::get<1>(element))>(std::get<1>(element))
            };
        });

        Output result = group_associatively(FormatTag{}, transformed_input);

        return result;
    }
}
