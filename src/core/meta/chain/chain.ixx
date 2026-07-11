export module recurseria.core.meta.chain;

import std;
export import :type_format;

namespace recurseria::core::meta {
    export template <typename... Ts>
    struct chain final {};

    template <typename T>
    struct unchain;

    template <typename... Ts>
    struct unchain<chain<Ts...>> final{
        using type = chain<Ts...>;
    };

    /// Checks whether `T` is a `chain<...>` instantiation.
    export template <typename T>
    concept IsChain = requires {
        typename unchain<T>::type;
        requires std::same_as<T, typename unchain<T>::type>;
    };

    /// Reverses a chain's type order.
    ///
    /// `chain_reverse<chain<A, B, C>>` yields `chain<C, B, A>`.
    /// @tparam T  a `chain<...>` instantiation
    export template <typename T>
        requires IsChain<T>
    struct chain_reverse;

    template <typename... Ts>
    struct chain_reverse<chain<Ts...>> {
        template <typename Acc, typename... Rest>
        struct rev;

        template <typename... Acc>
        struct rev<chain<Acc...>> {
            using type = chain<Acc...>;
        };

        template <typename... Acc, typename First, typename... Rest>
        struct rev<chain<Acc...>, First, Rest...> {
            using type = typename rev<chain<First, Acc...>, Rest...>::type;
        };

        using type = typename rev<chain<>, Ts...>::type;
    };

    /// Alias for `chain_reverse<T>::type`.
    export template <typename T>
        requires IsChain<T>
    using chain_reverse_t = typename chain_reverse<T>::type;

    export template <typename... Ts>
    using arg_pack = chain<Ts...>;
}
