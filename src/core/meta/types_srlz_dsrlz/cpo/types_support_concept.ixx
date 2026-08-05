export module recurseria.core.meta.types_srlz_dsrlz:types_support_concept;

import std;

import :serializable;
import :deserializable;

export namespace recurseria {
    // For now made by tuple

    // TODO: replace TupleLike with concept from std later
    template <typename T>
    concept TupleLike = requires {
        typename std::tuple_size<std::remove_cvref_t<T>>::type;
    };

    template <typename FormatTag, typename Output, typename InputTuple, std::size_t... Is>
    consteval bool all_tuple_elements_serializable(std::index_sequence<Is...>) {
        return (Serializable<FormatTag, Output, std::remove_cvref_t<std::tuple_element_t<Is, InputTuple>>> && ...);
    }

    template <typename FormatTag, typename OutputTuple, typename Input, std::size_t... Is>
    consteval bool all_tuple_elements_deserializable(std::index_sequence<Is...>) {
        return (Deserializable<FormatTag, std::remove_cvref_t<std::tuple_element_t<Is, OutputTuple>>, Input> && ...);
    }

    template <typename FormatTag, typename Output, typename InputTuple>
    concept SerializableTuple =
        TupleLike<InputTuple> &&
        all_tuple_elements_serializable<FormatTag, Output, InputTuple>(
            std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<InputTuple>>>{}
        );

    template <typename FormatTag, typename OutputTuple, typename Input>
    concept DeserializableTuple =
        TupleLike<OutputTuple> &&
        all_tuple_elements_deserializable<FormatTag, OutputTuple, Input>(
            std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<OutputTuple>>>{}
        );

    template <typename FormatTag, typename Type, typename FormatType>
    concept SerializableDeserializableTuple =
        SerializableTuple<FormatTag, FormatType, Type> &&
        DeserializableTuple<FormatTag, Type, FormatType>;
}
