export module serde.core.runtime.mapper:deserialization;

import :implementation;
import serde.core.runtime.type_erasure.functions.deserialization;

export namespace serde {
    template <typename InputV, typename Key>
    using IDsrlzFuncMapper = ISerDeFuncMapper<type_erased_dsrlz_func<InputV>, Key>;

    template <typename InputV, typename Key>
    using DsrlzFuncMapper = SerDeFuncMapper<type_erased_dsrlz_func<InputV>, Key>;
}