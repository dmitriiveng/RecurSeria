module;

#include <iterator>
#include <ranges>

export module serde.core.meta.functions.default_functions.template_containers.inserting.default_implementations:back_inserter_inserting;

export import serde.core.meta.functions.default_functions.template_containers.inserting.inserting_cpo;

export namespace serde::functions {

    template <typename Container>
    concept HasPushBack =
        requires(Container& c) {
            c.push_back(std::ranges::range_value_t<Container>{});
        };

    template <typename Container>
    concept HasInsert =
        requires(Container& c) {
            c.insert(c.end(), std::ranges::range_value_t<Container>{});
        };

    template <typename Container>
    requires HasPushBack<Container> || HasInsert<Container>
    auto tag_invoke(default_get_output_iterator_tag, Container& container){
        if constexpr (HasPushBack<Container>) {
            return std::back_inserter(container);
        } else {
            return std::inserter(container, container.end());
        }
    }
}
