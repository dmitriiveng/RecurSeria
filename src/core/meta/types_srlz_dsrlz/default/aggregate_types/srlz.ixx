module;
#include <vector>

export module recurseria.core.meta.types_srlz_dsrlz:aggregate_types_srlz;

// helpers
import recurseria.core.meta.helpers.sequence_ops;
import recurseria.core.meta.helpers.fields_getter;
// default dsrlz
import :default_srlz;
import :srlz;

export namespace recurseria::core::meta {
    //TODO find another way to set this
    using AggregateFieldsGetter = PfrFieldsGetter;

    template <typename T>
    using clean_type = std::remove_cv_t<std::remove_reference_t<T>>;

    template <typename Output, typename Input>
    requires std::is_aggregate_v<Input>
    void tag_invoke(default_serialize_tag, Output& out, const Input& input){
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
