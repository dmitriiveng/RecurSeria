export module recurseria.core.meta.types_srlz_dsrlz:serializable_noexcept;

import std;

export import :serializable;

export namespace recurseria::core::meta {
    template <typename FormatTag, typename Output, typename Input>
    concept SerializableNoexcept =
        (TagInvokeSerializable<FormatTag, Output, Input> &&
         noexcept(tag_invoke(FormatTag{}, serialize_tag{}, type_tag<Output>{}, std::declval<const Input&>()))) ||
        (DefaultSerializable<FormatTag, Output, Input> &&
         noexcept(tag_invoke(FormatTag{}, default_serialize_tag{}, type_tag<Output>{}, std::declval<const Input&>())));

    // Chain
    // Вот тут должна быть проветка noexcept(и тут вызов fold left)
    // И для более человекочитаемого вывода ошибки, добавь концепт,
    // который работает как TagInvokeSerializable (то есть крч просто что fold left можно вызвать)
    /*
    template <typename FormatTag, typename Output, typename Input, typename Chain>
    concept SerializableNoexcept =
        (TagInvokeSerializable<FormatTag, Output, Input> &&
         noexcept(tag_invoke(FormatTag{}, serialize_tag{}, type_tag<Output>{}, std::declval<const Input&>()))) ||
        (DefaultSerializable<FormatTag, Output, Input> &&
         noexcept(tag_invoke(FormatTag{}, default_serialize_tag{}, type_tag<Output>{}, std::declval<const Input&>())));
    */
}
