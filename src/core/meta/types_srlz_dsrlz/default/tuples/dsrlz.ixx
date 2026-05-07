module;
#include <tuple>
#include <vector>
#include <stdexcept>
#include <typeinfo>
#include <string>

export module recurseria.core.meta.types_srlz_dsrlz:tuple_types_dsrlz;
// helpers
import recurseria.core.meta.helpers.sequence_ops;
// default dsrlz
import :default_dsrlz;
import :dsrlz;

export namespace recurseria::core::meta {
    template <typename T>
    concept TupleLike = requires {
        typename std::tuple_size<std::remove_cvref_t<T>>::type;
    };

    template <typename FormatTag, typename OutputTuple, typename Input>
    requires TupleLike<OutputTuple>
    OutputTuple tag_invoke(FormatTag, default_deserialize_tag, type_tag<OutputTuple>, const Input& input) {
        std::vector<Input> input_vector;
        decompose_sequentially(FormatTag{}, input_vector, input);

        constexpr std::size_t N = std::tuple_size_v<std::remove_cvref_t<OutputTuple>>;
        if (input_vector.size() != N) {
            throw std::logic_error(
                std::string("Input size (")
                + std::to_string(input_vector.size())
                + ") does not match tuple size ("
                + std::to_string(N)
                + ") for type: "
                + typeid(OutputTuple).name()
            );
        }

        // Constructing and returning new tuple
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
            return OutputTuple{
                deserialize.as<FormatTag, std::remove_cvref_t<std::tuple_element_t<I, OutputTuple>>, decltype(input_vector[I])>(input_vector[I])...
            };
        }(std::make_index_sequence<N>{});
    }
}
