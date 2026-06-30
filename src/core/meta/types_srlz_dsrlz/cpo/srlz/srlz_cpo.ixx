export module recurseria.core.meta.types_srlz_dsrlz:srlz;

import std;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.chain;
export import recurseria.core.meta.exceptions;
export import :serializable;
export import :serializable_noexcept;

export namespace recurseria::core::meta {
    inline constexpr struct serialize_fn {
        template<typename FormatTag, typename Output, typename Input>
            requires SerializableNoexcept<FormatTag, Output, Input>
        constexpr Output as(const Input& value) const noexcept
        {
            if constexpr (TagInvokeSerializable<FormatTag, Output, Input> &&
                          noexcept(tag_invoke(FormatTag{}, serialize_tag{}, type_tag<Output>{}, value)))
            {
                return tag_invoke(FormatTag{}, serialize_tag{}, type_tag<Output>{}, value);
            } else {
                return tag_invoke(FormatTag{}, default_serialize_tag{}, type_tag<Output>{}, value);
            }
        }

        template<typename FormatTag, typename Output, typename Input>
            requires Serializable<FormatTag, Output, Input>
        constexpr Output as(const Input& value) const
        {
            if constexpr (TagInvokeSerializable<FormatTag, Output, Input>){
                try {
                    return tag_invoke(FormatTag{}, serialize_tag{}, type_tag<Output>{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("serialize", typeid(Input).name(), e.what());
                } catch (...) {
                    throw tag_invoke_error("serialize", typeid(Input).name(), "unknown exception");
                }
            }
            else {
                try {
                    return tag_invoke(FormatTag{}, default_serialize_tag{}, type_tag<Output>{}, value);
                } catch (const std::exception& e) {
                    throw tag_invoke_error("serialize", typeid(Input).name(), e.what());
                } catch (...) {
                    throw tag_invoke_error("serialize", typeid(Input).name(), "unknown exception");
                }
            }
        }

        // А тут просто сделай также как и выше, используя концепты (см serializable_noexcept.ixx)
        // Chain overloads
        template<typename FormatTag, typename Output, typename Input, typename Chain>
            requires IsChain<Chain>
        constexpr Output as(const Input& value) const {
            auto last = fold_left(
                [this]<typename T, typename Fmt = FormatTag>(const auto& v) -> decltype(auto) {
                    return this->template as<Fmt, T>(v);
                },
                value,
                Chain{}
            );
            return this->template as<FormatTag, Output>(last);
        }
    } serialize;
}
