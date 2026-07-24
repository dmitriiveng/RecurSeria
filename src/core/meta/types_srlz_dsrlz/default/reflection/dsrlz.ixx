export module recurseria.core.meta.types_srlz_dsrlz:reflection_dsrlz;

import std;

import recurseria.core.meta.helpers.reflection_helpers;
import recurseria.core.meta.helpers.sequence_ops;
import recurseria.core.meta.helpers.associative_ops;
import :reflection_concepts;
import :default_dsrlz;
import :dsrlz;

namespace recurseria::core::meta {
    // helpers

    // Span-based lookup for sequence variants.
    template <typename FormatTag, typename T, typename FlatRange, std::size_t... Is>
    auto lookup_members_sequential(const FlatRange& flat, std::index_sequence<Is...>) {
        return std::make_tuple(
            deserialize.as<FormatTag, member_type_t<T, Is>>(
                flat[Is]
            )...
        );
    }

    // Map-based lookup for associative variants.
    // Accesses values by member name via data.at(name).
    template <typename FormatTag, typename T, typename Input, std::size_t... Is>
    auto lookup_members(const std::map<std::string, Input>& data, std::index_sequence<Is...>) {
        return std::make_tuple(
            deserialize.as<FormatTag, member_type_t<T, Is>, Input>(
                data.at(std::string(std::meta::identifier_of(get_nth_member<T, Is>())))
            )...
        );
    }

    // Direct field assignment from flat span for non-aggregate sequential.
    template <typename FormatTag, typename Output, typename FlatRange, std::size_t... Is>
    void assign_sequential(Output& obj, const FlatRange& flat, std::index_sequence<Is...>) {
        ((obj.[:get_nth_member<Output, Is>():] =
            deserialize.as<FormatTag, member_type_t<Output, Is>>(flat[Is * 2 + 1])
        ), ...);
    }

    // functions

    // Sequence + Aggregate
    template <typename FormatTag, typename Output, typename Input>
    requires
        ReflectionSerializableDeserializable<FormatTag, Output, Input> &&
        std::is_aggregate_v<Output> &&
        SequentiallyDecomposable<FormatTag, Input> &&
        (!AssociativelyDecomposable<FormatTag, Input>)
    Output tag_invoke(FormatTag, default_deserialize_tag, type_tag<Output>, const Input& input) {
        constexpr auto N = std::meta::nonstatic_data_members_of(
            ^^Output, std::meta::access_context::unchecked()
        ).size();

        auto flat = decompose_sequentially(FormatTag{}, input);
        return build_aggregate<Output>(
            lookup_members_sequential<FormatTag, Output>(flat, std::make_index_sequence<N>{}),
            std::make_index_sequence<N>{}
        );
    }

    // Sequence + Non-aggregate
    template <typename FormatTag, typename Output, typename Input>
    requires
        ReflectionSerializableDeserializable<FormatTag, Output, Input> &&
        (!std::is_aggregate_v<Output>) &&
        SequentiallyDecomposable<FormatTag, Input> &&
        (!AssociativelyDecomposable<FormatTag, Input>)
    Output tag_invoke(FormatTag, default_deserialize_tag, type_tag<Output>, const Input& input) {
        constexpr auto N = std::meta::nonstatic_data_members_of(
            ^^Output, std::meta::access_context::unchecked()
        ).size();

        Output obj{};
        auto flat = decompose_sequentially(FormatTag{}, input);
        assign_sequential<FormatTag, Output>(obj, flat, std::make_index_sequence<N>{});
        return obj;
    }

    // Associative + Aggregate
    template <typename FormatTag, typename Output, typename Input>
    requires
        ReflectionSerializableDeserializable<FormatTag, Output, Input> &&
        std::is_aggregate_v<Output> &&
        AssociativelyDecomposable<FormatTag, Input>
    Output tag_invoke(FormatTag, default_deserialize_tag, type_tag<Output>, const Input& input) {
        constexpr auto N = std::meta::nonstatic_data_members_of(
            ^^Output, std::meta::access_context::unchecked()
        ).size();

        auto flat = decompose_associatively(FormatTag{}, input);
        std::map<std::string, Input> data;
        for (auto&& [k, v] : flat) {
            data.emplace(
                deserialize.as<FormatTag, std::string>(k),
                v
            );
        }

        auto tup = lookup_members<FormatTag, Output>(data, std::make_index_sequence<N>{});
        return build_aggregate<Output>(std::move(tup), std::make_index_sequence<N>{});
    }

    // Associative + Non-aggregate
    template <typename FormatTag, typename Output, typename Input>
    requires
        ReflectionSerializableDeserializable<FormatTag, Output, Input> &&
        (!std::is_aggregate_v<Output>) &&
        AssociativelyDecomposable<FormatTag, Input>
    Output tag_invoke(FormatTag, default_deserialize_tag, type_tag<Output>, const Input& input) {
        constexpr auto N = std::meta::nonstatic_data_members_of(
            ^^Output, std::meta::access_context::unchecked()
        ).size();

        Output obj{};
        auto flat = decompose_associatively(FormatTag{}, input);
        std::map<std::string, Input> data;
        for (auto&& [k, v] : flat) {
            data.emplace(
                deserialize.as<FormatTag, std::string>(k),
                v
            );
        }

        iterate_through_fields(obj, [&](std::string_view name, auto& field) {
            using FieldType = std::remove_cvref_t<decltype(field)>;
            field = deserialize.as<FormatTag, FieldType, Input>(
                data.at(std::string(name))
            );
        });

        return obj;
    }

}
