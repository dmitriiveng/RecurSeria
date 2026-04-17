module;

#include <ranges>

export module serde.core.meta.functions.default_functions.template_containers.concepts;

export import serde.core.meta.functions.default_functions.template_containers.inserting.inserting_cpo;

export namespace serde::functions {

    template <typename Container>
    concept SerializableContainer =
        std::ranges::input_range<Container>;


    template <typename Container>
    concept DeserializableContainer =
        InsertableContainer<Container>;
}
