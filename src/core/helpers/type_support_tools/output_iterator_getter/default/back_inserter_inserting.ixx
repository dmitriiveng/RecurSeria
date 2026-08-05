export module recurseria.helpers.output_iterator_getter:default_back_inserter_inserting;

import std;

import :cpo;
import :default_cpo;

export namespace recurseria {

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
