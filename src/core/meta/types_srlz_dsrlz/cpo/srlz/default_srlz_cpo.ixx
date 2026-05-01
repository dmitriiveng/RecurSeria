export module recurseria.core.meta.types_srlz_dsrlz:default_srlz;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    struct default_serialize_tag {};

    template <typename Output, typename Input>
    concept DefaultSerializable = requires(Output& out, const Input& value) {
        tag_invoke(default_serialize_tag{}, out, value);
    };
}
