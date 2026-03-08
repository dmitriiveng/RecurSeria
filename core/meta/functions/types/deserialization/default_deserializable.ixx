export module serde.core.meta.functions.types.default_deserializable;

export import serde.core.meta.functions.tag_invokable;

export namespace serde::functions {
    struct default_deserialize_tag {};

    template <typename Output, typename Input>
    concept DefaultDeserializable = requires(Output& out, const Input& value) {
        tag_invoke(default_deserialize_tag{}, out, value);
    };
}