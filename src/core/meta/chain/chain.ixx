export module recurseria.core.meta.chain;

import std;
import recurseria.core.meta.tag_invokable;
export import :type_format;

namespace recurseria::core::meta {
    /// Type-level list of intermediate representations.
    ///
    /// Used with `serialize.as` / `deserialize.as` to specify a pipeline
    /// of conversions. The chain is processed left-to-right:
    ///
    ///     serialize.as<FormatTag, chain<A, B>, Output>(x)
    ///
    /// applies `A` first, then `B`, then `Output`:
    ///
    ///     x -> A -> B -> Output
    ///
    /// @tparam Ts  intermediate types, processed in declaration order
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

    /// Type-level argument pack for passing multiple CPO arguments as one type.
    ///
    /// Simple serialization:
    ///     arg_pack<FormatTag, Output, Input>
    ///
    /// Pipeline serialization:
    ///     arg_pack<FormatTag, Chain, Output, Input>
    export template <typename... Ts>
    using arg_pack = chain<Ts...>;

    /// Applies `func<T>(value)` or `func<T, Fmt>(value)` for each
    /// element in the chain. Plain types pass `<T>`; `type_format<T, Fmt>`
    /// entries pass `<T, Fmt>`. Results are threaded left-to-right.
    export template <typename Func, typename Input, typename Last>
    constexpr auto fold_left(
        Func&& func,
        Input&& input,
        chain<Last>
    ) {
        if constexpr (is_type_format_v<Last>) {
            return func.template operator()<typename Last::type, typename Last::format>(std::forward<Input>(input));
        } else {
            return func.template operator()<Last>(std::forward<Input>(input));
        }
    }

    export template <typename Func, typename Input, typename First, typename... Rest>
        requires (sizeof...(Rest) > 0)
    constexpr auto fold_left(
        Func&& func,
        Input&& input,
        chain<First, Rest...>
    ) {
        if constexpr (is_type_format_v<First>) {
            return fold_left(
                std::forward<Func>(func),
                func.template operator()<typename First::type, typename First::format>(std::forward<Input>(input)),
                chain<Rest...>{}
            );
        } else {
            return fold_left(
                std::forward<Func>(func),
                func.template operator()<First>(std::forward<Input>(input)),
                chain<Rest...>{}
            );
        }
    }
}
