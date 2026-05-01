export module recurseria.core.runtime.mapper:deserialization;

import :implementation;
import recurseria.core.runtime.type_erasure.functions;

export namespace recurseria::core::runtime {
    template <typename InputV, typename Key>
    using IDsrlzFuncMapper = ISerDeFuncMapper<type_erased_dsrlz_func<InputV>, Key>;

    template <typename InputV, typename Key>
    using DsrlzFuncMapper = SerDeFuncMapper<type_erased_dsrlz_func<InputV>, Key>;
}
