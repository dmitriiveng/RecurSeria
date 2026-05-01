export module recurseria.core.meta.types_srlz_dsrlz:default_dsrlz;

export import recurseria.core.meta.tag_invokable;

export namespace recurseria::core::meta {
    struct default_deserialize_tag {};

    template <typename Output, typename Input>
    concept DefaultDeserializable = requires(Output& out, const Input& value) {
        tag_invoke(default_deserialize_tag{}, out, value);
    };
}
