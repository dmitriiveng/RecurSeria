export module recurseria.core.meta.helpers.output_iterator_getter:gettable;

import std;

export import recurseria.core.meta.tag_invokable;
export import :default_cpo;

export namespace recurseria {
    struct get_output_iterator_tag {};

    template <typename Container>
    concept CustomInsertableContainer =
        requires(Container& c) {
            tag_invoke(get_output_iterator_tag{}, c);
            requires std::output_iterator<
                decltype(tag_invoke(get_output_iterator_tag{}, c)),
                std::ranges::range_value_t<Container>
            >;
        };

    template <typename Container>
    concept InsertableContainer =
        CustomInsertableContainer<Container> ||
        DefaultInsertableContainer<Container>;
}
