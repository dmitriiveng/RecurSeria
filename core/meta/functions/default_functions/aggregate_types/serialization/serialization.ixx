module;
#include <concepts>
#include <vector>
#include <stdexcept>
#include <__type_traits/is_aggregate.h>

export module serde.core.meta.functions.default_functions.aggregate_types.serialization;

import serde.core.meta.functions.data_structures.sequence.grouping;
import serde.core.meta.functions.types.serialization;
import serde.core.meta.functions.default_functions.aggregate_types.fields_getters.boost_pfr_fields_getter;

export namespace serde::functions {
    //TODO find another way to set this
    using AggregateFieldsGetter = PfrFieldsGetter;

    template <typename T>
    using clean_type = std::remove_cv_t<std::remove_reference_t<T>>;

    template <typename Output, typename Input>
    requires std::is_aggregate_v<Input>
    void tag_invoke(serialize_tag, Output& out, const Input& input){
        constexpr std::size_t fields_count = AggregateFieldsGetter::template field_count<Input>();

        std::vector<Output> output_vector;
        output_vector.reserve(fields_count);

        AggregateFieldsGetter::for_each_field(input, [&output_vector](const auto& field) {
            Output output_field{};
            serialize(output_field, field);
            output_vector.push_back(output_field);
        });

        Output result{};
        group_sequentially(result, output_vector);
        out = std::move(result);
    }
}