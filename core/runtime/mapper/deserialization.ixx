export module serde.core.runtime.mapper:deserialization;

import serde.core.runtime.mapper;
import serde.core.runtime.type_erasure.functions.deserialization;

export namespace serde::dsrlz {
    template <typename InputV, typename Key>
    using DsrlzFuncMapper = SerDeFuncMapper<type_erased_dsrlz_func<InputV>, Key>;
}