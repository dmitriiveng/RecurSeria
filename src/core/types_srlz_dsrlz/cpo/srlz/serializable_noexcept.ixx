export module recurseria.srlz_dsrlz:serializable_noexcept;

import std;

export import :serializable;

export namespace recurseria {
    template <typename FormatTag, typename Output, typename Input>
    concept SerializableNoexcept =
        (TagInvokeSerializable<FormatTag, Output, Input> &&
         noexcept(tag_invoke(FormatTag{}, serialize_tag{}, type_tag<Output>{}, std::declval<const Input&>()))) ||
        (DefaultSerializable<FormatTag, Output, Input> &&
         noexcept(tag_invoke(FormatTag{}, default_serialize_tag{}, type_tag<Output>{}, std::declval<const Input&>())));

}
