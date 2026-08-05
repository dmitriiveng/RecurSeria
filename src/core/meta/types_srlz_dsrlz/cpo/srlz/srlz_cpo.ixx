export module recurseria.core.meta.types_srlz_dsrlz:srlz;

import std;

export import recurseria.core.meta.tag_invokable;
export import recurseria.core.meta.exceptions;
export import :serializable;
export import :serializable_noexcept;

export namespace recurseria {
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

    } serialize;
}
