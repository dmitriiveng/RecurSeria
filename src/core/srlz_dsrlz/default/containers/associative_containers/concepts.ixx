export module recurseria.srlz_dsrlz:associative_containers_concepts;

import std;

export import recurseria.helpers.output_iterator_getter;

import recurseria.helpers.associative_ops;

import :default_dsrlz;
import :dsrlz;

import :default_srlz;
import :srlz;

export namespace recurseria {
    template<typename Container>
    concept HasKeyCompare = requires { typename Container::key_compare; };

    template<typename Container>
    concept HasHasher = requires { typename Container::hasher; };

    template<typename Container>
    concept AssociativeContainer = requires {
        typename Container::key_type;
        typename Container::value_type;
    }
    && (HasKeyCompare<Container> || HasHasher<Container>)
    && requires(Container container, const typename Container::key_type& key) {
        { container.find(key) } -> std::same_as<typename Container::iterator>;
        { container.contains(key) } -> std::convertible_to<bool>;
    };

    template <typename Container>
    concept SrlzSupportedAssociativeContainer =
        std::ranges::input_range<Container>
        && AssociativeContainer<Container>;

    template <typename Container>
    concept DsrlzSupportedAssociativeContainer =
        InsertableContainer<Container>
        && AssociativeContainer<Container>;

    template <typename FormatTag, typename Output, typename Container>
    concept SerializableAssociativeContainer =
        SrlzSupportedAssociativeContainer<Container> &&
        AssociativeOpsSupported<FormatTag, Output> &&
        Serializable<FormatTag, Output, std::ranges::range_value_t<Container>>;

    template <typename FormatTag, typename Container, typename Input>
    concept DeserializableAssociativeContainer =
        DsrlzSupportedAssociativeContainer<Container> &&
        AssociativeOpsSupported<FormatTag, Input> &&
        Deserializable<FormatTag, std::ranges::range_value_t<Container>, Input>;

    template <typename FormatTag, typename Container, typename T>
    concept SerializableDeserializableAssociativeContainer =
        SerializableAssociativeContainer<FormatTag, T, Container> &&
        DeserializableAssociativeContainer<FormatTag, Container, T>;
}
