export module recurseria.core.meta.types_srlz_dsrlz:deserializable_noexcept;

import std;

export import :deserializable;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename Output, typename Input>
    concept DeserializableNoexcept =
        (TagInvokeDeserializable<FormatTag, Output, Input> &&
         noexcept(tag_invoke(FormatTag{}, deserialize_tag{}, type_tag<Output>{}, std::declval<const Input&>()))) ||
        (DefaultDeserializable<FormatTag, Output, Input> &&
         noexcept(tag_invoke(FormatTag{}, default_deserialize_tag{}, type_tag<Output>{}, std::declval<const Input&>())));
}
