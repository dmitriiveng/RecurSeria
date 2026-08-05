export module recurseria.srlz_dsrlz:default_dsrlz;

import std;

export import recurseria.tag_invokable;

export namespace recurseria {
    struct default_deserialize_tag {};

    template <typename FormatTag, typename Output, typename Input>
    concept DefaultDeserializable =
        // Output value should not be const or refence
        (!std::is_reference_v<Output>) &&
        (!std::is_const_v<Output>) &&
        // tag_invoke
        requires(const Input& value) {
            {tag_invoke(FormatTag{}, default_deserialize_tag{}, type_tag<Output>{}, value)} -> std::same_as<Output>;
        };
}
