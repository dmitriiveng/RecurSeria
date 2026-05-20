module;

#include <type_traits>

export module recurseria.core.meta.chain:type_format;

export namespace recurseria::core::meta {
    template <typename T, typename Fmt>
    struct type_format {
        using type = T;
        using format = Fmt;
    };

    template <typename T>
    struct is_type_format_impl : std::false_type {};

    template <typename T, typename Fmt>
    struct is_type_format_impl<type_format<T, Fmt>> : std::true_type {};

    template <typename T>
    constexpr bool is_type_format_v = is_type_format_impl<T>::value;
}
