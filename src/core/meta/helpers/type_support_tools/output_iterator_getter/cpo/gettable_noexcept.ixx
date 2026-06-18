export module recurseria.core.meta.helpers.output_iterator_getter:gettable_noexcept;

import std;

export import :gettable;

export namespace recurseria::core::meta {
    template <typename Container>
    concept InsertableContainerNoexcept =
        (CustomInsertableContainer<Container> &&
         noexcept(tag_invoke(get_output_iterator_tag{}, std::declval<Container&>()))) ||
        (DefaultInsertableContainer<Container> &&
         noexcept(tag_invoke(default_get_output_iterator_tag{}, std::declval<Container&>())));
}
