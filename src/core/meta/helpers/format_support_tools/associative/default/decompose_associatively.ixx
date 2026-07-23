export module recurseria.core.meta.helpers.associative_ops:decompose_associatively_default;

import std;
import :pair_concept;
import :default_decomposable;

import recurseria.core.meta.helpers.sequence_ops;

// The idea is that to use associative_ops with a given format,
// the corresponding tag_invoke() functions must be implemented for it.

/*
export namespace recurseria::core::meta {
    template <typename FormatTag, typename T>
        requires SequentiallyDecomposable<FormatTag, T> || DefaultSequentiallyDecomposable<FormatTag, T>
    auto tag_invoke(FormatTag, default_decompose_associatively_tag, const T& value) {
        auto flat = decompose_sequentially(FormatTag{}, value);
        std::vector<std::ranges::range_value_t<decltype(flat)>> vec;
        for (auto&& elem : flat) {
            vec.push_back(std::forward<decltype(elem)>(elem));
        }
        if(vec.size() % 2 != 0){
            throw tag_invoke_error(
                "default_decompose_associatively_tag",
                "sequentially decomposable range",
                "size cannot be devided by two"
            );
        }

        using E = std::ranges::range_value_t<decltype(vec)>;
        auto pair_count = vec.size() / 2;

        return std::views::iota(0ull, pair_count) // 0ull is an unsigned long long integer literal with a value of zero.
             | std::views::transform([vec = std::move(vec)](std::size_t i) {
                   return std::pair<E, E>{vec[2 * i], vec[2 * i + 1]};
               });
    }
}
*/
