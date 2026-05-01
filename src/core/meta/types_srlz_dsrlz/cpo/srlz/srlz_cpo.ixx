export module recurseria.core.meta.types_srlz_dsrlz:srlz;

export import recurseria.core.meta.tag_invokable;
import :default_srlz;

export namespace recurseria::core::meta {
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
