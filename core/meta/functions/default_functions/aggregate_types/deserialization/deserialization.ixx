export module serde.core.meta.functions.default_functions.aggregate_types.deserialization;

import <concepts>;
import <vector>;
import <stdexcept>;
import serde.core.meta.functions.data_structures.sequence.decomposing;
import serde.core.meta.functions.types.deserialization;
import serde.core.meta.functions.default_functions.aggregate_types.fields_getters.boost_pfr_fields_getter;

export namespace serde::functions {
    //TODO find another way to set this
    using AggregateFieldsGetter = PfrFieldsGetter;

    template <typename Output, typename Input>
    requires std::is_aggregate_v<Output>
    void tag_invoke(deserialize_tag, Output& out, const Input& input){
        std::vector<Input> input_vector;
        decompose_sequentially(input_vector, input);

        constexpr std::size_t fields_count = AggregateFieldsGetter::template field_count<Output>();

        if (input_vector.size() != fields_count) {
            throw std::logic_error(
                std::string("Input size (") + std::to_string(input_vector.size())
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
            // Removing cv & ref
            auto result = std::tuple<
                std::remove_cv_t<std::remove_reference_t<std::tuple_element_t<I, TupleType>>>...
            >{};

            // Filling with elements
            (
                deserialize(
                    std::get<I>(result),
                    input_vector[I]
                ),
                ...
            );

            return result;
        }(std::make_index_sequence<fields_count>{});

        // Using aggregate constructor
        out = std::apply(
            []<typename... Args>(Args&&... args) {
                return Output{std::forward<Args>(args)...};
            },
            std::move(tuple_filled)
        );
    }
}