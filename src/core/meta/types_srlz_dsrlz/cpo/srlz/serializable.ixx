export module recurseria.core.meta.types_srlz_dsrlz:serializable;

import std;

export import recurseria.core.meta.tag_invokable;
export import :default_srlz;

export namespace recurseria {
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
}
