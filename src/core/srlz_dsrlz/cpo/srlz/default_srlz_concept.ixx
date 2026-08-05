export module recurseria.srlz_dsrlz:default_srlz;

import std;

export import recurseria.tag_invokable;

export namespace recurseria {
    struct default_serialize_tag {};

    template <typename FormatTag, typename Output, typename Input>
    concept DefaultSerializable =
        // Output value should not be const or reference
        (!std::is_reference_v<Output>) &&
        (!std::is_const_v<Output>) &&
        // tag_invoke
        requires(const Input& value) {
            {tag_invoke(FormatTag{}, default_serialize_tag{}, type_tag<Output>{}, value)} -> std::same_as<Output>;
        };
}
