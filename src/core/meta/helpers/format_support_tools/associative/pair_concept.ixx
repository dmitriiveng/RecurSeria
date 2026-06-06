export module recurseria.core.meta.helpers.associative_ops:pair_concept;

import std;

export namespace recurseria::core::meta {
    template <typename T>
    concept PairLike = requires {
        typename std::tuple_size<std::remove_cvref_t<T>>::type;
    } && (std::tuple_size_v<std::remove_cvref_t<T>> == 2);
}
