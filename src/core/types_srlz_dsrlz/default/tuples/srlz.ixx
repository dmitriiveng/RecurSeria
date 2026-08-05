export module recurseria.srlz_dsrlz:tuple_types_srlz;

import std;
import recurseria.helpers.sequence_ops;
import :default_srlz;
import :srlz;
import :types_support_concept;

export namespace recurseria {
    template <typename FormatTag, typename Output, typename InputTuple>
    requires SerializableDeserializableTuple<FormatTag, InputTuple, Output>
    Output tag_invoke(FormatTag, default_serialize_tag, type_tag<Output>, const InputTuple& input) {
        //TODO rewrite without vector
        constexpr std::size_t N = std::tuple_size_v<std::remove_cvref_t<InputTuple>>;
        std::vector<Output> output_vector;
        output_vector.reserve(N);

        [&]<std::size_t... I>(std::index_sequence<I...>) {
            (
                [&output_vector, &input]() {
                    output_vector.push_back(
                        serialize.as<FormatTag, Output, decltype(std::get<I>(input))>(std::get<I>(input))
                    );
                }(),
                ...
            );
        }(std::make_index_sequence<N>{});

        return group_sequentially(FormatTag{}, output_vector);
    }
}
