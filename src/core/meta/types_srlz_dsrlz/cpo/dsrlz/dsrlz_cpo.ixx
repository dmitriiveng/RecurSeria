module;
#include <concepts>

export module recurseria.core.meta.types_srlz_dsrlz:dsrlz;

export import recurseria.core.meta.tag_invokable;
import :default_dsrlz;

export namespace recurseria::core::meta {
    struct deserialize_tag {};

    template <typename FormatTag, typename Output, typename Input>
    concept TagInvokeDeserializable =
        // Output value shopuld not be const or reference
        (!std::is_reference_v<Output>) &&
        (!std::is_const_v<Output>) &&
        // tag_invoke exists
        requires(const Input& value) {
            {tag_invoke(FormatTag{}, deserialize_tag{}, type_tag<Output>{}, value)} -> std::same_as<Output>;
        };

    template <typename FormatTag, typename Output, typename Input>
    concept Deserializable =
        TagInvokeDeserializable<FormatTag, Output, Input> ||
        DefaultDeserializable<FormatTag, Output, Input>;

    // CPO
    inline constexpr struct deserialize_fn {
        template<typename FormatTag, typename Output, typename Input>
            requires Deserializable<FormatTag, Output, Input>
        constexpr Output as(const Input& value) const {
            if constexpr (TagInvokeDeserializable<FormatTag, Output, Input>){
                return tag_invoke(FormatTag{}, deserialize_tag{}, type_tag<Output>{}, value);
            }
            else if constexpr(DefaultDeserializable<FormatTag, Output, Input>){
                return tag_invoke(FormatTag{}, default_deserialize_tag{}, type_tag<Output>{}, value);
            }
        }
    } deserialize;
}
