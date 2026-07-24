export module recurseria.core.meta.types_srlz_dsrlz:reflection_concepts;

import std;

export import recurseria.core.meta.helpers.output_iterator_getter;

import recurseria.core.meta.helpers.reflection_helpers;
import recurseria.core.meta.helpers.sequence_ops;
import recurseria.core.meta.helpers.associative_ops;
import :serializable;
import :deserializable;

export namespace recurseria::core::meta {
    // ReflectionSupported concept defined in reflection_helpers.ixx

    template <typename FormatTag, typename Output, typename Input, std::size_t... Is>
    consteval bool all_members_serializable(std::index_sequence<Is...>) {
        return (Serializable<FormatTag, Output, member_type_t<Input, Is>> && ...);
    }

    template <typename FormatTag, typename Output, typename Input, std::size_t... Is>
    consteval bool all_members_deserializable(std::index_sequence<Is...>) {
        return (Deserializable<FormatTag, member_type_t<Output, Is>, Input> && ...);
    }

    template <typename FormatTag, typename Output, typename Input>
    concept SerializableReflection =
        ReflectionSupported<Input> &&
        (SequenceOpsSupported<FormatTag, Output> || AssociativeOpsSupported<FormatTag, Output>) &&
        all_members_serializable<FormatTag, Output, Input>(
            std::make_index_sequence<std::meta::nonstatic_data_members_of(
                ^^Input, std::meta::access_context::unchecked()).size()>{});

    template <typename FormatTag, typename Output, typename Input>
    concept DeserializableReflection =
        ReflectionSupported<Output> &&
        (SequenceOpsSupported<FormatTag, Input> || AssociativeOpsSupported<FormatTag, Input>) &&
        all_members_deserializable<FormatTag, Output, Input>(
            std::make_index_sequence<std::meta::nonstatic_data_members_of(
                ^^Output, std::meta::access_context::unchecked()).size()>{});

    template <typename FormatTag, typename Type, typename FormatType>
    concept ReflectionSerializableDeserializable =
        SerializableReflection<FormatTag, FormatType, Type> &&
        DeserializableReflection<FormatTag, Type, FormatType>;

}
