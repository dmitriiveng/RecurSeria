module;

#include <iterator>
#include <ranges>

export module serde.core.meta.functions.default_functions.template_containers.inserting.inserting_cpo;

export import serde.core.meta.functions.tag_invokable;

export namespace serde::functions {

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

    //custom
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

    //concept
    template <typename Container>
    concept InsertableContainer =
        CustomInsertableContainer<Container> ||
        DefaultInsertableContainer<Container>;

    //cpo fallback
    inline constexpr struct get_output_iterator_fn {
        template<typename Container>
            requires InsertableContainer<Container>
        constexpr auto operator()(Container& container) const {
            if constexpr (CustomInsertableContainer<Container>){
                return tag_invoke(get_output_iterator_tag{}, container);
            }
            else if constexpr (DefaultInsertableContainer<Container>){
                return tag_invoke(default_get_output_iterator_tag{}, container);
            }
        }
    } get_output_iterator;
}
