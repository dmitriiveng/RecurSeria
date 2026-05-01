module;

#include <ranges>

export module recurseria.core.meta.types_srlz_dsrlz:containers_concepts;

export import recurseria.core.meta.helpers.output_iterator_getter;

export namespace recurseria::core::meta {
    template <typename Container>
    concept SerializableContainer = std::ranges::input_range<Container>;

    template <typename Container>
    concept DeserializableContainer = InsertableContainer<Container>;
}
