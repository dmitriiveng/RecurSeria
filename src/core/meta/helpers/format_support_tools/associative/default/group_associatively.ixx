export module recurseria.core.meta.helpers.associative_ops:group_associatively_default;

import std;
import :pair_concept;
import :default_groupable;

import recurseria.core.meta.helpers.sequence_ops;

/*
export namespace recurseria::core::meta {
    template <typename FormatTag, std::ranges::input_range Range>
        requires
            PairLike<std::ranges::range_value_t<Range>> &&
            ( DefaultSequentiallyGroupable<FormatTag, Range> || SequentiallyGroupable<FormatTag, Range> )
    auto tag_invoke(FormatTag, default_group_associatively_tag, Range&& range) {
        using PairType = std::ranges::range_value_t<Range>;
        using E = std::common_type_t<
            std::tuple_element_t<0, std::remove_cvref_t<PairType>>,
            std::tuple_element_t<1, std::remove_cvref_t<PairType>>
        >;

        std::vector<E> flat;
        for (auto&& pair : range) {
            flat.push_back(std::get<0>(std::forward<decltype(pair)>(pair)));
            flat.push_back(std::get<1>(std::forward<decltype(pair)>(pair)));
        }

        return group_sequentially(FormatTag{}, flat);
    }
}
*/
