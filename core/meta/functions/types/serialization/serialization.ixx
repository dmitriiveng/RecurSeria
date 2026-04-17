export module serde.core.meta.functions.types.serialization;

import serde.core.meta.functions.tag_invokable;
export import serde.core.meta.functions.types.default_serializable;

export namespace serde::functions {

    struct serialize_tag {};

    template <typename Output, typename Input>
    concept TagInvokeSerializable = requires(Output& out, const Input& value) {
        tag_invoke(serialize_tag{}, out, value);
    };

    inline constexpr struct serialize_fn {
        template<typename Output, typename Input>
            requires TagInvokeSerializable<Output, Input> || DefaultSerializable<Output, Input>
        constexpr void operator()(Output& out, const Input& value) const {
            if constexpr (TagInvokeSerializable<Output, Input>){
                tag_invoke(serialize_tag{}, out, value);
            }
            else if constexpr(DefaultSerializable<Output, Input>){
                tag_invoke(default_serialize_tag{}, out, value);
            }
        }
    } serialize;
}
