export module recurseria.core.meta.types_srlz_dsrlz:reflection_dsrlz;

import std;

import recurseria.core.meta.helpers.reflection_helpers;
import recurseria.core.meta.helpers.sequence_ops;
import :default_dsrlz;
import :dsrlz;

namespace recurseria::core::meta {
    /*

    // local helper: looks up each reflected member by name, deserializes via CPO
    template <typename FormatTag, typename T, typename Input, std::size_t... Is>
    auto lookup_members(const std::map<std::string, Input>& data, std::index_sequence<Is...>) {
        return std::make_tuple(
            deserialize.as<FormatTag, member_type_t<T, Is>, Input>(
                data.at(std::string(std::meta::identifier_of(get_nth_member<T, Is>())))
            )...
        );
    }

    template <typename FormatTag, typename Output, typename Input>
    requires ReflectionSupported<Output> && std::is_aggregate_v<Output>
    Output tag_invoke(FormatTag, default_deserialize_tag, type_tag<Output>, const Input& input) {
        constexpr auto N = std::meta::nonstatic_data_members_of(
            ^^Output,
            std::meta::access_context::unchecked()
        ).size();

        std::map<std::string, Input> data;
        for (const auto& [key, value] : decompose_sequentially(FormatTag{}, input)) {
            data.emplace(deserialize.as<FormatTag, std::string, Input>(key), value);
        }

        auto tup = lookup_members<FormatTag, Output>(data, std::make_index_sequence<N>{});
        return build_aggregate<Output>(std::move(tup), std::make_index_sequence<N>{});
    }

    template <typename FormatTag, typename Output, typename Input>
    requires ReflectionSupported<Output> && (!std::is_aggregate_v<Output>)
    Output tag_invoke(FormatTag, default_deserialize_tag, type_tag<Output>, const Input& input) {
        Output obj{};

        std::map<std::string, Input> data;
        for (const auto& [key, value] : decompose_sequentially(FormatTag{}, input)) {
            data.emplace(deserialize.as<FormatTag, std::string, Input>(key), value);
        }

        iterate_through_fields_nv(obj, [&](std::string_view name, auto& field) {
            using FieldType = std::remove_cvref_t<decltype(field)>;
            field = deserialize.as<FormatTag, FieldType, Input>(
                data.at(std::string(name))
            );
        });

        return obj;
    }
    */
}
