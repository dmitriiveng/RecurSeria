export module recurseria.core.meta.types_srlz_dsrlz:dsrlz;

export import recurseria.core.meta.tag_invokable;
import :default_dsrlz;

export namespace recurseria::core::meta {
    struct deserialize_tag {};

    template <typename Output, typename Input>
    concept TagInvokeDeserializable = requires(Output& out, const Input& value) {
        tag_invoke(deserialize_tag{}, out, value);
    };

    inline constexpr struct deserialize_fn {
        template<typename Output, typename Input>
            requires TagInvokeDeserializable<Output, Input> || DefaultDeserializable<Output, Input>
        constexpr void operator()(Output& out, const Input& value) const {
            if constexpr (TagInvokeDeserializable<Output, Input>){
                tag_invoke(deserialize_tag{}, out, value);
            }
            else if constexpr(DefaultDeserializable<Output, Input>){
                tag_invoke(default_deserialize_tag{}, out, value);
            }
        }
    } deserialize;
}
