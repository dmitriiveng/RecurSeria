export module recurseria.core.meta.helpers.associative_ops:pair_concept;

import std;

export namespace recurseria::core::meta {
    template <typename T>
    concept PairLike = requires {
        typename std::tuple_size<std::remove_cvref_t<T>>::type;
    } &&
    requires (T element) {
        std::get<0>(element);
        std::get<1>(element);
    } &&
    std::constructible_from<
        T,
        std::tuple_element_t<0, std::remove_cvref_t<T>>,
        std::tuple_element_t<1, std::remove_cvref_t<T>>
        > &&
    (std::tuple_size_v<std::remove_cvref_t<T>> == 2);
}
