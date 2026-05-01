module;

#include <vector>
#include <ranges>

export module recurseria.core.meta.types_srlz_dsrlz:containers_srlz;

// helpers
import recurseria.core.meta.helpers.sequence_ops;
import recurseria.core.meta.helpers.output_iterator_getter;
// concepts
import :containers_concepts;
// default dsrlz
import :default_srlz;
import :srlz;

export namespace recurseria::core::meta {
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
