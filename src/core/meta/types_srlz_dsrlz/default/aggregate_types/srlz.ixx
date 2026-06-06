export module recurseria.core.meta.types_srlz_dsrlz:aggregate_types_srlz;

import std;

// helpers
import recurseria.core.meta.helpers.sequence_ops;
// default dsrlz
import :default_srlz;
import :srlz;
import :aggregate_fields_getter_setter;

export namespace recurseria::core::meta {

    template <typename T>
    using clean_type = std::remove_cv_t<std::remove_reference_t<T>>;

    template <typename FormatTag, typename Output, typename Input>
    requires std::is_aggregate_v<Input>
    Output tag_invoke(FormatTag, default_serialize_tag, type_tag<Output>, const Input& input){

        //TODO rewrite without vector
        constexpr std::size_t fields_count = AggregateFieldsGetter::template field_count<Input>();

        std::vector<Output> output_vector;
        output_vector.reserve(fields_count);

        AggregateFieldsGetter::for_each_field(input, [&output_vector](const auto& field) {
            output_vector.push_back(
                serialize.as<FormatTag, Output, decltype(field)>(field)
            );
        });

        return group_sequentially(FormatTag{}, output_vector);
    }
}
