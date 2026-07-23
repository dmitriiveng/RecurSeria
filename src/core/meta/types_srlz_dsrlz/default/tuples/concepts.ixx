export module recurseria.core.meta.types_srlz_dsrlz:tuple_concepts;

import std;

export namespace recurseria::core::meta {
    template <typename T>
    concept TupleLike = requires {
        typename std::tuple_size<std::remove_cvref_t<T>>::type;
    };
}
TupleLike