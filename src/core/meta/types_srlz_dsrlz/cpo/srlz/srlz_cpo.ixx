module;
#include <concepts>

export module recurseria.core.meta.types_srlz_dsrlz:srlz;

export import recurseria.core.meta.tag_invokable;
import :default_srlz;

export namespace recurseria::core::meta {
    struct serialize_tag {};

    template <typename FormatTag, typename Output, typename Input>
    concept TagInvokeSerializable =
        // Output value shopuld not be const or reference
        (!std::is_reference_v<Output>) &&
        (!std::is_const_v<Output>) &&
        // tag_invoke
        requires(const Input& value) {
            {tag_invoke(FormatTag{}, serialize_tag{}, type_tag<Output>{}, value)} -> std::same_as<Output>;
        };

    template <typename FormatTag, typename Output, typename Input>
    concept Serializable =
        TagInvokeSerializable<FormatTag, Output, Input> ||
        DefaultSerializable<FormatTag, Output, Input>;

    inline constexpr struct serialize_fn {
        template<typename FormatTag, typename Output, typename Input>
            requires Serializable<FormatTag, Output, Input>
        constexpr Output as(const Input& value) const {
            if constexpr (TagInvokeSerializable<FormatTag, Output, Input>){
                return tag_invoke(FormatTag{}, serialize_tag{}, type_tag<Output>{}, value);
            }
            else if constexpr(DefaultSerializable<FormatTag, Output, Input>){
                return tag_invoke(FormatTag{}, default_serialize_tag{}, type_tag<Output>{}, value);
            }
        }
    } serialize;
}
