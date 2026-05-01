module;
#include <iterator>
#include <ranges>

export module recurseria.core.meta.helpers.output_iterator_getter:default_cpo;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    //default
    struct default_get_output_iterator_tag{};

    template <typename Container>
    concept DefaultInsertableContainer =
        requires(Container& c) {
            tag_invoke(default_get_output_iterator_tag{}, c);
            requires std::output_iterator<
                decltype(tag_invoke(default_get_output_iterator_tag{}, c)),
                std::ranges::range_value_t<Container>
            >;
        };
}
