module;
#include <tuple>
#include <type_traits>

export module recurseria.core.meta.types_srlz_dsrlz:srlz_pack_traits;

import recurseria.core.meta.chain;
import :srlz;

export namespace recurseria::core::meta {
    template <typename>
    struct serialize_args;

    template <typename... Ts>
    struct serialize_args<arg_pack<Ts...>> {
        static_assert(sizeof...(Ts) >= 3);
        using as_tuple = std::tuple<Ts...>;
        using format_tag = std::tuple_element_t<0, as_tuple>;
        using output_type = std::tuple_element_t<1, as_tuple>;
        using input_type = std::tuple_element_t<2, as_tuple>;
    };

    template <typename, typename = void>
    struct is_serialize_callable : std::false_type {};

    template <typename... Ts>
    struct is_serialize_callable<
        arg_pack<Ts...>,
        std::void_t<typename serialize_args<arg_pack<Ts...>>::format_tag>
    > : std::bool_constant<
        (sizeof...(Ts) >= 3) &&
        requires(const typename serialize_args<arg_pack<Ts...>>::input_type& v) {
            serialize.as<Ts...>(v);
        }
    > {};

    template <typename Args>
    concept SerializeArgPack = is_serialize_callable<Args>::value;
}
