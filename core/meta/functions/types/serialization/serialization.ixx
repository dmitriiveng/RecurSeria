export module serde.core.meta.functions.types.serialization;

export import serde.core.meta.functions.tag_invokable;

export namespace serde::functions {
    struct serialize_tag {};

    template <typename Output, typename Input>
    concept Serializable = requires(Output& out, const Input& value) {
        tag_invoke(serialize_tag{}, out, value);
    };

    inline constexpr struct serialize_fn {
        template<typename Output, typename Input>
            requires Serializable<Output, Input>
        constexpr void operator()(Output& out, const Input& value) const
            noexcept(noexcept(tag_invoke(serialize_tag{}, out, value)))
        {
            tag_invoke(serialize_tag{}, out, value);
        }
    } serialize;
}

