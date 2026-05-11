module;
#include <exception>
#include <iterator>
#include <ranges>
#include <typeinfo>

export module recurseria.core.meta.helpers.output_iterator_getter:cpo;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.exceptions;
import :default_cpo;

export namespace recurseria::core::meta {
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

    inline constexpr struct get_output_iterator_fn {
        template<typename Container>
            requires InsertableContainer<Container>
        constexpr auto operator()(Container& container) const {
            if constexpr (CustomInsertableContainer<Container>){
                try {
                    return tag_invoke(get_output_iterator_tag{}, container);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("get_output_iterator", typeid(Container).name(), e.what());
                }
            }
            else if constexpr (DefaultInsertableContainer<Container>){
                try {
                    return tag_invoke(default_get_output_iterator_tag{}, container);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("get_output_iterator", typeid(Container).name(), e.what());
                }
            }
        }
    } get_output_iterator;
}
