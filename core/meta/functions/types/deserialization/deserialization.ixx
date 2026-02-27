export module serde.core.meta.functions.types.deserialization;

import serde.core.meta.functions.tag_invokable;

namespace serde::functions {
    // Default

    void default_deserialize_fn();

    template <typename Output, typename Input>
    concept DefaultDeserializable = requires(Output& out, const Input& value) {
        default_deserialize_fn(out, value);
    };

    // User defined

    export struct deserialize_tag {};

    export template <typename Output, typename Input>
    concept TagInvokeDeserializable = requires(Output& out, const Input& value) {
        tag_invoke(deserialize_tag{}, out, value);
    };

    export inline constexpr struct deserialize_fn {
        template<typename Output, typename Input>
            requires TagInvokeDeserializable<Output, Input> || DefaultDeserializable<Output, Input>
        constexpr void operator()(Output& out, const Input& value) const
            noexcept(
                noexcept(tag_invoke(deserialize_tag{}, out, value)) 
                || (!TagInvokeDeserializable<Output, Input> && noexcept(default_deserialize_fn(out, value))))
        {
            if constexpr (TagInvokeDeserializable<Output, Input>){
                tag_invoke(deserialize_tag{}, out, value);
            }
            else{
                default_deserialize_fn(out, value);
            }
        }
    } deserialize;
}

