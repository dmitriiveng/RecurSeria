export module serde.core.runtime.mapper:serialization;

import :implementation;
import serde.core.runtime.type_erasure.functions.serialization;

export namespace serde {
    template <typename OutputV, typename Key>
    using ISrlzFuncMapper = ISerDeFuncMapper<type_erased_srlz_func<OutputV>, Key>;

    template <typename OutputV, typename Key>
    using SrlzFuncMapper = SerDeFuncMapper<type_erased_srlz_func<OutputV>, Key>;
}