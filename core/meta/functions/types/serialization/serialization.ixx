export module serde.core.meta.functions.types.serialization;

import serde.core.meta.functions.tag_invokable;

namespace serde::functions {
    // Default

    void default_serialize_fn();

    template <typename Output, typename Input>
    concept DefaultSerializable = requires(Output& out, const Input& value) {
        default_serialize_fn(out, value);
    };

    // User defined

    export struct serialize_tag {};

    export template <typename Output, typename Input>
    concept TagInvokeSerializable = requires(Output& out, const Input& value) {
        tag_invoke(serialize_tag{}, out, value);
    };

    export inline constexpr struct serialize_fn {
        template<typename Output, typename Input>
            requires TagInvokeSerializable<Output, Input> || DefaultSerializable<Output, Input>
        constexpr void operator()(Output& out, const Input& value) const
            noexcept(
                noexcept(tag_invoke(serialize_tag{}, out, value))
                || (!TagInvokeSerializable<Output, Input> && noexcept(default_serialize_fn(out, value)))
            )
        {
            if constexpr (TagInvokeSerializable<Output, Input>){
                tag_invoke(serialize_tag{}, out, value);
            }
            else{
                default_serialize_fn(out, value);
            }
        }
    } serialize;
}

