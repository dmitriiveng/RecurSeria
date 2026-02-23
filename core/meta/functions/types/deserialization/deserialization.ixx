export module serde.core.meta.functions.types.deserialization;

export import serde.core.meta.functions.tag_invokable;

export namespace serde::functions {
    struct deserialize_tag {};

    template <typename Output, typename Input>
    concept Deserializable = requires(Output& out, const Input& value) {
            tag_invoke(deserialize_tag{}, out, value);
    };

    inline constexpr struct deserialize_fn {
        template<typename Output, typename Input>
            requires Deserializable<Output, Input>
        constexpr void operator()(Output& out, const Input& value) const
            noexcept(noexcept(tag_invoke(deserialize_tag{}, out, value)))
        {
            tag_invoke(deserialize_tag{}, out, value);
        }
    } deserialize;
}

