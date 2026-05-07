module;
#include <vector>
#include <stdexcept>

export module recurseria.core.meta.types_srlz_dsrlz:aggregate_types_dsrlz;

// helpers
import recurseria.core.meta.helpers.sequence_ops;
import recurseria.core.meta.helpers.fields_getter;
// default dsrlz
import :default_dsrlz;
import :dsrlz;

export namespace recurseria::core::meta {
    //TODO find another way to set this
    using AggregateFieldsGetter = PfrFieldsGetter;

    template <typename FormatTag, typename Output, typename Input>
    requires std::is_aggregate_v<Output>
    Output tag_invoke(FormatTag, default_deserialize_tag, type_tag<Output>, const Input& input){
        std::vector<Input> input_vector;
        decompose_sequentially(FormatTag{}, input_vector, input);

        constexpr std::size_t fields_count = AggregateFieldsGetter::template field_count<Output>();

        if (input_vector.size() != fields_count) {
            throw std::logic_error(
                std::string("Input size (")
                + std::to_string(input_vector.size())
                + ") does not match number of fields ("
                + std::to_string(fields_count)
                + ") in aggregate of type: "
                + typeid(Output).name()
            );
        }

        // Tuple type for fields of Output
        using TupleType = AggregateFieldsGetter::template tuple_type<Output>;

        // Making a tuple of fields from a vector of InputV
        auto tuple_filled = [&]<std::size_t... I>(std::index_sequence<I...>) {
            return std::tuple<
                std::remove_cv_t<std::remove_reference_t<std::tuple_element_t<I, TupleType>>>...
            >{
                deserialize.as<
                    FormatTag,
                    std::remove_cv_t<std::remove_reference_t<std::tuple_element_t<I, TupleType>>>,
                    Input
                >(input_vector[I])...
            };
        }(std::make_index_sequence<fields_count>{});

        // Using aggregate constructor
        return std::apply(
            []<typename... Args>(Args&&... args) {
                return Output{std::forward<Args>(args)...};
            },
            std::move(tuple_filled)
        );
    }
}
