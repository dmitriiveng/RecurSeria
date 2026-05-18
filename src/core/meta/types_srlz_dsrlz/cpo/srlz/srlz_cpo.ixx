module;
#include <concepts>
#include <exception>
#include <typeinfo>

export module recurseria.core.meta.types_srlz_dsrlz:srlz;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.chain;
export import recurseria.core.meta.exceptions;
import :default_srlz;

export namespace recurseria::core::meta {
    struct serialize_tag {};

    template <typename FormatTag, typename Output, typename Input>
    concept TagInvokeSerializable =
        (!std::is_reference_v<Output>) &&
        (!std::is_const_v<Output>) &&
        requires(const Input& value) {
            {tag_invoke(FormatTag{}, serialize_tag{}, type_tag<Output>{}, value)} -> std::same_as<Output>;
        };

    template <typename FormatTag, typename Output, typename Input>
    concept Serializable =
        TagInvokeSerializable<FormatTag, Output, Input> ||
        DefaultSerializable<FormatTag, Output, Input>;

    /// Serializes a value into the target format.
    ///
    /// @tparam FormatTag serialization format tag
    /// @tparam Output    target type (e.g. `std::string`)
    /// @tparam Input     source value type
    /// @param  value     value to serialize
    /// @return the serialized representation
    ///
    /// @throws tag_invoke_error if the underlying `tag_invoke` raises an exception.
    inline constexpr struct serialize_fn {
        template<typename FormatTag, typename Output, typename Input>
            requires Serializable<FormatTag, Output, Input>
        constexpr Output as(const Input& value) const {
            if constexpr (TagInvokeSerializable<FormatTag, Output, Input>){
                try {
                    return tag_invoke(FormatTag{}, serialize_tag{}, type_tag<Output>{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("serialize", typeid(Input).name(), e.what());
                }
            }
            else if constexpr(DefaultSerializable<FormatTag, Output, Input>){
                try {
                    return tag_invoke(FormatTag{}, default_serialize_tag{}, type_tag<Output>{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("serialize", typeid(Input).name(), e.what());
                }
            }
        }

        template<typename FormatTag, typename Chain, typename Output, typename Input>
            requires IsChain<Chain>
        constexpr Output as(const Input& value) const {
            auto last = fold_left(
                [this]<typename T>(const auto& v) {
                    return this->template as<FormatTag, T>(v);
                },
                value,
                Chain{}
            );
            return this->template as<FormatTag, Output>(last);
        }
    } serialize;
}
