export module recurseria.core.meta.helpers.reflection_helpers;

import std;

namespace recurseria {

    export template<typename T>
    concept FullyPublicAndNamed = std::meta::is_class_type(^^T) &&
        !requires { typename std::tuple_size<std::remove_cvref_t<T>>::type; } &&
        [] {
            auto ctx = std::meta::access_context::unchecked();
            auto members = std::meta::nonstatic_data_members_of(^^T, ctx);
            if (members.size() == 0) return false;
            for (auto m : members)
                if (!std::meta::is_public(m) || !std::meta::has_identifier(m))
                    return false;
            return true;
        }();

    export template<typename T>
    concept ReflectionSupported =
        FullyPublicAndNamed<T> &&
        (
            std::is_default_constructible_v<T> ||
            std::is_aggregate_v<T>
        );

    export template <typename T, std::size_t I>
    consteval auto get_nth_member() {
        return std::meta::nonstatic_data_members_of(
            ^^T, std::meta::access_context::unchecked()
        )[I];
    }

    export template <typename T, typename Func, std::size_t... Is>
    constexpr void iterate_impl(T& object, Func&& function, std::index_sequence<Is...>) {
        ((function(
            std::meta::identifier_of(get_nth_member<T, Is>()),
            object.[:get_nth_member<T, Is>():]
        )), ...);
    }

    export template <typename T, typename Func>
    requires ReflectionSupported<T>
    constexpr void iterate_through_fields(T& object, Func&& function) {
        constexpr auto N = std::meta::nonstatic_data_members_of(
            ^^T, std::meta::access_context::unchecked()
        ).size();

        iterate_impl(object, std::forward<Func>(function), std::make_index_sequence<N>{});
    }

    export template <typename T, typename Tuple, std::size_t... Is>
    T build_aggregate(Tuple&& vals, std::index_sequence<Is...>) {
        return T{ std::get<Is>(std::forward<Tuple>(vals))... };
    }

    export template <typename T, std::size_t I>
    using member_type_t = [:std::meta::type_of(get_nth_member<T, I>()):];

}
