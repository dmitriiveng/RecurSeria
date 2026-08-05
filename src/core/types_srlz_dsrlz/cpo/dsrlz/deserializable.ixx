export module recurseria.srlz_dsrlz:deserializable;

import std;

export import recurseria.tag_invokable;
export import :default_dsrlz;

export namespace recurseria {
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
}
