export module recurseria.core.meta.types_srlz_dsrlz:dsrlz;

import std;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.chain;
export import recurseria.core.meta.exceptions;
export import :deserializable;

export namespace recurseria::core::meta {
    inline constexpr struct deserialize_fn {
        template<typename FormatTag, typename Output, typename Input>
            requires Deserializable<FormatTag, std::remove_cvref_t<Output>, Input>
        constexpr std::remove_cvref_t<Output> as(const Input& value) const {
            using CleanOutput = std::remove_cvref_t<Output>;
            if constexpr (TagInvokeDeserializable<FormatTag, CleanOutput, Input>){
                try {
                    return tag_invoke(FormatTag{}, deserialize_tag{}, type_tag<CleanOutput>{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("deserialize", typeid(CleanOutput).name(), e.what());
                }
            }
            else if constexpr(DefaultDeserializable<FormatTag, CleanOutput, Input>){
                try {
                    return tag_invoke(FormatTag{}, default_deserialize_tag{}, type_tag<CleanOutput>{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("deserialize", typeid(CleanOutput).name(), e.what());
                }
            }
        }

        template<typename FormatTag, typename Output, typename Input, typename Chain>
            requires IsChain<Chain>
        constexpr std::remove_cvref_t<Output> as(const Input& value) const {
            using CleanOutput = std::remove_cvref_t<Output>;
            auto last = fold_left(
                [this]<typename T, typename Fmt = FormatTag>(const auto& v) -> decltype(auto) {
                    return this->template as<Fmt, T>(v);
                },
                value,
                Chain{}
            );
            return this->template as<FormatTag, CleanOutput>(last);
        }
    } deserialize;
}
