export module serde.core.meta.functions.types.default_serializable;

export import serde.core.meta.functions.tag_invokable;

export namespace serde::functions {

    struct default_serialize_tag {};

    template <typename Output, typename Input>
    concept DefaultSerializable = requires(Output& out, const Input& value) {
        tag_invoke(default_serialize_tag{}, out, value);
    };
}