export module recurseria.core.runtime.mapper:serialization;

import :implementation;
export import recurseria.core.runtime.type_erasure.functions;

export namespace recurseria::core::runtime {
    template <typename OutputV, typename Key>
    using ISrlzFuncMapper = ISerDeFuncMapper<type_erased_srlz_func<OutputV>, Key>;

    template <typename OutputV, typename Key>
    using SrlzFuncMapper = SerDeFuncMapper<type_erased_srlz_func<OutputV>, Key>;
}
