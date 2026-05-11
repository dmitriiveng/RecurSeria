module;
#include <tuple>
#include <utility>
#include <type_traits>

export module recurseria.core.meta.types_srlz_dsrlz:aggregate_types_dsrlz;

// helpers
import recurseria.core.meta.helpers.sequence_ops;
// default dsrlz
import :default_dsrlz;
import :dsrlz;
import :aggregate_fields_getter_setter;

export namespace recurseria::core::meta {

    // tag_invoke
    template <typename FormatTag, typename Output, typename Input>
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
