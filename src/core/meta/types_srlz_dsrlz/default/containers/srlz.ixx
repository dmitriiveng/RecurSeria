export module recurseria.core.meta.types_srlz_dsrlz:containers_srlz;

import std;

// helpers
import recurseria.core.meta.helpers.sequence_ops;
import recurseria.core.meta.helpers.output_iterator_getter;

// concepts
import :containers_concepts;

// default dsrlz
import :default_srlz;
import :srlz;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename Output, typename InputContainer>
    requires
        DeserializableContainer<InputContainer> &&
        SerializableContainer<InputContainer> &&
        (
            TagInvokeSerializable<FormatTag, Output, std::ranges::range_value_t<InputContainer>> ||
            DefaultSerializable<FormatTag, Output, std::ranges::range_value_t<InputContainer>>
        )
    Output tag_invoke(FormatTag, default_serialize_tag, type_tag<Output>, const InputContainer& input){
        auto transformed_input = input | std::ranges::views::transform([](const auto& element){
            return serialize.as<FormatTag, Output, std::ranges::range_value_t<InputContainer>>(element);
        });

        Output result = group_sequentially(FormatTag{}, transformed_input);

        return result;
    }
}
