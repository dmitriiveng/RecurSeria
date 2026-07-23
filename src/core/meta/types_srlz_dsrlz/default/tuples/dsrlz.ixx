export module recurseria.core.meta.types_srlz_dsrlz:tuple_types_dsrlz;

import std;
// helpers
import recurseria.core.meta.helpers.sequence_ops;
// default dsrlz
import :default_dsrlz;
import :dsrlz;
import :tuple_concepts;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename OutputTuple, typename Input>
    requires TupleLike<OutputTuple>
    OutputTuple tag_invoke(FormatTag, default_deserialize_tag, type_tag<OutputTuple>, const Input& input) {
        using ClearOutputTuple = std::remove_cvref_t<OutputTuple>;
        constexpr std::size_t TupleSize = std::tuple_size_v<ClearOutputTuple>;

        auto decomposed = decompose_sequentially(FormatTag{}, input);

        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return ClearOutputTuple{
                deserialize.as<FormatTag, std::tuple_element_t<I, ClearOutputTuple>>(decomposed[I])...
            };
        }(std::make_index_sequence<TupleSize>{});
    }
}
