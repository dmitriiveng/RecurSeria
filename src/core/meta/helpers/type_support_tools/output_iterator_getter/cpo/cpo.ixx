export module recurseria.core.meta.helpers.output_iterator_getter:cpo;

import std;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.exceptions;
export import :gettable;
export import :gettable_noexcept;

export namespace recurseria::core::meta {
    inline constexpr struct get_output_iterator_fn {
        template<typename Container>
            requires InsertableContainerNoexcept<Container>
        constexpr auto operator()(Container& container) const noexcept
        {
            if constexpr (CustomInsertableContainer<Container> &&
                          noexcept(tag_invoke(get_output_iterator_tag{}, container)))
            {
                return tag_invoke(get_output_iterator_tag{}, container);
            } else {
                return tag_invoke(default_get_output_iterator_tag{}, container);
            }
        }

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
            else {
                try {
                    return tag_invoke(default_get_output_iterator_tag{}, container);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("get_output_iterator", typeid(Container).name(), e.what());
                }
            }
        }
    } get_output_iterator;
}
