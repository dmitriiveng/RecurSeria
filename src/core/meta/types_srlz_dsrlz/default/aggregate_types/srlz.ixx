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

    template <typename FormatTag, typename Output, typename Input>
    requires std::is_aggregate_v<Input>
    Output tag_invoke(FormatTag, default_serialize_tag, type_tag<Output>, const Input& input){
        constexpr std::size_t fields_count = AggregateFieldsGetter::template field_count<Input>();

        std::vector<Output> output_vector;
        output_vector.reserve(fields_count);

        AggregateFieldsGetter::for_each_field(input, [&output_vector](const auto& field) {
            output_vector.push_back(
                serialize.as<FormatTag, Output, decltype(field)>(field)
            );
        });

        Output result{};
        group_sequentially(FormatTag{}, result, output_vector);
        return result;
    }
}
