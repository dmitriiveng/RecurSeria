module;
#include <stdexcept>

export module recurseria.core.meta.types_srlz_dsrlz:aggregate_types_dsrlz;

// helpers
import recurseria.core.meta.helpers.sequence_ops;
import recurseria.core.meta.helpers.fields_getter;
// default dsrlz
import :default_dsrlz;
import :dsrlz;

namespace recurseria::core::meta {
    //TODO find another way to set this
    using AggregateFieldsGetter = PfrFieldsGetter;

    // tag_invoke
    export template <typename FormatTag, typename Output, typename Input>
        requires std::is_aggregate_v<Output>
    Output tag_invoke(FormatTag, default_deserialize_tag, type_tag<Output>, const Input& input){
        using TupleType = AggregateFieldsGetter::template tuple_type<Output>;

        TupleType tuple = deserialize.as<
            FormatTag,
            TupleType
        >(input);

        // Using aggregate constructor
        return std::apply(
            []<typename... Args>(Args&&... args) {
                return Output{std::forward<Args>(args)...};
            },
            std::move(tuple)
        );
    }
}
