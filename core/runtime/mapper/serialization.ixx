export module serde.core.runtime.mapper:serialization;

import serde.core.runtime.mapper;
import serde.core.runtime.type_erasure.functions.serialization;

export namespace serde::srlz {
    template <typename OutputV, typename Key>
    using SrlzFuncMapper = SerDeFuncMapper<type_erased_srlz_func<OutputV>, Key>;
}