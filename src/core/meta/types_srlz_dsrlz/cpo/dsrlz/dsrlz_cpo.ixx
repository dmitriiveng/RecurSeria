module;
#include <concepts>
#include <exception>
#include <typeinfo>

export module recurseria.core.meta.types_srlz_dsrlz:dsrlz;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.chain;
export import recurseria.core.meta.exceptions;
import :default_dsrlz;

export namespace recurseria::core::meta {
    struct deserialize_tag {};

    template <typename FormatTag, typename Output, typename Input>
    concept TagInvokeDeserializable =
        (!std::is_reference_v<Output>) &&
        (!std::is_const_v<Output>) &&
        requires(const Input& value) {
            {tag_invoke(FormatTag{}, deserialize_tag{}, type_tag<Output>{}, value)} -> std::same_as<Output>;
        };

    template <typename FormatTag, typename Output, typename Input>
    concept Deserializable =
        TagInvokeDeserializable<FormatTag, Output, Input> ||
        DefaultDeserializable<FormatTag, Output, Input>;

    /// Deserializes a value from the input format.
    ///
    /// @tparam FormatTag serialization format tag
    /// @tparam Output    target C++ type
    /// @tparam Input     input representation type
    /// @param  value     input data to deserialize from
    /// @return the deserialized value of type `Output`
    ///
    /// @throws tag_invoke_error if the underlying `tag_invoke` raises an exception.
    inline constexpr struct deserialize_fn {
        template<typename FormatTag, typename Output, typename Input>
            requires Deserializable<FormatTag, std::remove_cvref_t<Output>, Input>
        constexpr std::remove_cvref_t<Output> as(const Input& value) const {
            using CleanOutput = std::remove_cvref_t<Output>;
            if constexpr (TagInvokeDeserializable<FormatTag, CleanOutput, Input>){
                try {
                    return tag_invoke(FormatTag{}, deserialize_tag{}, type_tag<CleanOutput>{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("deserialize", typeid(CleanOutput).name(), e.what());
                }
            }
            else if constexpr(DefaultDeserializable<FormatTag, CleanOutput, Input>){
                try {
                    return tag_invoke(FormatTag{}, default_deserialize_tag{}, type_tag<CleanOutput>{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("deserialize", typeid(CleanOutput).name(), e.what());
                }
            }
        }

        template<typename FormatTag, typename Output, typename Input, typename Chain>
            requires IsChain<Chain>
        constexpr std::remove_cvref_t<Output> as(const Input& value) const {
            using CleanOutput = std::remove_cvref_t<Output>;
            auto last = fold_left(
                [this]<typename T, typename Fmt = FormatTag>(const auto& v) -> decltype(auto) {
                    return this->template as<Fmt, T>(v);
                },
                value,
                Chain{}
            );
            return this->template as<FormatTag, CleanOutput>(last);
        }
    } deserialize;
}
