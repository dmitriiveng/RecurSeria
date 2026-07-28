export module recurseria.core.meta.types_srlz_dsrlz:containers_concepts;

import std;

export import recurseria.core.meta.helpers.output_iterator_getter;

import recurseria.core.meta.helpers.sequence_ops;

import :default_dsrlz;
import :dsrlz;

import :default_srlz;
import :srlz;

import :associative_containers_concepts;

export namespace recurseria::core::meta {
    template <typename Container>
    concept SrlzSupportedContainer = std::ranges::input_range<Container>;

    template <typename Container>
    concept DsrlzSupportedContainer = InsertableContainer<Container>;

    template <typename FormatTag, typename Output, typename Container>
    concept SerializableContainer =
        SrlzSupportedContainer<Container> &&
        SequenceOpsSupported<FormatTag, Output> &&
        Serializable<FormatTag, Output, std::ranges::range_value_t<Container>>;

    template <typename FormatTag, typename Container, typename Input>
    concept DeserializableContainer =
        DsrlzSupportedContainer<Container> &&
        SequenceOpsSupported<FormatTag, Input> &&
        Deserializable<FormatTag, std::ranges::range_value_t<Container>, Input>;

    template <typename FormatTag, typename Container, typename T>
    concept SerializableDeserializableContainer =
        (!SerializableDeserializableAssociativeContainer<FormatTag, Container, T>) &&
        SerializableContainer<FormatTag, T, Container> &&
        DeserializableContainer<FormatTag, Container, T>;
}
