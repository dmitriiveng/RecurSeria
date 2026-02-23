export module serde.core.meta.functions.default_functions.aggregate_types.fields_getters.boost_pfr_fields_getter;

import <boost/pfr.hpp>;

export namespace serde{
    struct PfrFieldsGetter {
        template <typename Aggregate, typename Func>
        static void for_each_field(Aggregate& aggr, Func&& f) {
            boost::pfr::for_each_field(aggr, std::forward<Func>(f));
        }

        template<typename Aggregate>
        static constexpr std::size_t field_count() {
            return boost::pfr::tuple_size_v<Aggregate>;
        }

        template <typename Aggregate>
        using tuple_type = decltype(
            boost::pfr::structure_to_tuple(std::declval<Aggregate&>())
            );
    };
}