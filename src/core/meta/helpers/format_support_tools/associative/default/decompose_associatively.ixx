export module recurseria.core.meta.helpers.associative_ops:decompose_associatively_default;

import std;
import :pair_concept;
import :default_decomposable;

import recurseria.core.meta.helpers.sequence_ops;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename T>
        requires SequentiallyDecomposable<FormatTag, T>
    auto tag_invoke(FormatTag, default_decompose_associatively_tag, const T& value) {
        auto flat = decompose_sequentially(FormatTag{}, value);
        std::vector<std::ranges::range_value_t<decltype(flat)>> vec;
        for (auto&& elem : flat) {
            vec.push_back(std::forward<decltype(elem)>(elem));
        }

        using E = std::ranges::range_value_t<decltype(vec)>;
        auto pair_count = vec.size() / 2;

        return std::views::iota(0ull, pair_count)
             | std::views::transform([vec = std::move(vec)](std::size_t i) {
                   return std::pair<E, E>{vec[2 * i], vec[2 * i + 1]};
               });
    }
}
